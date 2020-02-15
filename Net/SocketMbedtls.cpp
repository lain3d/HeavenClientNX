//
// Created by rich on 2/2/20.
//

#include "SocketMbedtls.h"

//#ifdef USE_MBEDTLS
namespace ms
{
	SocketMbedtls::SocketMbedtls() {
	    ctx = mbedtls_net_context{MBEDTLS_FD};
	    mbedtls_net_init(&ctx);
	    if (mbedtls_net_set_nonblock(&ctx)) {
	    	printf("Error setting nonblock\n");
	    }
	}

	SocketMbedtls::~SocketMbedtls()
	{
	}

	bool SocketMbedtls::open(const char* address, const char* port)
	{
		int status;
		int error = mbedtls_net_connect(&ctx, address, port, MBEDTLS_NET_PROTO_TCP);
		int read;

		if (!error) {
			status = mbedtls_net_poll(&ctx, MBEDTLS_NET_POLL_READ, 20000); // todo: change timeout
			if (status > 0) {
				read = mbedtls_net_recv(&ctx, buffer, MAX_PACKET_LENGTH);
				if (read < 0) // we are already blocking with poll, is this necesssary?
					printf("Error reading from socket\n");
				return (read == HANDSH) && !error;
			}
		} else {
			printf("Socket error %x\n", error);
		}
		return true;
	}

	bool SocketMbedtls::close()
	{
		mbedtls_net_free(&ctx);
		return true;
	}

	std::size_t SocketMbedtls::receive(bool* recvok)
	{
		int status;
		mbedtls_net_
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