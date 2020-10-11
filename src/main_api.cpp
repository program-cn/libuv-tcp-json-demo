#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "nlohmann_json.hpp"
#include <iostream>

#define DEFAULT_PORT 5550 //7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    printf("1 buf->len: %d\n", buf->len);
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
    printf("2 buf->len: %d\n", buf->len);
}

void echo_write(uv_write_t *req, int status) {
    printf("1 echo_write: %d\n", status);    
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
    printf("2 echo_write: %d\n", status);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    printf("1 nread: %d\n", nread);
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        printf("1.5 req->buf: %s\n", req->buf);
        uv_buf_t return_data;
        char return_char[10] = "return";
        return_data.base = return_char;
        return_data.len = 7;
        uv_write((uv_write_t*) req, client, &return_data/* &req->buf*/, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    }
    printf("2 nread: %d\n", nread);
    free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    printf("1 on_new_connection: %d\n", status);
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
    printf("2 on_new_connection: %d\n", status);    
}

int main()
{
//demo json
    nlohmann::json obj = {
        {"bazel", "https://bazel.build"},
        {"cmake", "https://cmake.org/"},
    };
    std::string str = obj.dump(4);
    std::cout << str << std::endl;

//libuv
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
