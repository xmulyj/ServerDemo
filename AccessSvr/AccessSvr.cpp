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
	//添加CmdID对应的handler
    HANDLE_CLASS(AccessSvr)
    //HANDLE_CMD(CMD_EXAMPLE_ID, OnExample)
    //从这里添加其他handler
    HANDLE_CLASS_END


    //添加其他初始化内容
	int ret = 0;
	if((ret=m_conf.Init(m_conf_reader)) != 0)
	{
		LOG_ERROR(logger, "AccessSvr:AccessSvrConf Init failed.ret="<<ret);
		return -1;
	}

	if((ret=InitClientSessionMgr()) != 0)
	{
		return -1;
	}

	if((ret=DoListen()) != 0)
	{
		return -1;
	}

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

int AccessSvr::InitClientSessionMgr()
{
    //初始化session mgr的参数
    if(m_conf.conf_pack_type == 1)  //bin
    {
        LOG_INFO(logger, "AccessSvr:use Binary packet.");
        m_client_session_mgr.SetPacket(&gBinPacket);                     //打包器
    }
    else if(m_conf.conf_pack_type == 2)  //http
    {
        LOG_INFO(logger, "AccessSvr:use Http packet.");
        m_client_session_mgr.SetPacket(&gHttpPack);                     //打包器
    }

	m_client_session_mgr.SetMaxSession(m_conf.conf_session_max_num);         //最大连接数
	m_client_session_mgr.SetBlock((m_conf.conf_session_block?false:true));   //阻塞模式
	m_client_session_mgr.SetRecvTimeout(m_conf.conf_session_recv_timeout);   //接收超时时间
	m_client_session_mgr.SetSendTimeout(m_conf.conf_session_send_timeout);   //发送超时时间
	m_client_session_mgr.SetMaxRecvSize(m_conf.conf_session_max_recv_size);  //最多接收缓冲区
	m_client_session_mgr.SetMaxSendSize(m_conf.conf_session_max_send_size);  //最大发送缓冲区
	m_client_session_mgr.SetUserData((void*)this);                           //用户数据
    if(m_client_session_mgr.CreateSession() == false)
    {
        LOG_ERROR(logger, "AccessSvr:init client session mgr failed");
        return -1;
    }
	return 0;
}

int AccessSvr::DoListen()
{
	if(m_conf.conf_listen <= 0)
	{
		LOG_ERROR(logger, "AccessSvr:listen num = 0");
		return -1;
	}

	int fd = TcpSocket::Listen(m_conf.conf_listen_port, m_conf.conf_listen_ip.c_str(), m_conf.conf_back_log);
	if(fd < 0)
	{
		LOG_ERROR(logger, "AccessSvr:listen on ip="<<m_conf.conf_listen_ip<<":"<<m_conf.conf_listen_port
					<<" failed.errno="<<errno<<"("<<strerror(errno)<<")");
		return -2;
	}

	m_listen_fd = fd;
	m_acceptor.Init(GetIOServer(), m_listen_fd, &m_client_session_mgr);

	LOG_INFO(logger, "AccessSvr:DoListen succ.listen on ip="<<m_conf.conf_listen_ip<<":"<<m_conf.conf_listen_port<<".fd="<<fd);
	return 0;
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

