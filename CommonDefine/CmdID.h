/*
 * CmdID.h
 *
 *  Created on: Nov 21, 2014
 *      Author: young
 */

#ifndef CMDID_H_
#define CMDID_H_

//SvrID=1, AccessSvr


//SvrID=2, LogicSvr
#define CMD_REGISTER_REQ     2001  //注册
#define CMD_REGISTER_RSP     2002


//SvrID=3, NameSvr
#define CMD_ADD_NAME_REQ     3001  //添加名字
#define CMD_ADD_NAME_RSP     3002
#define CMD_DEL_NAME_REQ     3003  //删除名字
#define CMD_DEL_NAME_RSP     3004



#endif /* CMDID_H_ */
