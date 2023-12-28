#include "net.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

static err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err);

static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *tcp_recv_pbuf, err_t err);

/***
 * 函数名称 : tcp_server_init();
 * 函数描述 : TCP服务器初始化;
 * 传递值   : 无;
 * 返回值   : 无;
 **/
void tcp_server_init(void) {
    struct tcp_pcb *tcp_server_pcb;

    /* 为tcp服务器分配一个tcp_pcb结构体 */
    tcp_server_pcb = tcp_new();

    /* 绑定本地端号和IP地址 */
    tcp_bind(tcp_server_pcb, IP_ADDR_ANY, 80);

    /* 监听之前创建的结构体tcp_server_pcb */
    tcp_server_pcb = tcp_listen(tcp_server_pcb);

    /* 初始化结构体接收回调函数 */
    tcp_accept(tcp_server_pcb, tcp_server_accept);
}

/***
 * 函数名称 : tcp_server_accept();
 * 函数描述 : lwip数据接收回调函数，包含对tcp连接的确认，接收回调函数的配置;
 * 传递值   : *arg, *pcb, err ;
 * 返回值   : ERR_OK 无错误;
 **/
static err_t tcp_server_accept(void *arg, struct tcp_pcb *pcb, err_t err) {
    /* 确认监听与连接 */
    tcp_arg(pcb, mem_calloc(sizeof(struct name), 1));

    /* 发送一个建立连接的字符串 */
    tcp_write(pcb, "Received TCP data.\r\n", strlen("Received TCP data.\r\n"), 1);

    /* 配置接收回调函数 */
    tcp_recv(pcb, tcp_server_recv);

    return ERR_OK;
}

/***
 * 函数名称 : tcp_server_recv();
 * 函数描述 : 接受到数据后，将数据拷贝转发出去;
 * 传递值   : *arg, *pcb, *tcp_recv_pbuf, err;
 * 返回值   : ERR_ARG 非法逻辑，ERR_OK无错误;
 **/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *tcp_recv_pbuf, err_t err) {
    struct name *name = (struct name *) arg;

    if (tcp_recv_pbuf != NULL) {
        if (!name) {
            pbuf_free(tcp_recv_pbuf);
            return ERR_ARG;
        }
        /* 将接收到的数据再转发出去 */
        const char *tcp_reply = "The TCP data: ";
        tcp_write(pcb, tcp_reply, strlen(tcp_reply), 1);
        tcp_write(pcb, tcp_recv_pbuf->payload, tcp_recv_pbuf->len, 1);
        pbuf_free(tcp_recv_pbuf);
    } else if (err == ERR_OK) {
        /* 释放内存 */
        mem_free(name);
        return tcp_close(pcb);
    }
    return ERR_OK;
}


// ------------------------------udp----------------------------------
static void
udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *udp_recv_pbuf, const ip_addr_t *addr, u16_t port);

/***
 * 函数名称 : udp_server_init();
 * 函数描述 : UDP服务器初始化;
 * 传递值   : 无;
 * 返回值   : 无;
 **/
void udp_server_init(void) {
    struct udp_pcb *udp_server_pcb;

    /* 为udp服务器分配一个udp_pcb结构体 */
    udp_server_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);

    /* 绑定本地端号和IP地址 */
    udp_bind(udp_server_pcb, IP_ADDR_ANY, 81);

    /* 初始化结构体接收回调函数 */
    udp_recv(udp_server_pcb, udp_server_recv, (void *) "Received UDP data.");
}

/***
 * 函数名称 : udp_server_recv();
 * 函数描述 : 接受到数据后，将数据拷贝转发出去;
 * 传递值   : *arg, *pcb, *udp_recv_pbuf, *addr, port;
 * 返回值   : 无;
 **/
static void
udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *udp_recv_pbuf, const ip_addr_t *addr, u16_t port) {
    const char *udp_reply = "The UDP data: ";
    struct pbuf *udp_send_pbuf = pbuf_alloc(PBUF_TRANSPORT, strlen(udp_reply) + 1, PBUF_RAM);
    if (!udp_send_pbuf) {
        pbuf_free(udp_recv_pbuf);
        return;
    }
    memset(udp_send_pbuf->payload, 0, udp_send_pbuf->len);
    memcpy(udp_send_pbuf->payload, udp_reply, strlen(udp_reply));

    /* 发送 */
    udp_sendto(pcb, udp_send_pbuf, addr, port);
    udp_sendto(pcb, udp_recv_pbuf, addr, port);

    /* 释放buf */
    pbuf_free(udp_recv_pbuf);
    pbuf_free(udp_send_pbuf);
}
