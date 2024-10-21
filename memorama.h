#ifndef INC_MEMORAMA_H_
#define INC_MEMORAMA_H_

#include "stm32f4xx_hal.h"

//Puertos y pine LCD
#define LCD_D4_Pin GPIO_PIN_6
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_7
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_9
#define LCD_RS_GPIO_Port GPIOC
#define LCD_RW_Pin GPIO_PIN_8
#define LCD_RW_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_9
#define LCD_E_GPIO_Port GPIOB

//Puertos y pines Botones
#define btn_up_Pin GPIO_PIN_8
#define btn_up_GPIO_Port GPIOC
#define btn_right_Pin GPIO_PIN_6
#define btn_right_GPIO_Port GPIOC
#define btn_down_Pin GPIO_PIN_5
#define btn_down_GPIO_Port GPIOC
#define btn_left_Pin GPIO_PIN_12
#define btn_left_GPIO_Port GPIOA
#define btn_center_Pin GPIO_PIN_11
#define btn_center_GPIO_Port GPIOA

//Puertos y Pines leds
#define led_up_Pin GPIO_PIN_12
#define led_up_GPIO_Port GPIOB
#define led_right_Pin GPIO_PIN_2
#define led_right_GPIO_Port GPIOB
#define led_down_Pin GPIO_PIN_1
#define led_down_GPIO_Port GPIOB
#define led_left_Pin GPIO_PIN_15
#define led_left_GPIO_Port GPIOB
#define led_center_Pin GPIO_PIN_14
#define led_center_GPIO_Port GPIOB

//Instrucciones LCD
#define LCD_clean      0b00000001
#define LCD_home       0b00000010
#define LCD_secondLine 0b11000000
#define LCD_displayOFF 0b00001000
#define LCD_displayON  0b00001100
#define LCD_2lines     0b00101000
#define LCD_entryMode  0b00000110	//Entry Mode Set - Display clear, increment, without shift

//Funciones LCD
void LCD_WR_inst4bits(char instruction4bits);
void LCD_WR_data4bits(char data4bits);
void LCD_WR_inst(char full_instruction);
void LCD_WR_char(char full_data);
void LCD_init(void);
void LCD_WR_string(char sentence[]);

//Botones
void btn_init(void);
void led_init(void);

#endif /* INC_MEMORAMA_H_ */
