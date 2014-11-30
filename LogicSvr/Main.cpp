/*
 * Main.cpp
 *
 *  Created on: $DATE$
 *      Author: $USER$
 *
 *  POWERED BY ENET
 *  E-MAIL: xmulyj@gmail.com
 */

#include "LogicSvr.h"

int main()
{
    INIT_LOGGER("../conf/log4cplus.conf");
    ConfReader conf;
    if(conf.Init("../conf/TCPServer.conf") != 0)
        return -1;

    LogicSvr server(&conf);
    if(server.Init() != 0)
        return -1;

    //Set yourself Packet class by server.SetInternalPacket() method if needed:
    //server.SetInternalPacket(other_packet);

    server.RunForever();
    return 0;
}


