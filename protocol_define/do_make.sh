!#/bin/bash

protoc --cpp_out=. *.proto
protoc --python_out=. *.proto

make
