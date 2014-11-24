/*
 * NameSvr.h
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

class NameSvr:public TCPServer, public TCPServerRoute, public CommonSend
{
public:
    NameSvr(ConfReader *conf):TCPServer(conf){}
protected:
    int OnInit(ConfReader *config);
    bool OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size);
private:
    //handler类型定义
    typedef int HANDLER_TYPE(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size);
    typedef int (NameSvr::*HANLDER_PTR)(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size);
    DEF_HANDLE(uint32_t, HANLDER_PTR);
    //handler 方法
    HANDLER_TYPE OnAddNameReq;    //添加名称
    HANDLER_TYPE OnDelNameReq;    //删除名称
private:
    DECL_LOGGER(logger);
};

#endif /* NAMESVR_H_ */
