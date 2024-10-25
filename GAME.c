/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "memorama.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

GPIO_TypeDef* led_ports[] = {led_up_GPIO_Port , led_right_GPIO_Port, led_down_GPIO_Port , led_left_GPIO_Port , led_center_GPIO_Port};
uint16_t led_pins[] = {led_up_Pin, led_right_Pin , led_down_Pin , led_left_Pin, led_center_Pin};

GPIO_TypeDef* Button_Ports[] = {btn_up_GPIO_Port, btn_right_GPIO_Port, btn_down_GPIO_Port, btn_left_GPIO_Port, btn_center_GPIO_Port};
uint16_t Button_Pins[] = {btn_up_Pin, btn_right_Pin, btn_down_Pin , btn_left_Pin, btn_center_Pin};

int cur_sec[10];

int user_sec[20]; //suitable for every level (easy, medium & hard)

int delay = 500; //regresarlo a 1000

int nivel_selec = 0;
//button read functions
int read_buttons(void){
	while (1) {
		for (int i = 0; i < 5; i++) {
			if (HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i]) == GPIO_PIN_SET) {
				HAL_Delay(40);
				if (HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i]) == GPIO_PIN_SET) {
					while (HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i]) == GPIO_PIN_SET) {}
					HAL_Delay(40);
					return 1;
				}
	        }
	    }
	}
}

int btn_a_led(int idx){
	int x = 0;
	while(1){
		for(int i = 0; i < 5; i++){
			if(HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i])){
				HAL_Delay(40);
				if (HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i]) == GPIO_PIN_SET) {
					HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_SET);  // Turn LED on
					while (HAL_GPIO_ReadPin(Button_Ports[i], Button_Pins[i]) == GPIO_PIN_SET);
					HAL_Delay(40);
					user_sec[idx] = (x|1) << (4-i); //posicion en la que el usuario prende un led
					HAL_Delay(600);
					HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
					if(cur_sec[idx] != user_sec[idx]){
											return 0;
										}
					return 1;// Turn LED off
				}
			}
		}
	}
}

int botones_nivel(int nivel_act){
	int nivel_max = 3;

	while(1)
	{
		if(HAL_GPIO_ReadPin(btn_up_GPIO_Port, btn_up_Pin)){
			HAL_Delay(40);
			while(HAL_GPIO_ReadPin(btn_up_GPIO_Port, btn_up_Pin));
			HAL_Delay(40);
			if(nivel_act > 1){
				nivel_act --;
				break;
			}
		} else if(HAL_GPIO_ReadPin(btn_down_GPIO_Port, btn_down_Pin)){
			HAL_Delay(40);
			while(HAL_GPIO_ReadPin(btn_down_GPIO_Port, btn_down_Pin));
			HAL_Delay(40);
			if(nivel_act < nivel_max){
				nivel_act ++;
				break;
			}
		} else if(HAL_GPIO_ReadPin(btn_center_GPIO_Port, btn_center_Pin)){
			HAL_Delay(40);
			while(HAL_GPIO_ReadPin(btn_center_GPIO_Port, btn_center_Pin));
			HAL_Delay(40);
			return 0;
		}
	}

	return nivel_act;
}


int mostrar_niveles(void)
{
	int nivel = 1;
	int aux;

	while(1){

		LCD_WR_inst(LCD_clean);
		aux = 0;

		if (nivel == 1) {
			LCD_WR_string("> Facil");
			LCD_WR_inst(LCD_secondLine);
			LCD_WR_string("  Intermedio");

		} else if (nivel == 2) {
			LCD_WR_string("  Facil");
			LCD_WR_inst(LCD_secondLine);
			LCD_WR_string("> Intermedio");

		} else if (nivel == 3) {
			LCD_WR_string("  Intermedio");
			LCD_WR_inst(LCD_secondLine);
			LCD_WR_string("> Dificil");
		}


        HAL_Delay(100);
        aux = botones_nivel(nivel);
        if(aux == 0){
        	return nivel;
        } else {
        	nivel = aux;
        }
	}
}


