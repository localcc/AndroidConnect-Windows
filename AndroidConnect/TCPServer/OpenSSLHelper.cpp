#include "OpenSSLHelper.hpp"

#define OPENSSL_NO_TLS1_METHOD
#define OPENSSL_NO_TLS1_1_METHOD
#define OPENSSL_NO_SSL3_METHOD
#define OPENSSL_NO_DTLS1_METHOD
#define OPENSSL_NO_DTLS1_2_METHOD

namespace AndroidConnect {
	void OpenSSLHelper::InitializeOpenSSL() {
		SSL_library_init();
		OpenSSL_add_all_algorithms();
	}

	void OpenSSLHelper::configure_ssl(SSL* ssl) {
		SSL_set_cipher_list(ssl, "TLS_ECDHE_RSA_AES256_GCM_SHA384");
	}

	SSL_CTX* OpenSSLHelper::create_context() {
		SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());
		
		if (!ctx) return NULL;
		return ctx;
	}

	void OpenSSLHelper::configure_context(SSL_CTX* ctx, const char* certPath, const char* keyPath) {
		SSL_CTX_set_ecdh_auto(ctx, 1);

		if (SSL_CTX_use_certificate_file(ctx, certPath, SSL_FILETYPE_PEM) <= 0) /* Failed! */;
		if (SSL_CTX_use_PrivateKey_file(ctx, keyPath, SSL_FILETYPE_PEM) <= 0) /* Failed! */;
	}

	



	EVP_PKEY* OpenSSLHelper::GenerateKey() {
		EVP_PKEY* pkey = EVP_PKEY_new();
		if (!pkey) return NULL;

		RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
		if (!EVP_PKEY_assign_RSA(pkey, rsa)) return NULL;
		return pkey;
	}

	X509* OpenSSLHelper::GenerateX509(EVP_PKEY* pkey) {
		X509* x509 = X509_new();
		if (!x509) return NULL;

		ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

		X509_gmtime_adj(X509_get_notBefore(x509), 0);
		X509_gmtime_adj(X509_get_notAfter(x509), 0);

		X509_set_pubkey(x509, pkey);

		X509_NAME* name = X509_get_subject_name(x509);

		X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"CA", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"localChicken", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"localhost", -1, -1, 0);

		X509_set_issuer_name(x509, name);

		if (!X509_sign(x509, pkey, EVP_sha1())) return NULL;
		return x509;
	}

	bool OpenSSLHelper::WriteToDisk(X509* cert, EVP_PKEY* pkey) {
		FILE* pkey_file;
		fopen_s(&pkey_file, "cert.pem", "wb");
		if (!pkey_file) return false;

		bool ret = PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
		fclose(pkey_file);
		if (!ret) return false;

		FILE* x509_file;
		fopen_s(&x509_file, "key.pem", "wb");
		if (!x509_file) return false;

		ret = PEM_write_X509(x509_file, cert);
		fclose(x509_file);
		if (!ret) return false;
		return true;
	}
}