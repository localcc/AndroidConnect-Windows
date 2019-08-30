#pragma once

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#pragma comment(lib, "crypt32")

namespace AndroidConnect {
	class OpenSSLHelper {
	public:
		static EVP_PKEY* GenerateKey();
		static X509* GenerateX509(EVP_PKEY* pkey);
		static bool WriteToDisk(X509* cert, EVP_PKEY* pkey);
		static void InitializeOpenSSL();
		static SSL_CTX* create_context();
		static void configure_ssl(SSL* ssl);
		static void configure_context(SSL_CTX* ctx, const char* certPath, const char* keyPath);
	};
}