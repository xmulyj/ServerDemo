INCLUDE=-I/usr/include/enet -I./

#标准库
LIBS=-llog4cplus -lpthread -lrt -lprotobuf
#私有库
LIBS_MY=-lenet

CFLAGS=-g -D__LOG4CPLUS__

all:bin/AccessSvr

bin/AccessSvr:Main.cpp AccessSvr.cpp AccessSvr.h AccessSvrConf.cpp AccessSvrConf.h
	g++ Main.cpp AccessSvr.cpp AccessSvrConf.cpp ${INCLUDE} ${LIBS} ${LIBS_MY} ${CFLAGS} -o $@

clean:
	rm bin/AccessSvr
