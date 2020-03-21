//
// Created by rich on 2/2/20.
//

#include "SocketMbedtls.h"
#include <iostream>

//#ifdef USE_MBEDTLS
namespace ms
{
	SocketMbedtls::SocketMbedtls() {
		printf("mbedtls_net_context\n");
	    ctx = mbedtls_net_context{MBEDTLS_FD};
	    printf("mbedtls_net_init\n");
	    mbedtls_net_init(&ctx);
	    printf("finished socket init\n");
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
			if (mbedtls_net_set_nonblock(&ctx)) {
				printf("Error setting nonblock\n");
			}
			status = mbedtls_net_poll(&ctx, MBEDTLS_NET_POLL_READ, 20000); // todo: change timeout
			if (status > 0) {
				read = mbedtls_net_recv(&ctx, (unsigned char*) buffer, MAX_PACKET_LENGTH);
				if (read < 0) // we are already blocking with poll, is this necesssary?
					printf("Error reading from socket\n");
				return (read == HANDSHAKE_LEN) && !error;
			}
		} else {
			//printf("Socket error %x\n", error);

            printf("socket error %c%04X",(error<0)?'-':' ',(error<0)?-error:error);
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
		//int read = mbedtls_net_recv_timeout(&ctx, (unsigned char*) buffer, MAX_PACKET_LENGTH, 0);
		int read = mbedtls_net_recv(&ctx, (unsigned char*) buffer, MAX_PACKET_LENGTH);
		if (read > 0) {
			*recvok = read > 0;
			return (size_t) read;
		}
		return 0;
	}

	const int8_t* SocketMbedtls::get_buffer() const
	{
		return buffer;
	}

	bool SocketMbedtls::dispatch(const int8_t* bytes, std::size_t length)
	{
		int status = mbedtls_net_poll(&ctx, MBEDTLS_NET_POLL_WRITE, 20000);
		int result;
		if (status > 0) {
			result = mbedtls_net_send(&ctx, (unsigned char*) bytes, length);
		}
		return (status > 0) && result == length;
	}
}
//#endif