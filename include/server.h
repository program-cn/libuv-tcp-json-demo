#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <uv.h>
#include <string>

namespace uv
{
    typedef struct
    {
        uv_write_t req;
        uv_buf_t buf;
    } write_req_t;

    class UvServer
    {
    public:
        UvServer(/* args */);
        UvServer(const std::string& ip_addr, const int ip_port);

        ~UvServer();
        int  init();
        bool run();
        bool disconnect(){};
        void setIp(const int ip_port, const std::string& ip_addr);
    private:
        static void free_write_req(uv_write_t *req);
        static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
        
        void echo_write(uv_write_t *req, int status);
        void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
        void on_new_connection(uv_stream_t *server, int status);
        //bool createListenThread();
        static std::string getUVError(int retcode);

    /* data */    
    private:
        bool is_init_; 
        int ip_port_;
        std::string ip_;
        int default_backlog_;

        uv_loop_t* loop_;
        struct sockaddr_in addr_;

        uv_mutex_t write_mutex_;
        uv_tcp_t server_;
        uv_thread_t uv_thread_;
    };

} // namespace uv

#endif __SERVICE_H__