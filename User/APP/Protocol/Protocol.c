/**
  ******************************************************************************
  * @file    Protocol.c
  * @author  清华大学智能车团队
  * @version V1.0
  * @date    2019-1-7
  * @brief   与上位机之间的协议通信
  ******************************************************************************
	*/ 
#include "Protocol.h"

uint8_t FrameBuffer[FRAME_LENGTH] = {0}; //帧缓冲区
uint8_t FrameRevFlag = 0;                //接受完一帧数据标志位
uint16_t Postion_Set = 0;                //收到的位置设定值 0<=Postion_Set<=DISTANCE_PULSE
int8_t DirFlag = 0;//电机旋转方向控制，1表示向上，-1表示向下

/**
	@brief:校验和计算
 */
static uint16_t CheckSum(const uint8_t * Buf, uint8_t Num)
{
	uint16_t sum = 0;
	while(Num)
	{
		sum += *Buf;
		Buf++;
		Num--;
	}
	return sum;
}
/**
	@brief:对收到的信息进行处理
	@param: FrameNum--一帧数据的个数，DataIn--输入的原始数据，DataOut--要输出的数据
	@retval:返回收到的数据类型，当接收到的数据出错后会返回0，正确则会返回正数
 */
enum DownMsgType Msg_Process(uint8_t FrameNum,const uint8_t * DataIn, uint32_t * DataOut)
{
	uint16_t SumRev = 0;
	SumRev = *(DataIn + 7) | (*(DataIn + 8) << 8);//收到的校验和
	if(SumRev == CheckSum(DataIn, FrameNum - 2))
	{
		//通过了校验和则修改为正确的数据并返回数据类型
		*DataOut = (*(DataIn + 3) << 0)| (*(DataIn + 4) << 8)| (*(DataIn + 5) << 16)| (*(DataIn + 6) << 24);
		
		return (enum DownMsgType)*(DataIn + 2);
	}
	else//未通过校验和
	{
		*DataOut =0;
		return ERR;//帧数据校验和错误，舍弃本次数据
	}
}

int8_t SendMsgToHost(uint8_t Frame_num, uint8_t Frame_type, uint32_t Frame_data)
{
	uint8_t buf[FRAME_LENGTH] = {0};
	uint16_t Sum_send = 0;
	
	buf[0] = 0x66;
	buf[1] = Frame_num;
	buf[2] = Frame_type;
	buf[3] = (Frame_data >> 0) & 0xff;
	buf[4] = (Frame_data >> 8) & 0xff;
	buf[5] = (Frame_data >> 16) & 0xff;
	buf[6] = (Frame_data >> 24) & 0xff;
	
	Sum_send = CheckSum(buf, FRAME_LENGTH - 2);
	
	buf[7] = (Sum_send >> 0) & 0xff;
	buf[8] = (Sum_send >> 8) & 0xff;
	
	//Usart_SendArray( COM_USARTx, buf, FRAME_LENGTH);
	
	return 0;
}


int8_t SendPosToHost(uint16_t Frame_data)
{
	uint8_t buf[9] = {0};
	uint8_t Sum_send = 0;
	
	buf[0] = 0x55;
	buf[1] = 0x53;
	buf[2] = (Frame_data >> 0) & 0xff;
	buf[3] = (Frame_data >> 8) & 0xff;
	buf[4] = 0;
	buf[5] = 0;
	buf[6] = 0;
	buf[7] = 0;
	Sum_send = (uint8_t)CheckSum(buf, FRAME_LENGTH - 2);
	buf[8] = Sum_send;	
	//debug_Usart_SendArray( DEBUG_USARTx, buf, 9);
	
	return 0;
}




























