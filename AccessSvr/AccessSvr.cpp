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
static Packet gBinPacket;
static HttpPacket gHttpPack;

int AccessSvr::OnInit(ConfReader *config)
{
    //注册Cmd处理方法
    CALL_HANDLE_REG();

    //添加其他初始化内容

    return 0;
}

bool AccessSvr::OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    if(TCPServer::HaveHandler(cmd))
    {
        //由父类来处理
        TCPServer::OnPacket(session, cmd, packet_data, head_size, body_size, tid);
        return true;
    }

    //查找请求的session
    ClientTCPSession *client_session = dynamic_cast<ClientTCPSession*>(GetTraction(tid, true));
    if(client_session == NULL)
    {
        LOG_WARN(logger, "AccessSvr:OnPacket| not find client session for tid="<<tid);
        return true;
    }

    //重新打包
    Packet* packet = client_session->GetPacket(); assert(packet != NULL);
    ByteBuffer& send_byte_buffer = client_session->GetSendBuffer();
    int client_body_size = body_size;
    char* buffer = send_byte_buffer.RemainBuffer(client_body_size+100);
    if(buffer == NULL)
    {
        LOG_ERROR(logger, "AccessSvr:OnPacket| get remain buffer return NULL.cmd="<<cmd);
        return false;
    }
    int client_head_size = packet->SetHead(buffer, 100, client_body_size, cmd);
    if(client_head_size <= 0)
    {
        LOG_ERROR(logger, "AccessSvr:OnPacket| set head return failed.cmd="<<cmd);
        return false;
    }
    memcpy(buffer+client_head_size, packet_data+head_size, client_body_size);
    send_byte_buffer.AddSize(client_head_size+client_body_size);

    //异步发送
    if(client_session->AsyncSend() == true)
    {
        LOG_INFO(logger, "AccessSvr:OnPacket| send to client succ.cmd="<<cmd<<",tid="<<tid);
        return true;
    }
    else
    {
        LOG_ERROR(logger, "AccessSvr:OnPacket| send to client failed.cmd="<<cmd);
        return false;
    }

    return true;
}

//int AccessSvr::OnExample(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
//{
//    LOG_DEBUG(logger, "OnExample...");
//    return 0;
//}

void AccessSvr::OnListenSucc(ListenInfo &listen_info, const ConfSessionParam &session_param)
{
    //listen id=0内部使用
    int listen_id = listen_info.id;
    Packet *packet = GetInternalPacket();
    assert(packet != NULL);

    listen_info.session_mgr = &m_client_session_mgr;
    InitSessionMgr(&m_client_session_mgr, session_param);  //初始化listen session mgr
    m_client_session_mgr.Init(GetIOServer(), packet, listen_info.fd);
    LOG_INFO(logger, "AccessSvr:OnListenSucc. id="<<session_param.id<<",ip="<<session_param.ip<<":"<<session_param.port);
}

bool AccessSvr::OnClientPacket(ClientTCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
    //重新打包转发给内部server
    SessionDefault *svr_session = dynamic_cast<SessionDefault*>(ServerGroup_GetSession(cmd));
    if(svr_session == NULL)
    {
        LOG_ERROR(logger, "AccessSvr:OnClientPacket| get svr session return NULL.cmd="<<cmd);
        return false;
    }

    //生成tid
    tid = GUID::Get(GetTCPServerConf().conf_svr_index);

    //序列化
    Packet* packet = svr_session->GetPacket(); assert(packet != NULL);
    ByteBuffer& send_byte_buffer = svr_session->GetSendBuffer();
    int internal_body_size = body_size;
    char* buffer = send_byte_buffer.RemainBuffer(internal_body_size+100);
    if(buffer == NULL)
    {
        LOG_ERROR(logger, "AccessSvr:OnClientPacket| get remain buffer return NULL.cmd="<<cmd);
        return false;
    }
    int internal_head_size = packet->SetHead(buffer, 100, internal_body_size, cmd, tid);
    if(internal_head_size <= 0)
    {
        LOG_ERROR(logger, "AccessSvr:OnClientPacket| set head return failed.cmd="<<cmd);
        return false;
    }
    memcpy(buffer+internal_head_size, packet_data+head_size, internal_body_size);
    send_byte_buffer.AddSize(internal_head_size+internal_body_size);

    //异步发送
    if(svr_session->AsyncSend() == true)
    {
        LOG_INFO(logger, "AccessSvr:OnClientPacket| send to svr succ.cmd="<<cmd);
    }
    else
    {
        LOG_ERROR(logger, "AccessSvr:OnClientPacket| send to svr failed.cmd="<<cmd);
        return false;
    }

    //保存session
    int ret = SaveTraction(tid, session, 5);
    if(ret != 0)
    {
        LOG_ERROR(logger, "AccessSvr:OnClientPacket| save traction failed.tid="<<tid);
        return false;
    }
    else
    {
        LOG_DEBUG(logger, "AccessSvr:OnClientPacket| save traction succ.tid="<<tid<<",session="<<session);
    }
    return true;
}

IOStatus AccessSvr::OnClientError(ClientTCPSession *session, uint64_t now_ms)
{
    LOG_WARN(logger, "AccessSvr:OnClientError|");
    return IO_ERROR;
}
