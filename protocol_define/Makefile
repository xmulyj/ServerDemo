SRCS=$(wildcard *.cc)
OBJS=$(patsubst %.cc, %.o, $(SRCS))
INCLUDE=-I./
FLAGS=

LIBNAME=libproto.a

all:genpb $(OBJS) lib
genpb:
	protoc --cpp_out=. *.proto
%.o:%.cpp
	g++ -fPIC ${INCLUDE} $< ${FLAGS} -c -o $@

lib:
	ar -cr ${LIBNAME} $(OBJS)
	
clean:
	rm libproto.a *.o
