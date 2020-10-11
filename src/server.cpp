#include "server.h"
#include "uv-cpp.h"

#include "glog/logging.h"
#include <functional>
#include <iostream>

namespace uv
{
     UvServer::UvServer(const std::string& ip_addr, const int ip_port): is_init_(false), ip_port_(ip_port), 
                ip_("127.0.0.1"), default_backlog_(128), loop_(NULL)
    {
        init();
        run();
    }

    UvServer::UvServer(/* args */) : is_init_(false), ip_port_(5550), default_backlog_(128), loop_(NULL), ip_("127.0.0.1")
    {
        init();
    }

    UvServer::~UvServer()
    {
    }

    int UvServer::init() 
    {
        loop_ = uv_default_loop();
    
		if (!loop_) {
			LOG(INFO) << "loop is null on tcp init.";
			return -1;
		}
		int iret = uv_tcp_init(loop_, &server_);
		if (iret)
        {
            LOG(ERROR)<<getUVError(iret).c_str();
			return -2;
		}

		iret = uv_mutex_init(&write_mutex_);
		if (iret) {
			printf(getUVError(iret).c_str());
			return -3;
		}
		is_init_ = true;

        return 1;
    }

    void UvServer::setIp(const int ip_port, const std::string& ip) 
    {
        ip_port_ = ip_port;
        ip_ = ip;
    }

    // bool UvServer::createListenThread() 
    // {
    //   	int iret = uv_thread_create(&uv_thread_, run, this);//触发AfterConnect才算真正连接成功，所以用线程
	// 	if (iret) {
	// 		LOG(INFO) << (getUVError(iret).c_str());
	// 		return false;
	// 	}
    //     return true;
    // }

    bool UvServer::run() 
    {
		LOG(INFO) << "run() ";
        uv_ip4_addr(ip_.c_str(), ip_port_, &addr_);

        uv_tcp_bind(&server_, (const struct sockaddr*)&addr_, 0);
        uv_connection_cb2 fp = std::bind(&UvServer::on_new_connection, this, std::placeholders::_1, std::placeholders::_2);
        int r = uv_listen_x((uv_stream_t*) &server_, default_backlog_, fp);
        //int r = uv_listen((uv_stream_t*) &server_, default_backlog_, on_new_connection);

        if (r) {
            LOG(ERROR)<<"Listen error:"<<getUVError(r).c_str();
            return 1;
        }
        return uv_run(loop_, UV_RUN_DEFAULT);
    }

    void UvServer::on_new_connection(uv_stream_t *server, int status)
    {
        LOG(INFO) << "UvServer::on_new_connection ";
        LOG(INFO)<<"1 on_new_connection: "<<status;
        if (status < 0)
        {
            LOG(ERROR)<<"Read error:"<< getUVError(status).c_str();
            return;
        }

        uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
        uv_tcp_init(loop_, client);
        if (uv_accept(server, (uv_stream_t *)client) == 0)
        {
            uv_read_cb3 fp = std::bind(&UvServer::echo_read, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            uv_read_start_x((uv_stream_t *)client, alloc_buffer, fp);

            //uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
        }
        else
        {
            uv_close((uv_handle_t *)client, NULL);
        }
        //LOG(INFO)<<"2 on_new_connection: "<<status;
    }
    
    void UvServer::echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
    {
        LOG(INFO)<<"1 nread: "<<nread;
        if (nread > 0)
        {
            write_req_t *req = (write_req_t *)malloc(sizeof(write_req_t));
            req->buf = uv_buf_init(buf->base, nread);
            std::cout<<":: "<<buf->base<<std::endl;
            LOG(INFO)<<"==>> req->buf: "<<req->buf.len<<" || "<<req->buf.base<<" || ";
            LOG(INFO)<<" *****";
            LOG(INFO)<<"==>> req->buf: "<<req->buf.len<<" || "<<req->buf.base<<" || ";
            uv_buf_t return_data;
            char return_char[10] = "return";
            return_data.base = return_char;
            return_data.len = 7;
            //uv_write((uv_write_t *)req, client, &return_data /* &req->buf*/, 1, echo_write);
            uv_write_cb2 fp = std::bind(&UvServer::echo_write, this, std::placeholders::_1, std::placeholders::_2);
            uv_write_x((uv_write_t *)req, client,/* &return_data*/ &req->buf, 1, fp);
            return;
        }
        if (nread < 0)
        {
            if (nread != UV_EOF)
                LOG(ERROR)<<"Read error:"<< getUVError(nread).c_str();
            uv_close((uv_handle_t *)client, NULL);
        }
        LOG(INFO)<<"2 nread: "<<nread;
        free(buf->base);
    }
    
    void UvServer::free_write_req(uv_write_t *req)
    {
        write_req_t *wr = (write_req_t *)req;
        free(wr->buf.base);
        free(wr);
    }

    void UvServer::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
    {
        //LOG(INFO)<<"1 buf->len: " <<" \n";
        buf->base = (char *)malloc(suggested_size);
        buf->len = suggested_size;
        //LOG(INFO)<<"2 buf->len: " <<" \n";
    }

    void UvServer::echo_write(uv_write_t *req, int status)
    {
        LOG(INFO)<<"1 echo_write: "<<status;
        if (status)
        {
            LOG(ERROR)<<getUVError(status).c_str();
        }
        free_write_req(req);
        LOG(INFO)<<"2 echo_write: "<<status;
    }

	std::string UvServer::getUVError(int retcode)
	{
		std::string err;
		err = uv_err_name(retcode);
		err += ":";
		err += uv_strerror(retcode);
		return std::move(err);
	}

    
}
