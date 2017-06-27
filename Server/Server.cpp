#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libuv.lib")
#pragma comment(lib,"IPHLPAPI.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Userenv.lib")


int server_port = 7000;
uv_tcp_t tcpServer;
uv_loop_t* loop;

//开启一个tcp监听
int tcp_listener();
//一个新的连接到达时的回调函数
void on_connection(uv_stream_t* server, int status);


int tcp_listener() {
	struct sockaddr sockname, peername;
	int namelen;
	int r;
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", server_port, &addr);
	uv_tcp_init(loop, &tcpServer);
	uv_tcp_bind(&tcpServer, (const sockaddr*)&addr, 0);
	printf("bind!\r\n");
	uv_listen((uv_stream_t*)&tcpServer, 128, on_connection);

	//sockname，获得监听自己的ip和端口
	//memset(&sockname, -1, sizeof sockname);
	//namelen = sizeof sockname;
	//r = uv_tcp_getsockname(&tcpServer, &sockname, &namelen);
	//printf("the r is %d.\n", r);

	//没有连接时，peername是无意义的
	//namelen = sizeof peername;
	//r = uv_tcp_getpeername(&tcpServer, &peername, &namelen);
	//printf("the r is %d.\n", r);
	//if (r == -1) {
	//	printf("socket is not connected.\n");
	//}

	return 0;
}


void on_connection(uv_stream_t* server, int status) {
	struct sockaddr sockname, peername;
	int namelen;
	uv_tcp_t* handle;
	int r;

	if (status != 0) {
		printf("Connect error %s\n");
	}

	handle = (uv_tcp_t*)malloc(sizeof(*handle));

	r = uv_tcp_init(loop, handle);

	/* associate server with stream */
	handle->data = server;

	uv_accept(server, (uv_stream_t*)handle);
	printf("connect!\r\n");
	while (1)
	{
		
	}
	//这儿应该开启一个流读取数据(这个例子只是为了说明怎么获得客户端的地址和端口)
}


int main() {
	loop = uv_default_loop();
	tcp_listener();
	return uv_run(loop, UV_RUN_DEFAULT);
}










//#include <stdio.h>
//#include <stdlib.h>
//#include <uv.h>
//
//#pragma comment(lib,"ws2_32.lib")
//#pragma comment(lib,"libuv.lib")
//#pragma comment(lib,"IPHLPAPI.lib")
//#pragma comment(lib,"Psapi.lib")
//#pragma comment(lib,"Userenv.lib")
//
//uv_loop_t *loop;
//
//struct sockaddr_in addr;
//uv_tcp_t server;
//int bind_addr = 0;
//
//uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size) {
//	return uv_buf_init((char*)malloc(suggested_size), suggested_size);
//}
//
//void echo_write(uv_write_t *req, int status) {
//	if (status == -1) {
//		fprintf(stderr, "Write error!\n");
//	}
//	char *base = (char*)req->data;
//	free(base); 
//	free(req);
//}
//
//void echo_read(uv_stream_t *client, ssize_t nread, uv_buf_t buf) {
//	if (nread == -1) {
//		fprintf(stderr, "Read error!\n");
//		uv_close((uv_handle_t*)client, NULL);
//		return;
//	}
//
//	uv_write_t *write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
//	write_req->data = (void*)buf.base;
//	buf.len = nread;
//	uv_write(write_req, client, &buf, 1, echo_write);
//}
//
//
//void on_new_connection(uv_stream_t *server, int status) {
//	if (status == -1) {
//		return;
//	}
//
//	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
//	uv_tcp_init(loop, client);
//	if (uv_accept(server, (uv_stream_t*)client) == 0) {
//		printf("连接成功!\r\n");
//		uv_read_start((uv_stream_t*)client, (uv_alloc_cb)alloc_buffer, (uv_read_cb)echo_read);
//	}
//	else {
//		uv_close((uv_handle_t*)client, NULL);
//	}
//}
//
//int main() {
//	loop = uv_default_loop();
//	
//	uv_tcp_init(loop, &server);
//
//	uv_ip4_addr("127.0.0.1", 7000, &addr);
//	uv_tcp_bind(&server, (const struct sockaddr*)&addr,0);
//	int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
//	if (r) {
//		fprintf(stderr, "Listen error!\n");
//		Sleep(1000);
//		return 1;
//	}
//	return uv_run(loop, UV_RUN_DEFAULT);
//}