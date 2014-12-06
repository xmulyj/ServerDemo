import sys
import socket
import struct
from ctypes import c_longlong as longlong

from sys import path
path.append("../protocol_define")
path.append("./")

from Common import *
import LogicSvr_pb2

SVR_ADDR=("127.0.0.1",3002)

def Test_Register():
	print "[Test_Register]"
	CMD = 2001
	REQ = LogicSvr_pb2.RegisterReq()
	RSP = LogicSvr_pb2.RegisterRsp()
	
	REQ.uid=10000
	REQ.name="TestName"
	TID=longlong(12345)
	ReqAndRspTid(SVR_ADDR, CMD, REQ, RSP, TID.value)
	print "-----------------------------"

#########################################
if __name__ == '__main__':
	Test_Register()
