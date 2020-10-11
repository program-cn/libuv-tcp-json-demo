#include "uv-cpp.h"


/*************************************************************************************************/

void uv_release_cpp_cb_ptr(uv_handle_t * handle) {
  if (!handle->data)
    return;

  uv_function_wrapper_base *cb = static_cast<uv_function_wrapper_base *>(handle->data);
  delete cb;
}

/*************************************************************************************************/

void uv_listen_x_on_connection_cb(uv_stream_t* server, int status) {
  auto cb = *static_cast<uv_function_wrapper<uv_connection_cb2> *>(server->data);
  cb(server, status);
}

int uv_listen_x(uv_stream_t* stream, int backlog, const uv_connection_cb2& cb) {
  stream->data = new uv_function_wrapper<uv_connection_cb2>(cb);
  return ::uv_listen(stream, backlog, uv_listen_x_on_connection_cb);
}

/*************************************************************************************************/
void uv_echo_read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
  auto cb = *static_cast<uv_function_wrapper<uv_read_cb3> *>(client->data);
  cb(client, nread, buf);
}

int uv_read_start_x(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb3 cb)
{
  stream->data = new uv_function_wrapper<uv_read_cb3>(cb);
  return ::uv_read_start(stream, alloc_cb, uv_echo_read_cb);
}

/*************************************************************************************************/
void uv_echo_write_cb(uv_write_t *req, int status) {
  auto cb = *static_cast<uv_function_wrapper<uv_write_cb2> *>(req->data);
  cb(req, status);
}

int uv_write_x(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb2 cb)
{
  req->data = new uv_function_wrapper<uv_write_cb2>(cb);
  return ::uv_write(req, handle, bufs, nbufs, uv_echo_write_cb);
}
