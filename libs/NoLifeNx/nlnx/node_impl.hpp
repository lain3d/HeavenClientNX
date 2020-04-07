//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright � 2013 Peter Atashian                                          //
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

#pragma once
#include "node.hpp"

/*
 * Idea for new memory model:
 * - Maximum length buffer that uses key value(data) pairs
 * Initialization
 * 1. Create a buffer for the file that will load assets.
 * 2. When a new asset is loaded, place the asset into buffer. The key will contain the "num" and the value will contain
 * a pointer to the data within the buffer
 *
 * The buffer itself:
 * Could grow dynamically as needed or have an initial size that never gets changed. Will try the initial size first.
 *
 * We are already keeping a buffer for the data tables, it may be a better idea to only have all nodes loaded, then
 * do [m_data->num, ptr->table_offset] where ptr->table_offset contains the actual location in the file where the data
 * requested resides.
 */
namespace nl {
    //Internal data structure
#pragma pack(push, 1)
    struct node::data {
        uint32_t const name;
        uint32_t const children;
        uint16_t const num;
        node::type const type;
        union {
            int64_t const ireal;
            double const dreal;
            uint32_t const string;
            int32_t const vector[2];
            struct {
                uint32_t index;
                uint16_t width;
                uint16_t height;
            } const bitmap;
            struct {
                uint32_t index;
                uint32_t length;
            } const audio;
        };
    };
#pragma pack(pop)
}
