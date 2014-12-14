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
//需要路由功能的话请设置为1
#if 1    //路由规则
	string conf_route_file;
	config->GetValue("TCP_SERVER", "route_conf", conf_route_file);
	if(conf_route_file == "")
	{
		LOG_ERROR(logger, "OnInit:[TCP_SERVER]route_conf not set");
		return -1;
	}
	int ret = LoadRouteConf(conf_route_file.c_str());
	if(ret != 0)
	{
		LOG_ERROR(logger, "OnInit:LoadRouteConf failed.ret="<<ret);
		return -1;
	}
#endif

	//添加CmdID对应的handler
    HANDLE_CLASS(AccessSvr)
    //HANDLE_CMD(CMD_EXAMPLE_ID, OnExample)
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

//int AccessSvr::OnExample(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid)
//{
//    LOG_DEBUG(logger, "OnExample...");
//    return 0;
//}

void AccessSvr::OnListenSucc(ListenInfo &listen_info)
{
    //listen id=0内部使用
    int listen_id = listen_info.id;
    TCPServerConf &tcp_conf = GetTCPConf();
    const ConfSessionParam *client_listen_param = tcp_conf.GetListenParam(listen_id);
    Packet *packet = GetInternalPacket();
    assert(client_listen_param!=NULL && packet!=NULL);

    listen_info.session_mgr = &m_client_session_mgr;
    InitSessionMgr(&m_client_session_mgr, *client_listen_param);  //初始化listen session mgr
    m_client_session_mgr.Init(GetIOServer(), packet, listen_info.fd);
    LOG_INFO(logger, "AccessSvr:OnListenSucc. id="<<client_listen_param->id<<",ip="<<client_listen_param->ip<<":"<<client_listen_param->port);
}

bool AccessSvr::OnClientPacket(ClientTCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
	//重新打包转发给内部server
	tid = GUID::Get(GetTCPServerConf()->conf_svr_index);
	SessionDefault *svr_session = GetSvrSession(this, cmd);
	if(svr_session == NULL)
	{
		LOG_ERROR(logger, "AccessSvr:OnClientPacket get svr session return NULL.cmd="<<cmd);
		return false;
	}

    Packet* packet = session->GetPacket(); assert(packet != NULL);
    ByteBuffer& send_byte_buffer = svr_session->GetSendBuffer();
    //序列化
    int internal_body_size = body_size;
    char* buffer = send_byte_buffer.RemainBuffer(internal_body_size+100);
    if(buffer == NULL)
    {
    	LOG_ERROR(logger, "AccessSvr:OnClientPacket get remain buffer return NULL.cmd="<<cmd);
        return false;
    }
    int internal_head_size = packet->SetHead(buffer, 100, internal_body_size, cmd, &tid);
    if(internal_head_size <= 0)
    {
    	LOG_ERROR(logger, "AccessSvr:OnClientPacket set head return failed.cmd="<<cmd);
    	return false;
    }
    memcpy(buffer+internal_head_size, packet_data+head_size, internal_body_size);
    send_byte_buffer.AddSize(internal_head_size+internal_body_size);
    //异步发送
    if(svr_session->AsyncSend() == true)
    {
    	LOG_INFO(logger, "AccessSvr:OnClientPacket send to svr succ.cmd="<<cmd);
    	return true;
    }
    else
    {
    	LOG_ERROR(logger, "AccessSvr:OnClientPacket send to svr failed.cmd="<<cmd);
    	return false;
    }
}

IOStatus AccessSvr::OnClientError(ClientTCPSession *session, uint64_t now_ms)
{
	return IO_ERROR;
}
