#!/bin/bash

echo "build Protocol"
cd ProtocolDefine
./do_make.sh
cd ../

echo "make server"
make

