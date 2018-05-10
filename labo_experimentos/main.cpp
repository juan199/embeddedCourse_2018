// Lab 1

// Including files to use Driver Lib, some C functions and I2C + light sensor modules

extern "C"
{
    #include <ti/devices/msp432p4xx/inc/msp.h>
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include <ti/grlib/grlib.h>
    #include "HAL_I2C.h"
    #include "HAL_OPT3001.h"
}

#include "functions.hpp"

// To select the led

// 5 W Green light
//#define PX P2
//#define BITX BIT4

// 10 W Blue light
#define PX P5
#define BITX BIT6

// 15 W Red light
//#define PX P2
//#define BITX BIT6

// Variables

float g_fLux;

bool g_bFlagFirstTime32Timer2 = true;
bool g_bFirstLightMeasure = true;
bool g_bNightLevel = false;
bool g_bAdcFirstFiveSeconds = true;
bool g_bAdcSixthSecond = false;
bool g_bAdcAverageFirstFiveSeconds = true;
bool g_bAdcCompareNUpdate = false;
bool g_bAdcOnLight = false;

int g_iAdcCounter = 0;

uint16_t g_ui16AdcXResult = 0U;
uint16_t g_ui16AdcYResult = 0U;
uint16_t g_ui16AdcZResult = 0U;
uint16_t g_iCounterT321 = 0;

uint32_t g_u32_second1Data = 0;
uint32_t g_u32_second2Data = 0;
uint32_t g_u32_second3Data = 0;
uint32_t g_u32_second4Data = 0;
uint32_t g_u32_second5Data = 0;
uint32_t g_u32_second6Data = 0;

// Functions

// To blink once the led selected
void OnOffLed()
{
    PX-> OUT = BITX;
    DelayMs();
    PX-> OUT &= ~BITX;
}

// Interrupt handlers

extern "C"
{
    void T32_INT1_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_1->INTCLR = 0U;
        ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC;
        __enable_irq();
        return;
    }

    void T32_INT2_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_2->INTCLR = 0U;
        // Initialize I2C communication and OPT3001 digital ambient light sensor the very fist time
        if (g_bFlagFirstTime32Timer2 == true)
        {
            g_bFlagFirstTime32Timer2 = false;

            Init_I2C_GPIO();
            I2C_init();
            OPT3001_init();
            __delay_cycles(100000);

            TIMER32_2->BGLOAD = 0x00000753; // Changing Timer322 load -> 10 ms
        } else
        {

            // To get data from the light sensor and starting the AD converter
            g_fLux = OPT3001_getLux();
            ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC;

            // To select flags according to first light sensor measure
            if (g_bFirstLightMeasure == true)
            {
                if(g_fLux < 30 ){
                    g_bNightLevel = true;
                } else
                {
                    g_bNightLevel = false;
                }
                g_bFirstLightMeasure = false;
            } else
            // To check 'start on' condition (light sensor measure + microphone data)
            {
                if((g_fLux < 30) && (g_bAdcOnLight == true))
                {
                    g_bNightLevel = true;
                }
            }
        }
        __enable_irq();
        return;
    }

    void PORT4_IRQHandler (void)
    {
        __disable_irq();
        P4-> IFG = 0;

        // Set some flags to active led
        g_bNightLevel = true;
        g_iCounterT321 = 0;
        __enable_irq();
        return;
    }


    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        // To get light sensor data
        g_ui16AdcXResult = ADC14->MEM[0];
        g_ui16AdcYResult = ADC14->MEM[1];
        g_ui16AdcZResult = ADC14->MEM[2];

        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0 | ADC14_CLRIFGR0_CLRIFG1 | ADC14_CLRIFGR0_CLRIFG2;
        __enable_irq();
        return;
    }
}

int main(void)
{
    // Stop watchdog timer & configuring led
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    PX-> OUT &= ~BITX;
    PX-> DIR = BITX;

    P4->SEL0 = BIT0 | BIT2;
    P4->SEL1 = BIT0 | BIT2;
    P4->DIR &= ~(BIT0 | BIT2);
    P6->SEL0 = BIT1;
    P6->SEL1 = BIT1;
    P6->DIR &= ~BIT1;

    // *********** TIMER 32_1: counting to shut light off ***********
    // Setting up the Timer 32_1
    Timer321Setup();

    // *********** BUTTON ***********
    // The button is in the p4.1 pin
    ButtonSetup();

    // ******* ADC *******
    // Initialize the ADC to get data every time Timer 32_2 interrupts
    AdcSetup();

    while(true)
    {

    }

    return 0;
}
