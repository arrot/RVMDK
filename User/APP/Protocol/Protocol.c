/**
  ******************************************************************************
  * @file    Protocol.c
  * @author  �廪��ѧ���ܳ��Ŷ�
  * @version V1.0
  * @date    2019-1-7
  * @brief   ����λ��֮���Э��ͨ��
  ******************************************************************************
	*/ 
#include "Protocol.h"

uint8_t FrameBuffer[FRAME_LENGTH] = {0}; //֡������
uint8_t FrameRevFlag = 0;                //������һ֡���ݱ�־λ
uint16_t Postion_Set = 0;                //�յ���λ���趨ֵ 0<=Postion_Set<=DISTANCE_PULSE
int8_t DirFlag = 0;//�����ת������ƣ�1��ʾ���ϣ�-1��ʾ����

/**
	@brief:У��ͼ���
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
	@brief:���յ�����Ϣ���д���
	@param: FrameNum--һ֡���ݵĸ�����DataIn--�����ԭʼ���ݣ�DataOut--Ҫ���������
	@retval:�����յ����������ͣ������յ������ݳ����᷵��0����ȷ��᷵������
 */
enum DownMsgType Msg_Process(uint8_t FrameNum,const uint8_t * DataIn, uint32_t * DataOut)
{
	uint16_t SumRev = 0;
	SumRev = *(DataIn + 7) | (*(DataIn + 8) << 8);//�յ���У���
	if(SumRev == CheckSum(DataIn, FrameNum - 2))
	{
		//ͨ����У������޸�Ϊ��ȷ�����ݲ�������������
		*DataOut = (*(DataIn + 3) << 0)| (*(DataIn + 4) << 8)| (*(DataIn + 5) << 16)| (*(DataIn + 6) << 24);
		
		return (enum DownMsgType)*(DataIn + 2);
	}
	else//δͨ��У���
	{
		*DataOut =0;
		return ERR;//֡����У��ʹ���������������
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




























