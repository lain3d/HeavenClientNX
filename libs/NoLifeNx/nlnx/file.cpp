//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////

#include "file_impl.hpp"
#include "node_impl.hpp"
#ifdef _WIN32
#  ifdef _MSC_VER
#    include <codecvt>
#  else
#    include <clocale>
#    include <cstdlib>
#  endif
#  ifdef __MINGW32__
#    include <windows.h>
#  else
#    include <Windows.h>
#  endif // __MINGW32__
#elif __linux__
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/fcntl.h>
#  include <sys/mman.h>
#  include <unistd.h>
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/fcntl.h>
#  include <stdio.h>
#endif
#include <stdexcept>

namespace nl {
    file::file(std::string name) {
        open(name);
    }
    file::~file() {
        close();
    }
    void file::open(std::string name) {
        close();
        m_data = new data();
#ifdef _WIN32
#  ifdef _MSC_VER
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        auto str = convert.from_bytes(name);
#  else
        std::setlocale(LC_ALL, "en_US.utf8");
        auto str = std::wstring(name.size(), 0);
        auto len = std::mbstowcs(const_cast<wchar_t *>(str.c_str()), name.c_str(), str.size());
        str.resize(len);
#  endif
#  if WINAPI_FAMILY == WINAPI_FAMILY_APP
        m_data->file_handle = ::CreateFile2(str.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);
#  else
        m_data->file_handle = ::CreateFileW(str.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, nullptr);
#  endif
        if (m_data->file_handle == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Failed to open file " + name);
#  if WINAPI_FAMILY == WINAPI_FAMILY_APP
        m_data->map = ::CreateFileMappingFromApp(m_data->file_handle, 0, PAGE_READONLY, 0, nullptr);
#  else
        m_data->map = ::CreateFileMappingW(m_data->file_handle, 0, PAGE_READONLY, 0, 0, nullptr);
#  endif
        if (!m_data->map)
            throw std::runtime_error("Failed to create file mapping of file " + name);
#  if WINAPI_FAMILY == WINAPI_FAMILY_APP
        m_data->base = ::MapViewOfFileFromApp(m_data->map, FILE_MAP_READ, 0, 0);
#  else
        m_data->base = ::MapViewOfFile(m_data->map, FILE_MAP_READ, 0, 0, 0);
#  endif
        if (!m_data->base)
            throw std::runtime_error("Failed to map view of file " + name);
#else
        //m_data->file_handle = ::open(name.c_str(), O_RDONLY);
        printf("[*] opening file handle for %s\n", name.c_str());
        std::string path = "HeavenClient/" + name;
        m_data->file_handle = ::fopen(path.c_str(), "r");
        if (m_data->file_handle == NULL) {
            printf("[!] failed to open file.\n");
            throw std::runtime_error("Failed to open file " + name);
        }
        struct stat finfo;
        //int fd = ::fileno(m_data->file_handle);
        //if (::fstat(fd, &finfo) == -1)
        //    printf("woops");
        //    throw std::runtime_error("Failed to obtain file information of file " + name);
        m_data->size = finfo.st_size;
        //m_data->base = ::mmap(nullptr, m_data->size, PROT_READ, MAP_SHARED, m_data->file_handle, 0);
        //m_data->base = ::fread(a, m_data->size)
        //if (reinterpret_cast<intptr_t>(m_data->base) == -1)
        //    throw std::runtime_error("Failed to create memory mapping of file " + name);
#endif
        //printf("[*] header creation\n");
        m_data->header = (header*) malloc(sizeof(header));
        //printf("[*] header: reading from file\n");
        fread((void*) (m_data->header), sizeof(header), 1, m_data->file_handle);
        if (m_data->header->magic != 0x34474B50) {
            printf("[*] file magic is not valid. magic was 0x%08x\n", m_data->header->magic);
            throw std::runtime_error(name + " is not a PKG4 NX file");
        }
        //m_data->node_table = reinterpret_cast<node::data const *>(reinterpret_cast<char const *>(m_data->base) + m_data->header->node_offset);

        // seek to offset where nodes live
        //printf("[*] node: seek node offset\n");
        ::fseek(m_data->file_handle, m_data->header->node_offset, SEEK_SET);
        size_t node_size = sizeof(node::data);
        // allocate memory for nodes
        //printf("[*] node table creation\n");
        m_data->node_table = (node::data*) (malloc(node_size * m_data->header->node_count));
        //node::data nodes[m_data->header->node_count];
        // read in node data
        ::fread((void*)m_data->node_table, sizeof(node::data), m_data->header->node_count, m_data->file_handle);

        //nodes = m_data->node_table;

        /* load string table */
        // seek to offset where strings live
        ::fseek(m_data->file_handle, m_data->header->string_offset, SEEK_SET);
        // allocate memory for strings
        m_data->string_table = (uint64_t*) (malloc(sizeof(uint64_t*) * m_data->header->string_count));
        // read in string data
        ::fread((uint64_t*)m_data->string_table, sizeof(uint64_t*), m_data->header->string_count, m_data->file_handle);

        // read in bitmap table data
        m_data->bitmap_table = (uint64_t*) (malloc(sizeof(uint64_t*) * m_data->header->bitmap_count));
        ::fseek(m_data->file_handle, m_data->header->bitmap_offset, SEEK_SET);
        ::fread((uint64_t*)m_data->bitmap_table, sizeof(uint64_t*), m_data->header->bitmap_count, m_data->file_handle);

        // read in audio table data
        m_data->audio_table = (uint64_t*) (malloc(sizeof(uint64_t*) * m_data->header->audio_count));
        ::fseek(m_data->file_handle, m_data->header->audio_offset, SEEK_SET);
        ::fread((uint64_t*)m_data->audio_table, sizeof(uint64_t*), m_data->header->audio_count, m_data->file_handle);
    }
    void file::close() {
        if (!m_data) return;
#ifdef _WIN32
        ::UnmapViewOfFile(m_data->base);
        ::CloseHandle(m_data->map);
        ::CloseHandle(m_data->file_handle);
#else
        //::munmap(const_cast<void *>(m_data->base), m_data->size);
        ::fclose(m_data->file_handle);
#endif
        delete m_data;
        m_data = nullptr;
    }
    node file::root() const {
        return {m_data->node_table, m_data};
    }
    file::operator node() const {
        return root();
    }
    uint32_t file::string_count() const {
        return m_data->header->string_count;
    }
    uint32_t file::bitmap_count() const {
        return m_data->header->bitmap_count;
    }
    uint32_t file::audio_count() const {
        return m_data->header->audio_count;
    }
    uint32_t file::node_count() const {
        return m_data->header->node_count;
    }
    std::string file::get_string(uint32_t i) const {
        auto const s = reinterpret_cast<char const *>(m_data->base) + m_data->string_table[i];
        return {s + 2, *reinterpret_cast<uint16_t const *>(s)};
    }
}
