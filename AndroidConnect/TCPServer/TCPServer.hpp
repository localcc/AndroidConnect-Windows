#pragma once
#include "OpenSSLHelper.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>


#include <Windows.h>
#include "../WinAPI/Notification/NotificationHelper.hpp"
#include <map>
#include <vector>
#include <rttr/registration.h>
#include <rttr/method.h>
#include <thread>
#include "../WinAPI/Notification/NotificationHelper.hpp"
#include <openssl/crypto.h>
#include "Client.hpp"
//PLUGIN Ids: 
//NotificationPlugin ID: 0
//


#pragma comment(lib,"ws2_32")
namespace AndroidConnect {
#define MEMALLOC(type, size) (type*)malloc(sizeof(type) * size) 
	class TCPServer {
	public:
		static TCPServer* GetInstance() { return currentInstance; }
		TCPServer();
		~TCPServer();
		bool InitializeWSAData();
		SOCKET create_socket();
		bool listen_socket(SOCKET socket);
		void StartServer();


		static CRYPTO_RWLOCK *mutex;
		Client* GetClient(unsigned int id);

	private:
		template<class T>
		T calcBytes(T* data, T offset) {
			return(data[0] - offset) | (data[1] - offset);
		}
		void HandleClient(SSL* client);
		std::unordered_map<std::string, rttr::variant> classesMap;
		std::unordered_map<unsigned int, Client*> clients;
		short ltid;
		static TCPServer* currentInstance;

	};
}