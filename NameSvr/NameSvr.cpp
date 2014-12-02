/*
 * NameSvr.cpp
 *
 *  Created on: Nov 22, 2014
 *      Author: young
 *
 *  POWERED BY ENET
 *  E-MAIL: xmulyj@gmail.com
 */

#include "NameSvr.h"
#include "NameSvr.pb.h"
#include "ErrorCode.h"

IMPL_LOGGER(NameSvr, logger);

int NameSvr::OnInit(ConfReader *config)
{
    /*
    int ret = LoadRouteConf();
    if(ret != 0)
    {
    LOG_WARN(logger, "OnInit:load route conf failed.ret="<<ret);
    return -1;
    }
    */
    
    //4. 注册cmd处理方法
    HANDLE_CLASS(NameSvr)
    HANDLE_CMD(CMD_ADD_NAME_REQ, OnAddNameReq)
    HANDLE_CMD(CMD_DEL_NAME_REQ, OnDelNameReq)
    HANDLE_CLASS_END;

    return 0;
}

bool NameSvr::OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
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

int NameSvr::OnAddNameReq(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    AddNameReq add_name_req;
    AddNameRsp add_name_rsp;

    if(add_name_req.ParseFromArray(data+head_size, body_size) == false)
    {
        LOG_WARN(logger, "OnAddNameReq:parse req failed.");
        return ERR_FAILED;
    }
    LOG_DEBUG(logger, "OnAddNameReq:tid="<<tid<<",req="<<add_name_req.ShortDebugString());
    add_name_rsp.set_ret(0);

    int ret = RspSvr((SessionDefault*)session, CMD_ADD_NAME_RSP, &add_name_rsp);
    if(ret != 0)
    {
        LOG_WARN(logger, "OnAddNameReq:send rsp failed.ret="<<ret<<",rsp="<<add_name_rsp.ShortDebugString());
    }
    else
    {
        LOG_DEBUG(logger, "OnAddNameReq:send rsp succ.ret="<<ret<<",rsp="<<add_name_rsp.ShortDebugString());
    }
    return ERR_SUCC;
}

int NameSvr::OnDelNameReq(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    DelNameReq del_name_req;
    DelNameRsp del_name_rsp;

    if(del_name_req.ParseFromArray(data+head_size, body_size) == false)
    {
        LOG_WARN(logger, "OnDelNameReq:parse req failed.");
        return ERR_FAILED;
    }
    del_name_rsp.set_ret(0);

    int ret = RspSvr((SessionDefault*)session, CMD_DEL_NAME_RSP, &del_name_rsp);
    if(ret != 0)
    {
        LOG_WARN(logger, "OnDelNameReq:send rsp failed.ret="<<ret<<",rsp="<<del_name_rsp.ShortDebugString());
    }
    else
    {
        LOG_DEBUG(logger, "OnDelNameReq:send rsp succ.ret="<<ret<<",rsp="<<del_name_rsp.ShortDebugString());
    }
    return ERR_SUCC;
}
