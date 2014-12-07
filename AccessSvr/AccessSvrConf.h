/*
 * AccessSvrConf.h
 *
 *  Created on: Dec 7, 2014
 *      Author: young
 */

#ifndef ACCESSSVR_ACCESSSVRCONF_H_
#define ACCESSSVR_ACCESSSVRCONF_H_

#include "enet/ENet.h"
using namespace enet;

class AccessSvrConf
{
public:
	int Init(ConfReader *conf);

    int conf_pack_type;
    int conf_idle_ms;

    //session参数(listen和connect共用)
    int conf_session_max_num;       //最大会话个数(默认100)
    int conf_session_block;         //是否阻塞(默认0,非阻塞)
    int conf_session_recv_timeout;  //接收超时时间(默认3s)
    int conf_session_send_timeout;  //发送超时时间(默认3s)
    int conf_session_max_recv_size; //最大接收缓冲区(默认64K)
    int conf_session_max_send_size; //最大接收缓冲区(默认64K)

    //listen
    int conf_listen;
    string conf_listen_ip;
    int conf_listen_port;
    int conf_back_log;
};

#endif /* ACCESSSVR_ACCESSSVRCONF_H_ */
