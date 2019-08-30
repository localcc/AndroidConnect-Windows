#include <cstdio>

#include "WinAPI/Notification/NotificationHelper.hpp"
#include "WinAPI/Notification/XmlGenerator.hpp"
#include "TCPServer/TCPServer.hpp"
#include "TCPServer/OpenSSLHelper.hpp"
#include <filesystem>


void startup() {
	winrt::init_apartment();
	if (!std::filesystem::exists("key.pem") || !std::filesystem::exists("cert.pem")) {
		EVP_PKEY* pkey = AndroidConnect::OpenSSLHelper::GenerateKey();
		X509* cert = AndroidConnect::OpenSSLHelper::GenerateX509(pkey);
		bool success = AndroidConnect::OpenSSLHelper::WriteToDisk(cert, pkey);
		if (!success) {
			printf("Error creating certificate, run program as admin, or report bug!");
		}
	}
	AndroidConnect::TCPServer server = AndroidConnect::TCPServer::TCPServer();
	server.StartServer();
}

int main(int argc, char** argv) {
	if (argc > 0) {
		std::string argvv(argv[0]);
		if (argvv.find("-ToastActivated") != argvv.npos) {

		}
		else {
			startup();
		}
	}else{
		startup();
	}
	
}