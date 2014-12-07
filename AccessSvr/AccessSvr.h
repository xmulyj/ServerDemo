/*
 * AccessSvr.h
 *
 *  Created on: 2014-11-30 10:24
 *      Author: young
 *
 *  POWERED BY ENET
 *  E-MAIL: xmulyj@gmail.com
 */

#ifndef _ACCESSSVR_H_
#define _ACCESSSVR_H_

#include "enet/ENet.h"
using namespace enet;

#include <map>
using std::map;

#include "AccessSvrConf.h"

class ClientTCPSession:public SessionDefault
{
protected:
    // @override
    bool OnPacket(uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid);
};

class AccessSvr:public TCPServer, public TCPServerRoute
{
friend class ClientTCPSession;
public:
    AccessSvr(ConfReader *tcp_conf, ConfReader *access_conf):TCPServer(tcp_conf)
		,m_conf_reader(access_conf)
		,m_listen_fd(-1)
	{
	}

protected:
    int OnInit(ConfReader *config);
    bool OnPacket(TCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid);

    //转发客户端请求
    bool OnClientPacket(ClientTCPSession *session, uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid);
private:
    // CmdID的处理方法类型定义
    //   session: 请求的会话信息(即代表一个连接)
    //      data: 完整的请求数据,包含协议头和协议体;协议头主要用来分包的,协议体则包含应用层自定义的协议数据(参考Packet类的定义)
    // head_size: 协议头长度
    // body_size: 协议头体长度
    //       tid: 事务id(代表每个请求的唯一id号)
    // handler类型定义
    typedef int HANLDER_TYPE(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid);
    // handler指针类型定义
    typedef int (AccessSvr::*HANLDER_PTR)(TCPSession *session, const char *data, uint32_t head_size, uint32_t body_size, uint64_t tid);
    DEF_HANDLE(uint32_t/*cmd ID*/, HANLDER_PTR);
    // 声明handler方法
    //HANLDER_TYPE OnExample;
private:
    ConfReader *m_conf_reader;
    AccessSvrConf m_conf;
    int m_listen_fd;
    SessionAcceptor m_acceptor;
    SessionMgrDefault<ClientTCPSession> m_client_session_mgr;
    int InitClientSessionMgr();
    int DoListen();
private:
    DECL_LOGGER(logger);
};

//TCPSession
inline
bool ClientTCPSession::OnPacket(uint32_t cmd, const char *packet_data, uint32_t head_size, uint32_t body_size, uint64_t tid)
{
	AccessSvr* tcp_server = (AccessSvr*)GetUserData();
    assert(tcp_server != NULL);
    return tcp_server->OnClientPacket(this, cmd, packet_data, head_size, body_size, tid);
}

#endif /* _ACCESSSVR_H_ */

