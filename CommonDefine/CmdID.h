/*
 * CmdID.h
 *
 *  Created on: Nov 21, 2014
 *      Author: young
 */

#ifndef CMDID_H_
#define CMDID_H_

//SvrID=0, AccessSvr


//SvrID=1, LogicSvr
#define CMD_REGISTER_REQ     0x00010001  //注册
#define CMD_REGISTER_RSP     0x00010002


//SvrID=2, NameSvr
#define CMD_ADD_NAME_REQ     0x00020001  //添加名字
#define CMD_ADD_NAME_RSP     0x00020002
#define CMD_DEL_NAME_REQ     0x00020003  //删除名字
#define CMD_DEL_NAME_RSP     0x00020004



#endif /* CMDID_H_ */
