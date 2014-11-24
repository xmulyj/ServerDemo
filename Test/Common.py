import sys
import socket
import time
import struct

def PackHead(cmd, body_size):
	MAGIC_NUM="PACK"
	head=struct.pack("!4s2i",MAGIC_NUM, body_size, cmd)
	return head

def PackMsg(cmd, msg):
	size=msg.ByteSize()
	send_buf=PackHead(cmd, size)
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
	print ("REQ:cmd=%d|size=%d" %(CMD, len(send_buf)))
	recv_buf = SendAndRecvData(SVR_ADDR, send_buf)
	MAGIC_NUM_,BODY_SIZE_,CMD_=struct.unpack("!4s2i", recv_buf[0:12]);
	print("RSP:header=%s|%d|0x%X" %(MAGIC_NUM_, BODY_SIZE_, CMD_))

	#4. unserialize
	RSP.ParseFromString(recv_buf[12:])
	print ("RSP:ret=%d" %(RSP.ret))
	return RSP

