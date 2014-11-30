/*
 * AccessSvr.cpp
 *
 *  Created on: 2014-11-30 10:24
 *      Author: young
 *
 *  POWERED BY ENET
 *  E-MAIL: xmulyj@gmail.com
 */
#include "AccessSvr.h"

/* 1: 0x10000以内的CmdID内部占用,请从0x10001开始定义自己的CmdID;
 * 2: 建议将cmd ID移到统一的文件中进行定义;
 */
#define CMD_EXAMPLE_ID  0x10001

IMPL_LOGGER(AccessSvr, logger);

int AccessSvr::OnInit(ConfReader *config)
{
    //添加CmdID对应的handler
    HANDLE_CLASS(AccessSvr)
    HANDLE_CMD(CMD_EXAMPLE_ID, OnExample)
    //从这里添加其他handler
    HANDLE_CLASS_END


    //添加其他初始化内容

    return 0;
}

bool AccessSvr::OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    if(!HAS_HANDLE(cmd))
    {
        LOG_WARN(logger, "can't find handler for cmd="<<cmd<<".fd="<<session->GetFD());
        //由父类来处理
        return TCPServer::OnPacket(session, cmd, packet_data, head_size, body_size, tid);
    }

    HANLDER_PTR handler = GET_HANDLE(cmd);
    int ret = (this->*handler)(session, packet_data, head_size, body_size, tid);
    return ret==0?true:false;
}

int AccessSvr::OnExample(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    LOG_DEBUG(logger, "OnExample...");
    return 0;
}



