#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "ADC_en.hpp"
#include "ScreenWriter.hpp"
#include "AngleCalc.hpp"

extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
}

//#include "msp.h"


// ##########################
// Global/Static declarations
// ##########################
uint8_t Task::m_u8NextTaskID = 0; // - Init task ID

volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Mailbox* g_Mailbox = Mailbox::getMailbox();
Scheduler g_MainScheduler; // - Instantiate a Scheduler

ADC_en UniqueADC;




/* ADC results buffer */
//static uint16_t resultsBuffer[3];

//void drawTitle(void);

Graphics_Rectangle drawRect;
Graphics_Rectangle drawRect2;



// #########################
//          MAIN
// #########################
void main(void)
{
    /*
    drawRect.xMin = 0;
    drawRect.xMax = 127;

    drawRect.yMin = 0;
    drawRect.yMax = 64;

    drawRect2.xMin = 0;
    drawRect2.xMax = 70;

    drawRect2.yMin = 0;
    drawRect2.yMax = 40;

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BROWN);

    Graphics_fillRectangle(&g_sContext, &drawRect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BROWN);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_fillRectangle(&g_sContext, &drawRect2);
    Graphics_drawLine(&g_sContext,0,64,127,84);
    Graphics_drawLineH(&g_sContext, 0, 127, 90);
    */
    // - Instantiate two new Tasks
    LED BlueLED(BIT2);
    LED GreenLED(BIT1);

    AngleCalc Angle;
    ScreenWriter Screen;

    // - Run the overall setup function for the system
    Setup();
    // - Attach the Tasks to the Scheduler;
    g_MainScheduler.attach(&BlueLED,TaskType_Periodic, TaskActiveTrue,500);
    g_MainScheduler.attach(&GreenLED, TaskType_Periodic,TaskActiveFalse,600);
    g_MainScheduler.attach(&UniqueADC,TaskType_Always, TaskActiveTrue);
    g_MainScheduler.attach(&Angle,TaskType_Periodic, TaskActiveTrue,32);
    g_MainScheduler.attach(&Screen,TaskType_Periodic, TaskActiveTrue,64);

    // aquii se definen los destinos de cada task!
    //UniqueADC.MessageADC.u8DestinationID = Angle.m_u8TaskID;
    g_MainScheduler.DefineDestination(&UniqueADC, &Angle);
    g_MainScheduler.DefineDestination(&Angle, &Screen);
    //Angle.st_taskMessage.u8DestinationID = Screen.m_u8TaskID;

    // - Run the Setup for the scheduler and all tasks
    g_MainScheduler.setup();



    // - Main Loop
    while(1)
    {
    	__wfe(); // Wait for Event
        if(g_SystemTicks != g_MainScheduler.m_u64ticks)
        {
            //- Only execute the tasks if one tick has passed.
            g_MainScheduler.m_u64ticks = g_SystemTicks;
            g_MainScheduler.run();
        }
    }
}

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
	// ****************************
	//         DEVICE CONFIG
	// ****************************
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	// - This is the heart beat indicator.
	P1->DIR |= BIT0; //Red LED

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 3Mhz
	//0x00000BB8 para 1ms a 3 MHz
	//0x0000BB80 para 1ms a 48
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	__enable_irq();


	return;
}

extern "C"
{
    // - Handle the Timer32 Interrupt
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0; // - Toggle the heart beat indicator (1ms)
		g_SystemTicks++;
		return;
	}

	// - Handle ADC
    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        UniqueADC.m_i16AdcXResult = ADC14->MEM[0];
        UniqueADC.m_i16AdcYResult = ADC14->MEM[1];
        UniqueADC.m_i16AdcZResult = ADC14->MEM[2];
        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0 | ADC14_CLRIFGR0_CLRIFG1 | ADC14_CLRIFGR0_CLRIFG2;
        __enable_irq();
        return;
    }
}


