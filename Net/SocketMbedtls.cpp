//
// Created by rich on 2/2/20.
//

#include "SocketMbedtls.h"

//#ifdef USE_MBEDTLS
namespace ms
{
	SocketMbedtls::SocketMbedtls() {}

	SocketMbedtls::~SocketMbedtls()
	{
	}

	bool SocketMbedtls::open(const char* address, const char* port)
	{
		return true;
	}

	bool SocketMbedtls::close()
	{
		return false;
	}

	std::size_t SocketMbedtls::receive(bool* recvok)
	{
		return 0;
	}

	const int8_t* SocketMbedtls::get_buffer() const
	{
		return nullptr;
	}

	bool SocketMbedtls::dispatch(const int8_t* bytes, std::size_t length)
	{
		return false;
	}
}
//#endif