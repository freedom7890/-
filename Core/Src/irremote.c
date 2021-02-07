#include "irremote.h"
#include "tim.h"
//#include <stdio.h>
//#include "motor.h"

uint32_t receive_Code = 0;

void delay_us(uint16_t us)
	{
  uint16_t differ = 0xffff-us-5;
  //�趨TIM7��������ʼֵ                                
  __HAL_TIM_SET_COUNTER(&htim2,differ);        
  HAL_TIM_Base_Start(&htim2);    //������ʱ��        
        
  while(differ < 0xffff-5){    //�ж�
    //��ѯ�������ļ���ֵ
    differ = __HAL_TIM_GET_COUNTER(&htim2);                
  }
  HAL_TIM_Base_Stop(&htim2);
}

/*�ߵ�ƽ����ʱ�亯��*/
uint8_t IR_HighLevelPeriod(void){        
  uint8_t t=0;
  while(HAL_GPIO_ReadPin(ir_GPIO_Port,ir_Pin)==1){  //�ߵ�ƽ
    t++;
    delay_us(20);
    if(t>=250) return t;    //��ʱ���
  }
  return t;
}
/*����ң���ⲿ�жϻص�����*/
//���жϻص���������ɺ������
/*

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{                
    if(GPIO_Pin == IR_Pin)
	{ 
		
		uint8_t receive_Flag=0;
    uint8_t Tim=0,Ok=0,Data,Num=0;
  while(1){
    if(HAL_GPIO_ReadPin(IR_GPIO_Port,IR_Pin)==1){
      //��ô˴θߵ�ƽʱ��
      Tim = IR_HighLevelPeriod();        

      if(Tim>=250) 
        break;                //���õ��ź�
      if(Tim>=200 && Tim<250)
        Ok=1;                //�յ���ʼ�ź�
      else if(Tim>=60 && Tim<90)
        Data=1;                //�յ�����1
      else if(Tim>=10 && Tim<50)
        Data=0;                //�յ�����0

      if(Ok==1){
        receive_Code <<= 1;
        receive_Code += Data;
        if(Num>=32){
          receive_Flag=1;
          break;
        }
      }
      Num++;
    }
  }
	
	if (receive_Flag == 1)
		{ 
			receive_Flag=0;
			//HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
			printf("%X", receive_Code);//������Ҫ�ض��������ں�ʹ��
			//adc_bat();
			//HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
			switch(receive_Code)
          {
            case 0x00FFE01F:   //���
                 close();
                 break;  
            case 0x00FFA857:  //�ҿ�
                 open();
                 break;  
            case 0x00FF906F:    //ͣ
                 stop();
                 break;  
     
            default:stop();break;
          }
		}
}
}
*/
