import sys
import socket
import time
import struct
from ctypes import c_longlong as longlong

def PackHead(cmd, body_size, tid, ret):
	MAGIC_NUM="PACK"
	tid_=longlong(tid)
	head=struct.pack("!4s3i1q",MAGIC_NUM, ret, body_size, cmd, tid_.value)
	return head

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


### pb message ###
def PackMsg(cmd, msg, tid):
	size=msg.ByteSize()
	send_buf=PackHead(cmd, size, tid, 0)
	send_buf += msg.SerializeToString()
	return send_buf

def ReqAndRsp(SVR_ADDR, CMD, REQ, RSP, TID):
	#2. serialize
	send_buf=PackMsg(CMD, REQ, TID)
	#print repr(send_buf)

	#3. send&recv data
	print ("[REQ:cmd=%d,body_size=%d,tid=%d]\n%s" %(CMD, len(send_buf), TID, str(REQ)))
	recv_buf = SendAndRecvData(SVR_ADDR, send_buf)
	MAGIC_,RET_,BODY_SIZE_,CMD_,TID_ = struct.unpack("!4s3i1q", recv_buf[0:24]);
	#4. unserialize
	RSP.ParseFromString(recv_buf[24:])
	print("[RSP:magic=%s,ret=%d,body_size=%d,cmd=%d,tid=%ld]\n%s" %(MAGIC_, RET_, BODY_SIZE_, CMD_, TID_, str(RSP)))

	return RSP
