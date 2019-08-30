#include "Client.hpp"

namespace AndroidConnect {
#define ALLOCATE_MEM(type, size) (type*)malloc(sizeof(type) * size)
#define GETSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
	
	Client::Client(SSL* ssl, unsigned int ThreadId) {
		this->ssl = ssl;
		this->ThreadId = ThreadId;
		inits["0"] = rttr::type::get_by_name("0").create();

	}


	void Client::Callback(void* message, int type, int size) {
		printf("Sending!\n");
		unsigned char dataSize[2]{ (size >> 8) & 0x000000FF, size & 0x000000FF }, dataType[2]{ (type >> 8) & 0x000000FF, type & 0x000000FF };

		printf("Callback!\n");
		SSL_write(ssl, dataSize, 2);
		SSL_write(ssl, dataType, 2);
		SSL_write(ssl, message, size);
		printf("Sent!");
	}
	



	void Client::StartHandling() {
		int dataAmount = 0;
		do {
			unsigned char dataSize[2]{ 0 }, dataType[2]{ 0 };
			SSL_read(ssl, dataSize, 2);
			SSL_read(ssl, dataType, 2);
			int dataSize_i = ((dataSize[0] << 8) | dataSize[1]);
			std::string dataType_s = std::to_string((dataType[0] << 8) | dataType[1]);
			printf("Hello!\n");
			printf("%s\n", dataType_s.c_str());

			unsigned char* data = (unsigned char*)malloc(dataSize_i);
			dataAmount = SSL_read(ssl, data, dataSize_i);
			rttr::type::get_by_name(dataType_s).get_method("Invoke").invoke(rttr::type::get_by_name(dataType_s).create(), data, dataSize_i, (unsigned int)0);

		} while (dataAmount);
	}



	Client::~Client() {
	
	}
}