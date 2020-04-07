cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(NoLifeNx CXX)
set(CMAKE_BUILD_TYPE Debug)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    add_compile_options(-std=c++17)
endif()

if(SWITCH_LIBNX)
    add_definitions(-D__SWITCH__=1)
    add_definitions(-DSTATIC_LIBVORBIS=1)
    add_definitions(-DPLATFORM_UNIX=1)
    add_definitions(-DPLATFORM_SWITCH=1)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-D_LARGEFILE_SOURCE=1)
    add_definitions(-DPRAGMA_ONCE=1)
    include_directories("$ENV{PORTLIBS_PREFIX}/include")
    include_directories("$ENV{DEVKITPRO}/libnx/include")
    add_compile_options(-march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIE -mcpu=cortex-a57+crc+fp+simd)
    add_compile_options(-fsigned-char -Wno-strict-aliasing -Wfatal-errors -Wno-sign-compare)
endif()

aux_source_directory(. NOLIFENX_SOURCES)
add_library(NoLifeNx ${NOLIFENX_SOURCES})
if(${BUILD_SHARED_LIBS})
    install(TARGETS NoLifeNx LIBRARY DESTINATION lib)
else()
    install(TARGETS NoLifeNx LIBRARY ARCHIVE DESTINATION lib)
endif()
file(GLOB NOLIFENX_HEADERS . *.hpp)
install(FILES 
audio.hpp
bitmap.hpp
file.hpp
node.hpp
nxfwd.hpp
nx.hpp
DESTINATION include/nx)

target_include_directories(NoLifeNx PUBLIC ${PROJECT_SOURCE_DIR}/lz4-1.8.2/lib/)
target_link_libraries(NoLifeNx ${PROJECT_SOURCE_DIR}/lz4-1.8.2/lib/liblz4.a)
