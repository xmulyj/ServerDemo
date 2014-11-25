/*
 * LogicSvr.cpp
 *
 *  Created on: Nov 22, 2014
 *      Author: young
 */

#include "LogicSvr.h"
#include "Name.pb.h"
#include "ErrorCode.h"

IMPL_LOGGER(LogicSvr, logger);

int LogicSvr::OnInit(ConfReader *config)
{
    //4. 注册cmd处理方法
    HANDLE_CLASS(LogicSvr)
    HANDLE_CLASS_END;

    return 0;
}

bool LogicSvr::OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    if(!HAS_HANDLE(cmd))
    {
        //LOG_ERROR(logger, "can't find handler for cmd="<<cmd<<".fd="<<fd);
        
        //return false;
        //由父类来处理
        return TCPServer::OnPacket(session, cmd, packet_data, head_size, body_size, tid);
    }

    HANLDER_PTR handler = GET_HANDLE(cmd);
    int ret = (this->*handler)(session, packet_data, head_size, body_size, tid);
    return ret==0?true:false;
}


