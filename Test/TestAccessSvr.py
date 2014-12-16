import sys
import socket
import struct

from sys import path
path.append("../ProtocolDefine")
path.append("./")

from Common import *
import LogicSvr_pb2

SVR_ADDR=("127.0.0.1", 3000)

def Test_Register():
	print "[Test_Register]"
	CMD = 2001
	REQ = LogicSvr_pb2.RegisterReq()
	RSP = LogicSvr_pb2.RegisterRsp()
	
	REQ.uid=10000
	REQ.name="TestName"
	TID=0
	ReqAndRsp(SVR_ADDR, CMD, REQ, RSP, TID)
	print "-----------------------------"

#########################################
if __name__ == '__main__':
	Test_Register()
