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

// Define program attributes here

// USE_ASIO : Use asio for networking, if not defined use Winsock. TODO: rich

#if defined(__SWITCH__)
#define USE_MBEDTLS
#endif

#if defined(__linux__)
#define USE_ASIO
#endif

#if defined(WIN32)
#define USE_WINSOCK
#endif

// USE_XXHASH : Use xxhash for file check (additional dependency)
//#define USE_XXHASH

// USE_CRYPTO : Use cryptography for communication with the server.
#define USE_CRYPTO

// PRINT_WARNINGS : Print warnings and minor errors to the console.
#define PRINT_WARNINGS