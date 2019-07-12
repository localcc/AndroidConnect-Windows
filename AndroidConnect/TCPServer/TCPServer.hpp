#pragma once
#include "OpenSSLHelper.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>


#include <Windows.h>
#include "../WinAPI/Notification/NotificationHelper.hpp"
#include <map>



//PLUGIN Ids: 
//NotificationPlugin ID: 0
//


#pragma comment(lib,"ws2_32")
namespace AndroidConnect {

	class TCPServer {
	public:

		TCPServer();
		~TCPServer();
		bool InitializeWSAData();
		SOCKET create_socket();
		bool listen_socket(SOCKET socket);
		void StartServer();

		NotificationHelper helper;
	private:
		template<class T>
		T calcBytes(T* data, T offset) {
			return(data[0] - offset) | (data[1] - offset);
		}

		void HandleClient(SSL* client);
	};
}