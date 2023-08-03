/*
 * communication.c
 *
 *  Created on: Jun 11, 2023
 *      Author: xq123
 */
#include "main.h"
#include "communication.h"
#include "stdbool.h"
#include "steer.h"

COM_CREATE(USART1)
COM_CREATE(USART3)
COM_CREATE(UART4)
COM_CREATE(UART5)

uint16_t Communication_TX_Count = 0;
uint16_t Communication_RX_Count = 0;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;

extern cartesian_coordinate_system_t a4_points[4];

void Communication_Unpack(UART_HandleTypeDef *huart)
{
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        HAL_UART_DMAStop(huart);
        if (huart->Instance == USART1)
        {
            Communication_RX_Count++;
            USART1_RX_Length = BUFFER_LENGTH
                    - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
            a4_points[0].x = (int8_t) USART1_RX_Buffer[0];
            a4_points[0].z = (int8_t) USART1_RX_Buffer[1];
            a4_points[1].x = (int8_t) USART1_RX_Buffer[2];
            a4_points[1].z = (int8_t) USART1_RX_Buffer[3];
            a4_points[2].x = (int8_t) USART1_RX_Buffer[4];
            a4_points[2].z = (int8_t) USART1_RX_Buffer[5];
            a4_points[3].x = (int8_t) USART1_RX_Buffer[6];
            a4_points[3].z = (int8_t) USART1_RX_Buffer[7];
            HAL_UART_Receive_DMA(huart, USART1_RX_Buffer, BUFFER_LENGTH);
        }
        if (huart->Instance == USART3)
        {
            USART3_RX_Length = BUFFER_LENGTH
                    - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
            if (USART3_RX_Buffer[USART3_RX_Length - 1] == 9)
            {
                a4_points[0].x = (int8_t) USART3_RX_Buffer[0];
                a4_points[0].z = (int8_t) USART3_RX_Buffer[1];
                a4_points[1].x = (int8_t) USART3_RX_Buffer[2];
                a4_points[1].z = (int8_t) USART3_RX_Buffer[3];
                a4_points[2].x = (int8_t) USART3_RX_Buffer[4];
                a4_points[2].z = (int8_t) USART3_RX_Buffer[5];
                a4_points[3].x = (int8_t) USART3_RX_Buffer[6];
                a4_points[3].z = (int8_t) USART3_RX_Buffer[7];
            }
            HAL_UART_Receive_DMA(huart, USART3_RX_Buffer, BUFFER_LENGTH);
        }
        if (huart->Instance == UART4)
        {
            UART4_RX_Length = BUFFER_LENGTH
                    - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);

            HAL_UART_Receive_DMA(huart, UART4_RX_Buffer, BUFFER_LENGTH);
        }
    }
}

