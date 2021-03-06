/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Compiler includes. */
#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_tickless_generic.h"

#if configUSE_TICKLESS_IDLE == 1
#include "power_manager.h"
#include "timer_manager.h"
#include "timer_adapter.h"

extern tm_timer_id_t g_rtosSleepTimerID;
void vPortRtcIsr(void);

#define portNVIC_PENDSV_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 24UL)
#define TICKLESS_RTC_BASE_PTR (RTC_Type *) RTC_BASE
#define TICKLESS_RTC_IRQn RTC_FR_IRQn

static uint32_t s_rtcSleepStartCount;

static inline void RTC_SetWakeupCount(RTC_Type *base, uint32_t wakeupValue)
{
	TM_SetTimer(g_rtosSleepTimerID, kTM_SingleShotTimer, wakeupValue, (tm_callback_t)vPortRtcIsr,
				(void *)(g_rtosSleepTimerID));
}

static inline uint32_t RTC_GetWakeupCount(RTC_Type *base)
{
	uint32_t sleepCount = TA_GetTimestamp();
	if (sleepCount >= s_rtcSleepStartCount)
	{
		sleepCount = sleepCount - s_rtcSleepStartCount;
	}
	else
	{
		sleepCount = sleepCount + portMAX_32_BIT_NUMBER - s_rtcSleepStartCount;
	}
	return sleepCount;
}

static inline void RTC_StartTimer(RTC_Type *base)
{
	s_rtcSleepStartCount = TA_GetTimestamp();
}

__weak void RTC_StopTimer(RTC_Type *base);
#endif

extern uint32_t SystemCoreClock; /* in Kinetis SDK, this contains the system core clock speed */

/*
 * LPT timer base address and interrupt number
 */
#if configUSE_TICKLESS_IDLE == 1
RTC_Type *vPortGetRtcBase(void)
{
	return TICKLESS_RTC_BASE_PTR;
}

IRQn_Type vPortGetRtcIrqn(void)
{
	return TICKLESS_RTC_IRQn;
}
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The number of SysTick increments that make up one tick period.
 */
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The maximum number of tick periods that can be suppressed is limited by the
 * 24 bit resolution of the SysTick timer.
 */
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t xMaximumPossibleSuppressedTicks = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The number of LPTIMER increments that make up one tick period.
 */
#if configUSE_TICKLESS_IDLE == 1
	static uint32_t ulLPTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The flag of LPTIMER is occurs or not.
 */
#if configUSE_TICKLESS_IDLE == 1
	static volatile bool ulLPTimerInterruptFired = false;
#endif /* configUSE_TICKLESS_IDLE */

