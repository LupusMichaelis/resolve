#include <arpa/inet.h>
#include <stdbool.h>
#include <memory.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

bool get_address_of(char * in, char * out, size_t size_out);

int main(int argc, char **argv)
{
	if(argc < 2)
		return EXIT_FAILURE;

	bool shit_happened = false;
	for(int position = 1; position < argc; ++position)
	{
		char * p_candidate = *(argv + position);

		char p_ip_address[100];
		bzero(p_ip_address, sizeof p_ip_address);
		int success = get_address_of(p_candidate, p_ip_address, sizeof p_ip_address);

		if(success)
			printf("%s: %s\n", p_candidate, p_ip_address);
		else
		{
			shit_happened = true;
			printf("Can not resolve '%s'\n", p_candidate);
		}
	}

	if(shit_happened)
		return EXIT_FAILURE;
}

bool get_address_of(char * p_candidate, char * p_ip_address, size_t ip_address_size)
{
	struct addrinfo * result;
	struct addrinfo hints;

	bzero(&hints, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	int success = getaddrinfo(p_candidate, NULL, &hints, &result);

	if(0 != success)
	{
		fprintf(stderr, "getaddrinfo: '%s'\n", gai_strerror(success));
		return false;
	}
	
	void * p = NULL;
	switch(result->ai_family)
	{
		case AF_INET:
			p = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
			break;
		case AF_INET6:
			p = &((struct sockaddr_in6 *) result->ai_addr)->sin6_addr;
			break;
	}

	inet_ntop(result->ai_family, p, p_ip_address, ip_address_size);
	freeaddrinfo(result);

	return true;
}
