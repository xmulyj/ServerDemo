ENET_INSTALL_PREFIX=/usr

#头文件路径
INCLUDE=-I${ENT_INSTALL_PREFIX}/include -I${ENET_INSTALL_PREFIX}/include/enet
INCLUDE+=-I../common_define -I../protocol_define

#标准库
LIBS=-lpthread -lrt -lprotobuf -llog4cplus ${ENET_INSTALL_PREFIX}/lib/libenet.a
LIBS+=../protocol_define/libproto.a

CFLAGS=-g -D__LOG4CPLUS__

all:bin/LogicSvr

bin/LogicSvr:Main.cpp LogicSvr.cpp LogicSvr.h
	if [ ! -d "bin" ];then mkdir bin;fi
	if [ ! -d "log" ];then mkdir log;fi
	g++ Main.cpp LogicSvr.cpp ${INCLUDE} ${LIBS} ${LIBS_MY} ${CFLAGS} -o $@

clean:
	rm bin/LogicSvr