#if configUSE_TICKLESS_IDLE == 1

	void vPortRtcIsr(void)
	{
		ulLPTimerInterruptFired = true;
	}

	void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
	{
		uint32_t ulReloadValue, ulCompleteTickPeriods;
		TickType_t xModifiableIdleTime;
		RTC_Type *pxRtcBase;

		pxRtcBase = vPortGetRtcBase();
		if (pxRtcBase == 0) return;

		/* Make sure the SysTick reload value does not overflow the counter. */
		if( xExpectedIdleTime > xMaximumPossibleSuppressedTicks )
		{
			xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
		}
		if (xExpectedIdleTime == 0) return;
		/* Calculate the reload value required to wait xExpectedIdleTime
		tick periods.  -1 is used because this code will execute part way
		through one of the tick periods. */
		ulReloadValue = ulLPTimerCountsForOneTick * ( xExpectedIdleTime - 1UL );

		/* Stop the RTC and systick momentarily.  The time the RTC and systick is stopped for
		is accounted for as best it can be, but using the tickless mode will
		inevitably result in some tiny drift of the time maintained by the
		kernel with respect to calendar time. */
		RTC_StopTimer(pxRtcBase);
		portNVIC_SYSTICK_CTRL_REG &= ~portNVIC_SYSTICK_ENABLE_BIT;

		/* Enter a critical section but don't use the taskENTER_CRITICAL()
		method as that will mask interrupts that should exit sleep mode. */
		__disable_irq();
		__DSB();
		__ISB();

		/* If a context switch is pending or a task is waiting for the scheduler
		to be unsuspended then abandon the low power entry. */
		if( eTaskConfirmSleepModeStatus() == eAbortSleep )
		{
			/* Restart from whatever is left in the count register to complete
			this tick period. */
			portNVIC_SYSTICK_LOAD_REG = portNVIC_SYSTICK_CURRENT_VALUE_REG;

			/* Restart SysTick. */
			portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;

			/* Reset the reload register to the value required for normal tick
			periods. */
			portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;

			/* Re-enable interrupts - see comments above __disable_irq()
			call above. */
			__enable_irq();
		}
		else
		{
			/* Set the new reload value. */
			RTC_SetWakeupCount(pxRtcBase, ulReloadValue);

			/* Enable RTC. */
			RTC_StartTimer(pxRtcBase);

			/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
			set its parameter to 0 to indicate that its implementation contains
			its own wait for interrupt or wait for event instruction, and so wfi
			should not be executed again.  However, the original expected idle
			time variable must remain unmodified, so a copy is taken. */
			xModifiableIdleTime = xExpectedIdleTime;
			configPRE_SLEEP_PROCESSING( xModifiableIdleTime );
			if( xModifiableIdleTime > 0 )
			{
				__DSB();
				PM_PowerManagement();
				portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
				portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
				portNVIC_SYSTICK_CTRL_REG = portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT;
				__enable_fiq();
				__ISB();
			}
			configPOST_SLEEP_PROCESSING( xExpectedIdleTime );

			ulLPTimerInterruptFired = false;

			/* Re-enable interrupts - see comments above __disable_irq()
			call above. */
			__enable_irq();
			__NOP();
			if( ulLPTimerInterruptFired )
			{
				/* The tick interrupt handler will already have pended the tick
				processing in the kernel.  As the pending tick will be
				processed as soon as this function exits, the tick value
				maintained by the tick is stepped forward by one less than the
				time spent waiting. */
				ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
				ulLPTimerInterruptFired = false;
			}
			else
			{
				/* Something other than the tick interrupt ended the sleep.
				Work out how long the sleep lasted rounded to complete tick
				periods (not the ulReload value which accounted for part
				ticks). */
				ulCompleteTickPeriods = RTC_GetWakeupCount(pxRtcBase) / ulLPTimerCountsForOneTick;
			}

			/* Stop RTC when CPU waked up then set portNVIC_SYSTICK_LOAD_REG back to its standard
			value.  The critical section is used to ensure the tick interrupt
			can only execute once in the case that the reload register is near
			zero. */
			RTC_StopTimer(pxRtcBase);
			portENTER_CRITICAL();
			{
				portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
				vTaskStepTick( ulCompleteTickPeriods );
				portNVIC_SYSTICK_LOAD_REG = ulTimerCountsForOneTick - 1UL;
			}
			portEXIT_CRITICAL();
		}
	}
#endif /* configUSE_TICKLESS_IDLE */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void vPortSetupTimerInterrupt( void )
{
	/* Calculate the constants required to configure the tick interrupt. */
	#if configUSE_TICKLESS_IDLE == 1
		ulTimerCountsForOneTick = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ );
		ulLPTimerCountsForOneTick = configRTC_CLOCK_HZ / configTICK_RATE_HZ;
		xMaximumPossibleSuppressedTicks = portMAX_32_BIT_NUMBER / ulLPTimerCountsForOneTick;
		NVIC_EnableIRQ(vPortGetRtcIrqn());
	#endif /* configUSE_TICKLESS_IDLE */

	/* Configure SysTick to interrupt at the requested rate. */
	portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
	portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );
}
