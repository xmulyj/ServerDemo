/*
 * LogicSvr.h
 *
 *  Created on: Nov 22, 2014
 *      Author: young
 */

#ifndef NAMESVR_H_
#define NAMESVR_H_

#include "enet/ENet.h"
using namespace enet;

#include "CmdID.h"
#include "CommonSend.h"

class LogicSvr:public TCPServer, public TCPServerRoute, public CommonSend
{
public:
    LogicSvr(ConfReader *conf):TCPServer(conf){}
protected:
    int OnInit(ConfReader *config);
    bool OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid);
private:
    //handler类型定义
    typedef int HANDLER_TYPE(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid);
    typedef int (LogicSvr::*HANLDER_PTR)(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid);
    DEF_HANDLE(uint32_t, HANLDER_PTR);
    //handler 方法
private:
    DECL_LOGGER(logger);
};

#endif /* NAMESVR_H_ */
