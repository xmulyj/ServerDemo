/*
 * Main.cpp
 *
 *  Created on: Nov 22, 2014
 *      Author: young
 */



#include "NameSvr.h"

int main()
{
    INIT_LOGGER("../conf/log4cplus.conf");


    ConfReader conf;
    if(conf.Init("../conf/server.conf") != 0)
        return -1;

    NameSvr server(&conf);
    if(server.Init() != 0)
        return -1;
    //Set yourself Packet class by server.SetInternalPacket() method if needed:
    //server.SetInternalPacket(other_packet);
    server.RunForever();
    return 0;
}
