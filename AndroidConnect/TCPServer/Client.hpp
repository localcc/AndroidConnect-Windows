#pragma once
#include "OpenSSLHelper.hpp"
#include <string>
#include "rttr/registration.h"
#include <unordered_map>



namespace AndroidConnect {
	
	class Client {
	public:
		Client(SSL*, unsigned int ThreadId);
		void StartHandling();
		void Callback(void* data, int type, int size);
		~Client();
	private:
		SSL* ssl;
		unsigned int ThreadId;
		std::unordered_map<std::string, rttr::variant> inits;
	};
}
