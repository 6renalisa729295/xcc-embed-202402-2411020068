#include "main.h"

void GPIO_Init_Manual(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  // 行 PE4,PE5,PE6 推挽输出
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

  // 列 PE0~PE3 上拉输入
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // LED PF0~PF7
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
}

uint8_t KeyScan(void)
{
	uint8_t key = 0;
	// 所有行拉高
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_SET);

	// 第1行 PE4
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_Delay(1);
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == GPIO_PIN_RESET) key = 1;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == GPIO_PIN_RESET) key = 2;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET) key = 3;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) key = 4;
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
	if(key != 0)
	{
		HAL_Delay(10);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1)==0 ||
			   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)==0 );
		return key;
	}

	// 第2行 PE5
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_Delay(1);
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == GPIO_PIN_RESET) key = 5;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == GPIO_PIN_RESET) key = 6;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET) key = 7;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) key = 8;
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
	if(key != 0)
	{
		HAL_Delay(10);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1)==0 ||
			   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)==0 );
		return key;
	}

	// 第3行 PE6
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_Delay(1);
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == GPIO_PIN_RESET) key = 9;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == GPIO_PIN_RESET) key = 10;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET) key = 11;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) key = 12;
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
	if(key != 0)
	{
		HAL_Delay(10);
		while( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1)==0 ||
			   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)==0 || HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)==0 );
		return key;
	}
	return 0;
}

void BCD_Output(uint8_t num)
{
	uint8_t bcd;
	if(num > 9)
		bcd = 0;
	else
		bcd = num;

	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
	                         GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

	if(bcd & (1<<0)) HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET); //1
	if(bcd & (1<<1)) HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET); //2
	if(bcd & (1<<2)) HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_SET); //4
	if(bcd & (1<<3)) HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET); //8
}

int main(void)
{
	uint8_t key_val;
	uint8_t num = 0;

	HAL_Init();
	GPIO_Init_Manual();
	BCD_Output(0);

	while (1)
	{
		key_val = KeyScan();
		if(key_val != 0)
		{
			num = key_val;
			BCD_Output(num);
		}
	}
}
