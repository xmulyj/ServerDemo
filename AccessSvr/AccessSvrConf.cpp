/*
 * AccessSvrConf.cpp
 *
 *  Created on: Dec 7, 2014
 *      Author: young
 */

#include "AccessSvrConf.h"

int AccessSvrConf::Init(ConfReader *conf)
{
    int temp = -1;
    string str_temp;
    conf->GetValue("ACCESS_SERVER", "pack_type", str_temp, "");
    if(str_temp == "")
    {
        return -1;
    }
    else if(str_temp == "bin")
    {
        conf_pack_type = 1;
    }
    else if(str_temp == "http")
    {
        conf_pack_type = 2;
    }
    else
    {
        return -2;
    }

    //idle_ms
    conf->GetValue("ACCESS_SERVER", "idle_ms", conf_idle_ms, 3000);

    //会话参数
    conf->GetValue("ACCESS_SERVER", "session_max_num", conf_session_max_num, 100);
    conf->GetValue("ACCESS_SERVER", "session_block", conf_session_block, 0);
    conf->GetValue("ACCESS_SERVER", "session_recv_timeout", conf_session_recv_timeout, 3);
    conf->GetValue("ACCESS_SERVER", "session_send_timeout", conf_session_send_timeout, 3);
    conf->GetValue("ACCESS_SERVER", "session_max_recv_size", conf_session_max_recv_size, 65536);
    conf->GetValue("ACCESS_SERVER", "session_max_send_size", conf_session_max_send_size, 65536);

    //listen
    conf->GetValue("ACCESS_SERVER", "listen_num", conf_listen, 0);
    if(conf_listen > 0)
    {
        conf->GetValue("TCP_LISTEN_0", "ip", conf_listen_ip);
        if(conf_listen_ip == "")
        {
            return -3;
        }
        conf->GetValue("TCP_LISTEN_0", "port", conf_listen_port, 0);
        if(conf_listen_port == 0)
        {
            return -3;
        }
        conf->GetValue("TCP_LISTEN_0", "back_log", conf_back_log, 128);
    }

    return 0;
}
