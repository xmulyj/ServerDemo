/*
 * CommonSend.h
 *
 *  Created on: Nov 22, 2014
 *      Author: young
 */

#ifndef COMMONSEND_H_
#define COMMONSEND_H_

#include "enet/ENet.h"
using namespace enet;

#include <google/protobuf/message.h>
using namespace ::google::protobuf;

class CommonSend
{
public:
    int SendToSvr(SessionDefault *session, uint32_t cmd, Message *msg, uint64_t tid);
    int SendToSvr(TCPServer *server, int fd, uint32_t cmd, Message *msg, uint64_t tid);
    int SendToSvr(TCPServerRoute *server, uint32_t cmd, Message *msg, uint32_t svr_id, TCPServerRoute::RouteType route_type, uint64_t tid);

    int ReqSvr(TCPServerRoute *server, uint32_t cmd, Message *msg, uint64_t tid)  //直接按cmd路由
    {
        SessionDefault *session = server->GetSvrSession((TCPServer*)server, cmd);
        return SendToSvr(session, cmd, msg, tid);
    }
    int RspSvr(SessionDefault *session, uint32_t cmd, Message *msg, uint64_t tid)
    {
        return SendToSvr(session, cmd, msg, tid);
    }
};

inline
int CommonSend::SendToSvr(SessionDefault *session, uint32_t cmd, Message *msg, uint64_t tid)
{
    if(session == NULL)
        return -1;
    Packet* packet = session->GetPacket(); assert(packet != NULL);
    ByteBuffer& send_byte_buffer = session->GetSendBuffer();
    //序列化
    int body_size = msg->ByteSize();
    char* buffer = send_byte_buffer.RemainBuffer(body_size+100);
    if(buffer == NULL)
    {
        return -2;
    }
    int head_size = packet->SetHead(buffer, 100, body_size, cmd, &tid);
    if(head_size <= 0)
        return -3;
    msg->SerializePartialToArray(buffer+head_size, body_size);
    send_byte_buffer.AddSize(head_size+body_size);
    //异步发送
    return session->AsyncSend()?0:-4;
}

inline
int CommonSend::SendToSvr(TCPServer *server, int fd, uint32_t cmd, Message *msg, uint64_t tid)
{
    //获取会话和byte buffer
    SessionMgr *session_mgr = server->GetInternalSessionMgr();
    SessionDefault *session = dynamic_cast<SessionDefault*>(session_mgr->FindSession(fd));
    return SendToSvr(session, cmd, msg, tid);
}

inline
int CommonSend::SendToSvr(TCPServerRoute *server, uint32_t cmd, Message *msg, uint32_t svr_id, TCPServerRoute::RouteType route_type, uint64_t tid)
{
    //获取会话和byte buffer
    SessionDefault *session = server->GetSvrSession((TCPServer*)server, svr_id, route_type);
    return SendToSvr(session, cmd, msg, tid);
}

#endif /* COMMONSEND_H_ */
