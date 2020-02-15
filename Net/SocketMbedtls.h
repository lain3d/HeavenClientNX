//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../MapleStory.h"

//#ifdef USE_MBEDTLS
#include "NetConstants.h"
#include <mbedtls/net_sockets.h>

#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#define MBEDTLS_FD 202

namespace ms
{
#ifndef USE_CRYPTO
    const std::size_t HANDSHAKE_LEN = 2;
#else
    const std::size_t HANDSHAKE_LEN = 16;
#endif

    // Class that wraps an asio socket.
    class SocketMbedtls
    {
    public:
        SocketMbedtls();
        ~SocketMbedtls();

        bool open(const char* address, const char* port);
        bool close();
        std::size_t receive(bool* connected);
        const int8_t* get_buffer() const;
        bool dispatch(const int8_t* bytes, std::size_t length);

    private:
        mbedtls_net_context ctx;
        //io_service ioservice;
        //tcp::resolver resolver;
        //tcp::socket socket;
        int8_t buffer[MAX_PACKET_LENGTH];
    };
}
//#endif