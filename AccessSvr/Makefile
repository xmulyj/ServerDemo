ENET_INSTALL_PREFIX=/usr

#头文件路径
INCLUDE=-I${ENT_INSTALL_PREFIX}/include -I${ENET_INSTALL_PREFIX}/include/enet
INCLUDE+=-I../CommonDefine -I../ProtocolDefine

#标准库
LIBS=-lpthread -lrt -lprotobuf -llog4cplus ${ENET_INSTALL_PREFIX}/lib/libenet.a
LIBS+=../ProtocolDefine/libproto.a

CFLAGS=-g -D__LOG4CPLUS__

all:bin/AccessSvr

bin/AccessSvr:Main.cpp AccessSvr.cpp AccessSvr.h
	if [ ! -d "bin" ];then mkdir bin;fi
	if [ ! -d "log" ];then mkdir log;fi
	g++ Main.cpp AccessSvr.cpp ${INCLUDE} ${LIBS} ${CFLAGS} -o $@

clean:
	rm bin/AccessSvr
