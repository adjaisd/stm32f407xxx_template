#ifndef __NET_H__
#define __NET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

// 实现tcp收发数据，参考: https://blog.csdn.net/q361750389/article/details/53022911

struct name {
    int length;
    char bytes[BUF_MAX_LEN];
};

void tcp_server_init(void);

#ifdef __cplusplus
};
#endif

#endif /* __NET_H__ */
