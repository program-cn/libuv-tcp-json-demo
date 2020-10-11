#include <uv.h>
#include <functional>

typedef std::function<void (uv_stream_t* server, int status)> uv_connection_cb2;
typedef std::function<void (uv_stream_t* client, ssize_t nread, const uv_buf_t *buf)> uv_read_cb3;
typedef std::function<void (uv_write_t* req, int status)> uv_write_cb2;

 /*************************************************************************************************/

class uv_function_wrapper_base {
public:
  virtual ~uv_function_wrapper_base(){};
};

template <typename F>
class uv_function_wrapper : public uv_function_wrapper_base {
public: 
  uv_function_wrapper(const F& cb) : m_cb(cb) { }

  template<typename... ArgTypes>
  void operator()(ArgTypes ...args) {
    m_cb(args...);
  }

  F m_cb;
};



int uv_listen_x(uv_stream_t* stream, int backlog, const uv_connection_cb2& cb);
int uv_read_start_x(uv_stream_t*, uv_alloc_cb alloc_cb, uv_read_cb3 read_cb);
int uv_write_x(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb2 cb);

void uv_release_cpp_cb_ptr(uv_handle_t * handle);

