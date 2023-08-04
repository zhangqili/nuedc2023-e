/*
 * communication.h
 *
 *  Created on: Jun 11, 2023
 *      Author: xq123
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include "stdint.h"
#include "usart.h"

#define BUFFER_LENGTH 64
#define BIT(a) (1<<a)

#define COM_CREATE(u)\
uint8_t u##_RX_Buffer[BUFFER_LENGTH];\
uint8_t u##_RX_Length=0;\
uint8_t u##_TX_Buffer[BUFFER_LENGTH];\
uint8_t u##_TX_Length=0;

#define COM_DECLARE(u)\
extern uint8_t u##_RX_Buffer[BUFFER_LENGTH];\
extern uint8_t u##_RX_Length;\
extern uint8_t u##_TX_Buffer[BUFFER_LENGTH];\
extern uint8_t u##_TX_Length;

#define Communication_SOH(u) u##_TX_Buffer[0]=1;
#define Communication_STX(u) u##_TX_Buffer[1]=2;
#define Communication_ETX(u) u##_TX_Buffer[USART1_TX_Length]=3;
#define Communication_EOH(u) u##_TX_Buffer[USART1_TX_Length]=4;

#define Communication_Add8(u,a,b) u##_TX_Buffer[u##_TX_Length]=(a);\
                               u##_TX_Buffer[u##_TX_Length+1]=(b);\
                               if(u##_TX_Length<BUFFER_LENGTH){u##_TX_Length+=2;}

#define Communication_Add32(u,a,b) u##_TX_Buffer[u##_TX_Length]=(a);\
                               memcpy(u##_TX_Buffer+u##_TX_Length+1,(uint8_t*)&(b),4);\
                               if(u##_TX_Length<BUFFER_LENGTH)u##_TX_Length+=5;

#define Communication_AddLength(u)  u##_TX_Buffer[u##_TX_Length]=(u##_TX_Length+1);\
                                    u##_TX_Length++;

#define Communication_USART1_Transmit_DMA() Communication_AddLength(USART1);\
                                    		HAL_UART_Transmit_DMA(&huart1,USART1_TX_Buffer,USART1_TX_Length);\
                                    		USART1_TX_Length=0;

#define Communication_USART1_Transmit() Communication_AddLength(USART1);\
                                            HAL_UART_Transmit(&huart1,USART1_TX_Buffer,USART1_TX_Length,0xff);\
                                            USART1_TX_Length=0;

#define Communication_Enable(huart,buf,len) HAL_UART_Receive_DMA(huart,buf,len);\
                                            __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);

COM_DECLARE(USART1)
COM_DECLARE(USART3)
COM_DECLARE(UART4)
COM_DECLARE(UART5)

extern uint16_t Communication_TX_Count;
extern uint16_t Communication_RX_Count;

void Communication_Unpack();

#endif /* COMMUNICATION_H_ */
