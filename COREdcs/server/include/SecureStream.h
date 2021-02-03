#ifndef DCS_SECURE_STREAM_H
#define DCS_SECURE_STREAM_H
#include "ConnectionStream.h"
#include <openssl/err.h>
#include <openssl/ssl.h>
class SecureStream : public ConnectionStream {
  private:
    SSL *ssl;
    int sock;
    const SSL_METHOD *meth = TLSv1_2_client_method();

  public:
    SecureStream(int sd);
    virtual ~SecureStream();
    ssize_t send(char *buffer, size_t len) override {
        auto s = SSL_write(ssl, buffer, len);
        if(s < 0) {
            throw std::runtime_error("Failed send");
        }
        return s;
    }
    ssize_t receive(char *buffer, size_t len) override {
        auto s = SSL_read(ssl, buffer, len);
        if(s < 1) {
            throw std::runtime_error("Failed receive");
        }
        return s;
    }
    bool initOnce() {
        SSL_library_init();
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();
        return true;
    }
};

SecureStream::SecureStream(int sd) : ConnectionStream(sd) {
    static bool init = initOnce();
    if(init) {
    }
    SSL_CTX *ctx = SSL_CTX_new(meth);
    ssl = SSL_new(ctx);
    sock = SSL_get_fd(ssl);
    SSL_set_fd(ssl, sd);
    int err = SSL_connect(ssl);
    if(err <= 0) {
        throw std::runtime_error("Failed SSL connection");
    }
}

SecureStream::~SecureStream() {}

#endif  // DCS_SECURE_STREAM_H