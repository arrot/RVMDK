#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "stm32f4xx.h"

#define FRAME_LENGTH     9    //每帧数据的长度
enum DownMsgType 
{
	ERR = 0,
	PosSetMsg,
	PulseMsg,
};
enum UpMsgType 
{
	PosMsg = 1,
	UpPulseMsg,
	ErrMsg,
};
enum ErroCode 
{
	OverCurrentFault = 1,
	MotorDamageFault,
};

enum DownMsgType Msg_Process(uint8_t FrameNum,const uint8_t * DataIn, uint32_t * DataOut);

#endif //__PROTOCOL_H__
