//Game inicialization
void start(){
	LCD_WR_string("    MEMORAMA");
	LCD_WR_inst(0b11000000);
	LCD_WR_string("Pulse un boton");
	int x = read_buttons();
	if(x == 1){
		LCD_WR_inst(LCD_clean);
		LCD_WR_string("Comenzando...");
		HAL_Delay(1000);
		LCD_WR_inst(LCD_clean);
		LCD_WR_string("Selecciona un");
		LCD_WR_inst(0b11000000);
		LCD_WR_string("Nivel");
		HAL_Delay(1000);
		nivel_selec = mostrar_niveles();
		LCD_WR_inst(LCD_clean);
		LCD_WR_string("Seleccionado");
		/*
		HAL_Delay(800);
		LCD_WR_inst(LCD_clean);
		LCD_WR_string("A jugar!!");
		HAL_Delay(1000);
		*/
	}
}

//Reset Game
void reset(){
	LCD_WR_inst(LCD_clean);
	for (int i = 0; i < 5; i++) {
		HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
	}
	for (int i = 0; i < 10; i++) {
		cur_sec[i] = 0;
		user_sec[i] = 0;
	}
}

void create_sequence(int idx){
	int x = 0;
	srand(HAL_GetTick());
	int k = (rand() % 5);
	cur_sec[idx] = (x|1) << (4-k);
}

void play_sequence(int lim){
	for(int i = 0; i< lim; i++){
		int count = 0;//explain
		int aux = cur_sec[i];
		while(aux != 0){
			if(aux & 1){
				break; //breaks the loop when the position of the led to turn on is identified
			}
			aux >>= 1;
			count += 1;
		}
		count = abs(count - 4);
		HAL_GPIO_WritePin(led_ports[count], led_pins[count], GPIO_PIN_SET);
		HAL_Delay(delay); //should be a parameter,and modified depending on the level
		HAL_GPIO_WritePin(led_ports[count], led_pins[count], GPIO_PIN_RESET);
		HAL_Delay(100);
	}
}

int verify_entry(int lim){ //determines whether the user continues/wins or loses
	for(int i = 0; i <= lim; i++){
		if (user_sec[i] != cur_sec[i]){
			return 0; //returns False
		}
	}
	return 1;
}

void win(void){
	LCD_WR_inst(LCD_clean);
	LCD_WR_string("Felicidades");
	LCD_WR_inst(LCD_secondLine);
	LCD_WR_string("Ganaste!!");
}



int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LCD_init();
  btn_init();
  led_init();
  int y = 0; //sequence length
  int state = 0; // 0 (when you lose or the game is starting), 1 (when playing)
  int lim;



  while (1)
  {
    /* USER CODE END WHILE */
	if(state == 0){
		start();
		state = 1;
		y = 0;
	}

	if(state == 1){
		LCD_WR_inst(LCD_clean);
		char sentence[4];
		sprintf(sentence, "%d", y);
		LCD_WR_string(sentence);
		if(nivel_selec == 1){
		  delay -= 25;
		  lim = 4; //lim 10

		} else if(nivel_selec == 2){
		  delay -= 32;
		  lim = 15;
		} else if(nivel_selec ==3){
		  delay -= 15;
		  lim = 20;

		}

		create_sequence(y);
		play_sequence(y + 1);
		for(int j = 0; j < (y + 1); j++){//leer el boton "y" veces
			int v = btn_a_led(j);
			if(v == 0){
				state = 0;
				reset();
				break;
			}
		}

		if(state == 1){
			int usr_entry = verify_entry(y);

			if(usr_entry == 0){
				LCD_WR_string("Perdiste ):");
				HAL_Delay(500);
				state = 0;
				reset();
			}

			if(lim == y){
				state = 0;
				win();
				reset();
			}
			y++;
		}
	}

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
