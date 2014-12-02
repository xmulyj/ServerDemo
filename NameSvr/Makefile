INCLUDE=-I/usr/include/enet -I./ -I../common_define -I../protocol_define

#标准库
LIBS=-llog4cplus -lpthread -lrt -lprotobuf
#私有库
LIBS_MY=-lenet ../protocol_define/libproto.a

CFLAGS=-g -D__LOG4CPLUS__

all:bin/NameSvr

bin/NameSvr:Main.cpp NameSvr.cpp *.h
	g++ Main.cpp NameSvr.cpp ${INCLUDE} ${LIBS} ${LIBS_MY} ${CFLAGS} -o $@

clean:
	rm bin/NameSvr
