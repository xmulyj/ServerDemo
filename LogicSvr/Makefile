ENET_INSTALL_PREFIX=/usr

#头文件路径
INCLUDE=-I${ENT_INSTALL_PREFIX}/include -I${ENET_INSTALL_PREFIX}/include/enet
INCLUDE+=-I../CommonDefine -I../ProtocolDefine

#标准库
LIBS=-lpthread -lrt -lprotobuf -llog4cplus ${ENET_INSTALL_PREFIX}/lib/libenet.a
LIBS+=../ProtocolDefine/libproto.a

CFLAGS=-g -D__LOG4CPLUS__ -D__USE_PROTOCOL_BUFF__

all:bin/LogicSvr

bin/LogicSvr:Main.cpp LogicSvr.cpp LogicSvr.h
	if [ ! -d "bin" ];then mkdir bin;fi
	if [ ! -d "log" ];then mkdir log;fi
	g++ Main.cpp LogicSvr.cpp ${INCLUDE} ${LIBS} ${CFLAGS} -o $@

clean:
	rm bin/LogicSvr

rebuild:clean all
