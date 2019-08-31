#include "TCPServer.hpp"

#include <openssl/applink.c>

#include <charconv>
#include "../Plugins/Notification.pb.h"


#define PORT "4908"


RTTR_REGISTRATION{
rttr::registration::class_<AndroidConnect::NotificationHelper>("0").constructor<>()
.method("Invoke", &AndroidConnect::NotificationHelper::SendNotification);
}

namespace AndroidConnect {


	std::vector<SSL*> sockets;


	TCPServer* TCPServer::currentInstance = nullptr;

	TCPServer::TCPServer() {
		ltid = 0;
		if (!InitializeWSAData()) this->~TCPServer();
		OpenSSLHelper::InitializeOpenSSL();
		//Loop start
		//Loop end
		currentInstance = this;
	}

	TCPServer::~TCPServer() {

	}


	static void ProcessorFunc(LPCWSTR args, NOTIFICATION_USER_INPUT_DATA const* data, ULONG count) {
		printf("Something happened!\n");
	}

	void TCPServer::StartServer() {
		const SOCKET ListeningSocket = create_socket();
		
		listen_socket(ListeningSocket);

		SSL_CTX* ctx = OpenSSLHelper::create_context();
		OpenSSLHelper::configure_context(ctx, "cert.pem", "key.pem");

		unsigned int clientid = 0;
	
		while (true) {
			const SOCKET UnsecureClient = accept(ListeningSocket, nullptr, nullptr);
			if (UnsecureClient == SOCKET_ERROR) {
				closesocket(ListeningSocket);
				WSACleanup();
				break;
			}
			SSL* ssl = SSL_new(ctx);

			SSL_set_fd(ssl, UnsecureClient);
			
			if (SSL_accept(ssl) <= 0) {
				printf("Failed to ssl accept the client! Restart program as admin or report bug!");
			}
			else {
				clients[clientid] = &Client(ssl, clientid);
				clients[clientid++]->StartHandling();

			}
		}

		
	}



	Client* TCPServer::GetClient(unsigned int id) {
		return clients[id];
	}

	bool TCPServer::InitializeWSAData() {
		WSAData wsaData;	
		int iResult;
		return (iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) == 0;
	}

	SOCKET TCPServer::create_socket() {
		int iResult;
		struct addrinfo* result, * ptr, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		if ((iResult = getaddrinfo(nullptr, PORT, &hints, &result)) != 0) {
			WSACleanup();
			return NULL;
		}
		SOCKET ListeningSocket = INVALID_SOCKET;

		ListeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListeningSocket == INVALID_SOCKET) {
			freeaddrinfo(result);
			WSACleanup();
			return NULL;
		}


		iResult = bind(ListeningSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			freeaddrinfo(result);
			closesocket(ListeningSocket);
			WSACleanup();
			return NULL;
		}
		freeaddrinfo(result);
		return ListeningSocket;
	}

	bool TCPServer::listen_socket(SOCKET socket) {
		
		if (listen(socket, 0) == SOCKET_ERROR) {
			closesocket(socket);
			WSACleanup();
			return false;
		}
		return true;
	}



}
