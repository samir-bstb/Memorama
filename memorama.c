#include "memorama.h"
//#include <stdio.h>
//#include <stdlib.h>

void LCD_WR_inst4bits(char instruction4bits)
{
	char bit;
	//********************************************************
	bit = instruction4bits & 0b00001000;
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, bit );
	//********************************************************
	bit = instruction4bits & 0b00000100;
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, bit );
	//********************************************************
	bit = instruction4bits & 0b00000010;
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, bit );
	//********************************************************
	bit = instruction4bits & 0b00000001;
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, bit );
	//********************************************************

	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET ); //RS=0
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET ); //RW=0
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,  LCD_E_Pin,  GPIO_PIN_SET );   //E=1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,  LCD_E_Pin,  GPIO_PIN_RESET );   //E=0
	HAL_Delay(1); //To wait for Busy Flag (instead checking it)
}

void LCD_WR_data4bits(char data4bits)
{

	uint8_t bit;
	//********************************************************
	bit = data4bits & 0b00001000;
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, bit );
	//********************************************************
	bit = data4bits & 0b00000100;
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, bit );
	//********************************************************
	bit = data4bits & 0b00000010;
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, bit );
	//********************************************************
	bit = data4bits & 0b00000001;
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, bit );
	//********************************************************

	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET );   //RS=1
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET ); //RW=0
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,  LCD_E_Pin,  GPIO_PIN_SET );   //E=1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port,  LCD_E_Pin,  GPIO_PIN_RESET );   //E=0
	HAL_Delay(1); //To wait for Busy Flag (instead checking it)

}

void LCD_WR_inst(char full_instruction)
{
	uint8_t half;
	half = full_instruction>>4;
	LCD_WR_inst4bits(half);
	half = full_instruction&0b00001111;
	LCD_WR_inst4bits(half);
}


void LCD_WR_char(char full_data)
{
	uint8_t half;
	half = full_data>>4;
	LCD_WR_data4bits(half);
	half = full_data&0b00001111;
	LCD_WR_data4bits(half);
}

void LCD_init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, LCD_D4_Pin|LCD_D5_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, LCD_D7_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, LCD_D6_Pin|LCD_RW_Pin|LCD_E_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin */
	  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pins : LCD_D7_Pin LCD_RS_Pin */
	  GPIO_InitStruct.Pin = LCD_D7_Pin|LCD_RS_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*Configure GPIO pins : LCD_D6_Pin LCD_RW_Pin LCD_E_Pin */
	  GPIO_InitStruct.Pin = LCD_D6_Pin|LCD_RW_Pin|LCD_E_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_Delay(15); //wait 15ms
	LCD_WR_inst4bits(0b00000011);
	LCD_WR_inst4bits(0b00000011);
	LCD_WR_inst4bits(0b00000011);
	LCD_WR_inst4bits(0b00000011);
	LCD_WR_inst4bits(0b00000010); //4bits
	LCD_WR_inst(LCD_2lines); //Function Set - 4bits, 2 lines, 5x8 chars
	LCD_WR_inst(LCD_displayOFF); //Display Turn Off
	LCD_WR_inst(LCD_clean); //Display Clean
	LCD_WR_inst(LCD_entryMode); //Entry Mode Set - Display clear, increment, without shift
	LCD_WR_inst(LCD_displayON); //Display Turn On
}

void LCD_WR_string(char sentence[])
{
    uint8_t aux_var = 0;
    while (sentence[aux_var] != '\0')
    {
        LCD_WR_char(sentence[aux_var]); // Print character
        aux_var++; // Move to the next character
    }
}

// Botones
void btn_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(GPIOC, btn_up_Pin|btn_right_Pin|btn_down_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOA, btn_left_Pin|btn_center_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = btn_up_Pin|btn_right_Pin|btn_down_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = btn_left_Pin|btn_center_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//Leds
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(GPIOB, led_up_Pin|led_right_Pin|led_down_Pin|led_left_Pin|led_center_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = led_up_Pin|led_right_Pin|led_down_Pin|led_left_Pin|led_center_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
