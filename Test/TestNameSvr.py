import sys
import socket
import struct

from sys import path
path.append("../ProtocolDefine")
path.append("./")

from Common import *
import NameSvr_pb2

SVR_ADDR=("127.0.0.1",3003)

def Test_AddName():
	print "[Test_AddName]"
	CMD = 0x00020001
	REQ = NameSvr_pb2.AddNameReq()
	RSP = NameSvr_pb2.AddNameRsp()
	
	REQ.uid=10000
	REQ.name="TestName"
	TID=12345
	ReqAndRsp(SVR_ADDR, CMD, REQ, RSP, TID)
	print "-----------------------------"

def Test_DelName():
	print "[Test_DelName]"
	CMD = 0x00020003
	REQ = NameSvr_pb2.DelNameReq()                                                
	RSP = NameSvr_pb2.DelNameRsp()

	REQ.uid=10000
	REQ.name="TestName"
	TID=12346
	ReqAndRsp(SVR_ADDR, CMD, REQ, RSP, TID)
	print "-----------------------------"

#########################################
if __name__ == '__main__':
	Test_AddName()
	Test_DelName()
