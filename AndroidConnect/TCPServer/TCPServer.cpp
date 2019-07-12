#include "TCPServer.hpp"

#include <openssl/applink.c>
#include <thread>
#include "../WinAPI/Notification/NotificationHelper.hpp"
#include <charconv>
#include "../Plugins/Notification.pb.h"


#define PORT "4908"


namespace AndroidConnect {


	TCPServer::TCPServer() {
		if (!InitializeWSAData()) TCPServer::~TCPServer();
		OpenSSLHelper::InitializeOpenSSL();
		//Loop start
		helper = NotificationHelper();
		//Loop end
	}

	TCPServer::~TCPServer() {

	}

	void TCPServer::StartServer() {

		SOCKET ListeningSocket = TCPServer::create_socket();
		TCPServer::listen_socket(ListeningSocket);

		SOCKET UnsecureClient;
		SSL_CTX* ctx = OpenSSLHelper::create_context();
		OpenSSLHelper::configure_context(ctx, "cert.pem", "key.pem");


		while (true) {

			UnsecureClient = INVALID_SOCKET;
			UnsecureClient = accept(ListeningSocket, NULL, NULL);
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
				std::thread t([this, ssl] { this->HandleClient(ssl); });
				t.detach();

			}
		}
		
	}


	void TCPServer::HandleClient(SSL* client) {
		bool hasData = true;
		do {
			byte dataSizeRecv[2];

			hasData = SSL_read(client, dataSizeRecv, 2);
			byte dataTypeRecv[2];
			hasData = SSL_read(client, dataTypeRecv, 2);
			int dataSize = (dataSizeRecv[0] << 8) | (dataSizeRecv[1]);
			char dataType[2];
			std::to_chars(dataType, dataType + 1, ((dataTypeRecv[0] << 8) | (dataTypeRecv[1])));
			byte* data = new byte[dataSize];
			hasData = SSL_read(client, data, dataSize);
			helper.SendNotification(data, dataSize);

		} while (hasData);
	}



	bool TCPServer::InitializeWSAData() {
		WSAData wsaData;
		int iResult;
		if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) return false;
		return true;
	}

	SOCKET TCPServer::create_socket() {
		int iResult;
		struct addrinfo* result, * ptr, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		if ((iResult = getaddrinfo(NULL, PORT, &hints, &result)) != 0) {
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


		iResult = bind(ListeningSocket, result->ai_addr, (int)result->ai_addrlen);
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