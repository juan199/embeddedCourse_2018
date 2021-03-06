#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "ADC_en.hpp"
#include "ScreenWriter.hpp"
#include "AngleCalc.hpp"


//#include "msp.h"


// ##########################
// Global/Static declarations
// ##########################
uint8_t Task::m_u8NextTaskID = 0; // - Init task ID

volatile static uint64_t g_SystemTicks = 0; // - The system counter.
Mailbox* g_Mailbox = Mailbox::getMailbox();
Scheduler g_MainScheduler; // - Instantiate a Scheduler

// - Instantiate a new task
ADC_en UniqueADC;

// #########################
//          MAIN
// #########################
void main(void)
{
    // - Instantiate two new Tasks
    AngleCalc Angle;
    ScreenWriter Screen;

    // - Run the overall setup function for the system
    Setup();
    // - Attach the Tasks to the Scheduler;
    g_MainScheduler.attach(&UniqueADC,TaskType_Always, TaskActiveTrue);
    g_MainScheduler.attach(&Angle,TaskType_Periodic, TaskActiveTrue,32);
    g_MainScheduler.attach(&Screen,TaskType_Periodic, TaskActiveTrue,64);

    // Define task's Destination
    g_MainScheduler.DefineDestination(&UniqueADC, &Angle);
    g_MainScheduler.DefineDestination(&Angle, &Screen);

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
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 48Mhz
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


