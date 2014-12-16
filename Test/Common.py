import sys
import socket
import time
import struct
from ctypes import c_longlong as longlong


def PackHead(cmd, body_size):
	MAGIC_NUM="PACK"
	head=struct.pack("!4s2i",MAGIC_NUM, body_size, cmd)
	return head

def PackMsg(cmd, msg):
	size=msg.ByteSize()
	send_buf=PackHead(cmd, size)
	send_buf += msg.SerializeToString()
	return send_buf


def PackHeadInternal(cmd, body_size, tid):
	#MAGIC_NUM="INTE"
	MAGIC_NUM="PACK"
	ret=0
	head=struct.pack("!4s2i1q1i",MAGIC_NUM,body_size,cmd,tid,ret)
	return head

def PackMsgWithTID(cmd, msg, tid):
	size=msg.ByteSize()
	send_buf=PackHeadInternal(cmd, size, tid)
	send_buf += msg.SerializeToString()
	return send_buf


def SendData(svr_addr, data):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(svr_addr)
	sock.send(data)
	print "send data finished,go to sleep 2s"
	time.sleep(2)
	sock.close()

def SendAndRecvData(svr_addr, data):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(svr_addr)
	sock.send(data)
	recv_data = sock.recv(65536)
	sock.close()
	return recv_data



def ReqAndRsp(SVR_ADDR, CMD, REQ, RSP):
	#2. serialize
	send_buf=PackMsg(CMD, REQ)
	#print repr(send_buf)

	#3. send&recv data
	print ("[REQ:cmd=%d,body_size=%d]\n%s" %(CMD, len(send_buf), str(REQ)))
	recv_buf = SendAndRecvData(SVR_ADDR, send_buf)
	MAGIC_NUM_,BODY_SIZE_,CMD_=struct.unpack("!4s2i", recv_buf[0:12]);
	#4. unserialize
	RSP.ParseFromString(recv_buf[12:])
	print("[RSP:magic=%s,cmd=%d,body_size=%d]\n%s" %(MAGIC_NUM_, CMD_, BODY_SIZE_, str(RSP)))

	return RSP


def ReqAndRspTid(SVR_ADDR, CMD, REQ, RSP, TID):
        #2. serialize
        send_buf=PackMsgWithTID(CMD, REQ, TID) 
        #print repr(send_buf)

        #3. send&recv data
        print ("[REQ:cmd=%d,body_size=%d]\n%s" %(CMD, len(send_buf), str(REQ)))
        recv_buf = SendAndRecvData(SVR_ADDR, send_buf)
        MAGIC_NUM_,BODY_SIZE_,CMD_,TID_,RET_=struct.unpack("!4s2i1q1i", recv_buf[0:24]);
        #4. unserialize
        RSP.ParseFromString(recv_buf[24:])
        print("[RSP:magic=%s,cmd=%d,body_size=%d,tid=%ld,ret=%d]\n%s" %(MAGIC_NUM_, CMD_, BODY_SIZE_, TID_, RET_, str(RSP)))

        return RSP

