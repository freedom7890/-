
#include "IR.h"
#include "main.h"
#include "tim.h"

#define CHECK_TIME_LOGIC_0() (CaptureBuf >= IR_us_LOGIC_0 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_0 + IR_DEVIATION)
#define CHECK_TIME_LOGIC_1() (CaptureBuf >= IR_us_LOGIC_1 - IR_DEVIATION && CaptureBuf <= IR_us_LOGIC_1 + IR_DEVIATION)
#define CHECK_TIME_START() (CaptureBuf >= IR_us_START - IR_DEVIATION && CaptureBuf <= IR_us_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_START() (CaptureBuf >= IR_us_REPEAT_START - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_START + IR_DEVIATION)
#define CHECK_TIME_REPEAT_END() (CaptureBuf >= IR_us_REPEAT_END - IR_DEVIATION && CaptureBuf <= IR_us_REPEAT_END + IR_DEVIATION)

uint16_t CaptureBuf = 0; //��Ų��񻺴�
uint8_t CaptureFlag = 1; //�����־λ
uint8_t IRData[4] = {0};//�������
uint8_t IRFlag = 0;//�������״̬
uint8_t IRReceiveFlag = 0;//��ɽ��ձ�־

void IR_Init()
{
	__HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING); //��Ϊ������
	HAL_TIM_IC_Start_IT(&IR_TIM, IR_TIM_CHANNE);							 //�������벶��
}
void IR_CaptureCallback()
{
	if (CaptureFlag)
	{
		__HAL_TIM_SET_COUNTER(&IR_TIM, 0);
		__HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_ICPOLARITY_FALLING); //����Ϊ�½��ز���
		CaptureFlag = 0;																															//״̬��־λ��0
	}
	else
	{
		CaptureBuf = HAL_TIM_ReadCapturedValue(&IR_TIM, IR_TIM_CHANNE);												 //��ȡ��ǰ�Ĳ���ֵ.
		__HAL_TIM_SET_CAPTUREPOLARITY(&IR_TIM, IR_TIM_CHANNE, TIM_INPUTCHANNELPOLARITY_RISING); //��Ϊ������
		CaptureFlag = 1;																																			 //״̬��־λ��1
		if (CHECK_TIME_START())	 //���յ����ݿ�ʼλ
		{
			IRFlag = 1;
			for (uint8_t i = 0; i < 4; i++)
			{
				IRData[i] = 0;//�������
			}
			return; //�˳����ȴ���������
		}
		else if (CHECK_TIME_REPEAT_START()) //���յ�ѭ����ʼλ
		{
			IRFlag = 40; //�ظ�����
			return;			 //�˳����ȴ�ѭ���ڶ�λ
		}

		if (IRFlag && IRFlag <= 32)//��������
		{
			if (CHECK_TIME_LOGIC_1()) //�ж�Ϊ�߼�1
			{
				IRData[(IRFlag - 1) / 8] |= (1 << (7 - (IRFlag - 1) % 8));
			}
			else if (!CHECK_TIME_LOGIC_0()) //�жϲ����߼�0
			{
				IRFlag = 0; //ֱ���˳�����
				return;
			}
			IRFlag++;
			if (IRFlag == 33)//����32�����������
			{
				if((IR_CHECK_ADDRESS && IRData[0] != (uint8_t)~IRData[1]) || (IR_CHECK_COMMAND && IRData[2] != (uint8_t)~IRData[3]))//У��
				{
					return;
				}
				IRReceiveFlag = 1;
				return;
			}
		}
		if (IRFlag == 40)
		{
			if (CHECK_TIME_REPEAT_END())
			{
				IRReceiveFlag = IR_CHECK_REPEAT;
			}
		}
	}
}
uint8_t IR_Scanf(uint32_t* data)
{
	if(IRReceiveFlag)IRReceiveFlag=0;
	else return 0;
	*data=0;
	for(uint8_t i=0;i<4;i++)
	{
		*data|=IRData[i]<<8*(3-i);
	}
	return 1;
}



