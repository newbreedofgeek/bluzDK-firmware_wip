/**
 ******************************************************************************
 * @file    rtc_hal.c
 * @author  Satish Nair
 * @version V1.0.0
 * @date    18-Dec-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-14 Spark Labs, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "rtc_hal.h"
#include "stm32f2xx_rtc.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void HAL_RTC_Configuration(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	__IO uint32_t AsynchPrediv = 0x7F, SynchPrediv = 0xFF;

	/* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_Alarm_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Check if the StandBy flag is set */
	if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
	{
		/* System resumed from STANDBY mode */

		/* Clear StandBy flag */
		PWR_ClearFlag(PWR_FLAG_SB);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		EXTI_ClearITPendingBit(EXTI_Line17);

		/* No need to configure the RTC as the RTC configuration(clock source, enable,
	       prescaler,...) is kept after wake-up from STANDBY */
	}
	else
	{
		/* StandBy flag is not set */

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			//Do nothing
		}

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

		/* Check on RTC init */
		if (RTC_Init(&RTC_InitStructure) == ERROR)
		{
			/* RTC Prescaler Config failed */
		}
	}
}

time_t HAL_RTC_Get_UnixTime(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	/* Get the current Time and Date */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	struct tm calendar_time;

	/* Set calendar_time time struct values */
	calendar_time.tm_hour = RTC_TimeStructure.RTC_Hours;
	calendar_time.tm_min = RTC_TimeStructure.RTC_Minutes;
	calendar_time.tm_sec = RTC_TimeStructure.RTC_Seconds;

	/* Set calendar_time date struct values */
	calendar_time.tm_wday = RTC_DateStructure.RTC_WeekDay;
	calendar_time.tm_mday = RTC_DateStructure.RTC_Date;
	calendar_time.tm_mon = RTC_DateStructure.RTC_Month;
	calendar_time.tm_year = RTC_DateStructure.RTC_Year;

	return (time_t)mktime(&calendar_time);
}

void HAL_RTC_Set_UnixTime(time_t value)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	struct tm *calendar_time;
	calendar_time = localtime(&value);

	/* Get calendar_time time struct values */
	RTC_TimeStructure.RTC_Hours = calendar_time->tm_hour;
	RTC_TimeStructure.RTC_Minutes = calendar_time->tm_min;
	RTC_TimeStructure.RTC_Seconds = calendar_time->tm_sec;

	/* Get calendar_time date struct values */
	RTC_DateStructure.RTC_WeekDay = calendar_time->tm_wday;
	RTC_DateStructure.RTC_Date = calendar_time->tm_mday;
	RTC_DateStructure.RTC_Month = calendar_time->tm_mon;
	RTC_DateStructure.RTC_Year = calendar_time->tm_year;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		/* RTC Set Time failed */
	}

	/* Configure the RTC date register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	{
		/* RTC Set Date failed */
	}
}

void HAL_RTC_Set_UnixAlarm(time_t value)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;

	/* Disable the Alarm A */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

	struct tm *alarm_time;
	alarm_time = localtime(&value);

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_TimeStructure.RTC_Hours + alarm_time->tm_hour;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes + alarm_time->tm_min;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = RTC_TimeStructure.RTC_Seconds + alarm_time->tm_sec;

	/* Configure the RTC Alarm A register */
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	/* Enable the RTC Alarm A Interrupt */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* Enable the Alarm  A */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	/* Clear RTC Alarm Flag */
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

void RTC_Alarm_irq(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		if(NULL != HAL_RTCAlarm_Handler)
		{
			HAL_RTCAlarm_Handler();
		}

		/* Clear EXTI line17 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line17);

		/* Check if the Wake-Up flag is set */
		if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
		{
			/* Clear Wake Up flag */
			PWR_ClearFlag(PWR_FLAG_WU);
		}

		/* Clear RTC Alarm interrupt pending bit */
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
}
