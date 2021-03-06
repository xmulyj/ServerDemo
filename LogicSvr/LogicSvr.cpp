/*
 * LogicSvr.cpp
 *
 *  Created on: 2014-11-30 10:22
 *      Author: young
 *
 *  POWERED BY ENET
 *  E-MAIL: xmulyj@gmail.com
 */
#include "LogicSvr.h"
#include "LogicSvr.pb.h"
#include "NameSvr.pb.h"
#include "ErrorCode.h"

/* 1: 0x10000以内的CmdID内部占用,请从0x10001开始定义自己的CmdID;
 * 2: 建议将cmd ID移到统一的文件中进行定义;
 */
#define CMD_EXAMPLE_ID  0x10001

IMPL_LOGGER(LogicSvr, logger);

int LogicSvr::OnInit(ConfReader *config)
{
    //注册Cmd处理方法
    CALL_HANDLE_REG();

    //添加其他初始化内容
    SetServer(this);

    return 0;
}

bool LogicSvr::OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    if(!HAS_HANDLE(cmd))
    {
        //LOG_DEBUG(logger, "can't find handler for cmd="<<cmd<<".fd="<<session->GetFD());
        //由父类来处理
        return TCPServer::OnPacket(session, cmd, packet_data, head_size, body_size, tid);
    }

    HANLDER_PTR handler = GET_HANDLE(cmd);
    int ret = (this->*handler)(session, packet_data, head_size, body_size, tid);
    return ret==0?true:false;
}

int LogicSvr::OnRegister(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    RegisterReq register_req;
    if(register_req.ParseFromArray(data+head_size, body_size) == false)
    {
        LOG_WARN(logger, "OnRegister:parse req failed.");
        return ERR_FAILED;
    }
    LOG_INFO(logger, "OnRegister:tid="<<tid<<",req="<<register_req.ShortDebugString());

    AddNameReq add_name_req;
    add_name_req.set_uid(register_req.uid());
    add_name_req.set_name(register_req.name());

    int ret = ReqSvr(CMD_ADD_NAME_REQ, &add_name_req, 0, tid, session);
    if(ret != 0)
    {
        LOG_ERROR(logger, "OnRegister:send AddNameReq failed.ret="<<ret<<",tid="<<tid<<",req="<<add_name_req.ShortDebugString());
        return -1;
    }
    else
    {
    	LOG_DEBUG(logger, "OnRegister:send AddNameReq succ.ret="<<ret<<",tid="<<tid<<",req="<<add_name_req.ShortDebugString());
    }

    return 0;
}

int LogicSvr::OnAddNameRsp(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    AddNameRsp add_name_rsp;
    if(add_name_rsp.ParseFromArray(data+head_size, body_size) == false)
    {
        LOG_WARN(logger, "OnAddNameRsp:parse req failed.");
        return ERR_FAILED;
    }
    LOG_INFO(logger, "OnAddNameRsp:tid="<<tid<<",rsp="<<add_name_rsp.ShortDebugString());

    //回包
    RegisterRsp register_rsp;
    register_rsp.set_ret(add_name_rsp.ret());
    int ret = RspSvr(CMD_REGISTER_RSP, &register_rsp, 0, tid);
    if(ret != 0)
    {
        LOG_WARN(logger, "OnAddNameRsp:send RegisterRsp failed.ret="<<ret<<",tid="<<tid<<",rsp="<<register_rsp.ShortDebugString());
    }
    else
    {
        LOG_INFO(logger, "OnAddNameRsp:send RegisterRsp succ.ret="<<ret<<",tid="<<tid<<",rsp="<<register_rsp.ShortDebugString());
    }

    return 0;
}


