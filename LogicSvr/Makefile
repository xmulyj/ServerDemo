INCLUDE=-I/usr/include/enet -I./

#标准库
LIBS=-llog4cplus -lpthread -lrt -lprotobuf
#私有库
LIBS_MY=-lenet

CFLAGS=-g -D__LOG4CPLUS__

all:bin/LogicSvr

bin/LogicSvr:Main.cpp LogicSvr.cpp LogicSvr.h
	g++ Main.cpp LogicSvr.cpp ${INCLUDE} ${LIBS} ${LIBS_MY} ${CFLAGS} -o $@

clean:
	rm bin/LogicSvr
