extern "C" {
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"
}

// 5 W Green light
//#define PX P2
//#define BITX BIT4
// 10 W Blue light
#define PX P5
#define BITX BIT6
// 15 W Red light
//#define PX P2
//#define BITX BIT6

float g_fLux;
bool g_bFlagFirstTime32Timer1 = true;      // Change variable's name
bool g_bFlagFirstTime32Timer2 = true;      // Change variable's name

uint16_t ADC14Result = 0U;                 // Change variable's name

//bool g_bToggleFlagTimerA = false;          // Change variable's name
bool g_bFlagFirstTimerA = true;            // Change variable's name
bool g_bNightLevel = false;
uint16_t g_iTimerA = 0;
bool g_bAdcFirstFiveSeconds = true;
bool g_bAdcSixthSecond = false;
bool g_bAdcAverageFirstFiveSeconds = true;
bool g_bAdcCompareNUpdate = false;
bool g_bAdcOnLight = false;
int g_iAdcCounter = 0;
uint32_t u32_second1Data = 0;
uint32_t u32_second2Data = 0;
uint32_t u32_second3Data = 0;
uint32_t u32_second4Data = 0;
uint32_t u32_second5Data = 0;
uint32_t u32_second6Data = 0;

void DelayMs () {                // Approximately 1 s
     int l_iCOUNTER = 0;         // Check variable's name. Is it OK?
     while (l_iCOUNTER < 50000)  // Where can we find the cycles number per instruction?
         // We want this function to have a duration of 1 s (at 3 MHz)
         // Can we have the start up simulated with delays?
     {
         l_iCOUNTER++;
     }
 }

void OnOffLed(){
    PX-> OUT = BITX;
    DelayMs();
    PX-> OUT &= ~BITX;
}

void BlinkSetUp ()
 {
    OnOffLed();
    DelayMs();
    OnOffLed();
    DelayMs();
    OnOffLed();
 }


extern "C"
{
    void T32_INT1_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_1->INTCLR = 0U; // clear interrupt flag

        if (g_bNightLevel == true){
            if (g_iTimerA < 600){
                PX-> OUT = BITX;
                g_iTimerA++;
            }
        else {
                PX-> OUT &= ~BITX;
                g_bAdcOnLight = false;
                g_bNightLevel = false;
                g_iTimerA = 0;
            }
        }

        //ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start AD conversion
        __enable_irq();
        return;
    }

    void T32_INT2_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_2->INTCLR = 0U; // clear interrupt flag

        if (g_bFlagFirstTime32Timer2 == true)
        {
            g_bFlagFirstTime32Timer2 = false;
            //P5-> OUT = BIT6;
            //P2-> OUT = ~(BIT4 | BIT6);

            /* Initialize I2C communication */
            Init_I2C_GPIO();
            I2C_init();

            /* Initialize OPT3001 digital ambient light sensor */
            OPT3001_init();
            __delay_cycles(100000);

            TIMER32_2->BGLOAD = 0x000015F9; // 30 ms
        } else {
            //P5-> OUT = ~BIT6;
            //P2-> OUT ^= BIT4 | BIT6;
            g_fLux = OPT3001_getLux();
            ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start AD conversion

            if (g_bFlagFirstTimerA == true){
                if(g_fLux < 30 ){
                    g_bNightLevel = true;
                } else{
                    g_bNightLevel = false;
                }
                g_bFlagFirstTimerA = false;
            } else {
                if((g_fLux < 30) && (g_bAdcOnLight == true)){
                    g_bNightLevel = true;
                } else {
                    g_bNightLevel = false;
                }
            }


            /*
            if (g_bNightLevel == true){
                if (g_iTimerA < 3){
                    PX-> OUT = BITX;
                    g_iTimerA++;
                }
            else {
                    PX-> OUT &= ~BITX;
                }
            }*/
        }
        __enable_irq();
        return;
    }
    /*
    void TA0_N_IRQHandler(void)
    {
        __disable_irq();
        TIMER_A0->CTL &= 0xfffe; // clear interrupt flag
        if (g_bFlagFirstTimerA == true){
            if(g_fLux < 30 ){
                g_bNightLevel = true;
            } else{
                g_bNightLevel = false;
            }
            g_bFlagFirstTimerA = false;
        }

        if (g_bNightLevel == true){
            if (g_iTimerA < 3){
                PX-> OUT = BITX;
                g_iTimerA++;
            }
        else {
                PX-> OUT &= ~BITX;
            }
        }
        __enable_irq();
        return;
    }
    */


    void PORT4_IRQHandler (void)
    {
        __disable_irq();
        P4-> IFG = 0; // clear interrupt flag
        //OnOffLed();
        g_bNightLevel = true;
        g_iTimerA = 0;
        __enable_irq();
        return;
    }


    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        ADC14Result = ADC14->MEM[0];
        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;


        if(g_bAdcFirstFiveSeconds == true)
        {
            if((0 <= g_iAdcCounter) && (g_iAdcCounter< 100)) // pending: use a case
            {
                u32_second1Data += ADC14Result;
            }
            if((100 <= g_iAdcCounter) && (g_iAdcCounter< 200))
            {
                u32_second2Data += ADC14Result;
            }
            if((200 <= g_iAdcCounter) && (g_iAdcCounter< 300))
            {
                u32_second3Data += ADC14Result;
            }
            if((300 <= g_iAdcCounter) && (g_iAdcCounter< 400))
            {
                u32_second4Data += ADC14Result;
            }
            if((400 <= g_iAdcCounter) && (g_iAdcCounter< 500))
            {
                u32_second5Data += ADC14Result;
            }
            g_iAdcCounter++;
            if(g_iAdcCounter == 501)
            {
                g_bAdcFirstFiveSeconds = false;
                g_bAdcSixthSecond = true;
                g_iAdcCounter = 0;
            }
        }

        if(g_bAdcSixthSecond == true)
        {
            u32_second6Data += ADC14Result;
            g_iAdcCounter++;
            if(g_iAdcCounter == 100)
            {
                g_bAdcSixthSecond = false;
                g_bAdcCompareNUpdate = true;
                g_iAdcCounter = 0;

            }
        }

        if(g_bAdcCompareNUpdate == true)
        {
            if(g_bAdcAverageFirstFiveSeconds == true)
            {
                u32_second1Data = u32_second1Data/100;
                u32_second2Data = u32_second2Data/100;
                u32_second3Data = u32_second3Data/100;
                u32_second4Data = u32_second4Data/100;
                u32_second5Data = u32_second5Data/100;
                g_bAdcAverageFirstFiveSeconds = false;
            }
            u32_second6Data = u32_second6Data/100;

            if(u32_second6Data > (u32_second1Data + (u32_second1Data*0.1)) || // 10% ?
               u32_second6Data > (u32_second2Data + (u32_second2Data*0.1)) ||
               u32_second6Data > (u32_second3Data + (u32_second3Data*0.1)) ||
               u32_second6Data > (u32_second4Data + (u32_second4Data*0.1)) ||
               u32_second6Data > (u32_second5Data + (u32_second5Data*0.1)))
            {
                g_bAdcOnLight = true;
                //OnOffLed();
                //PX-> OUT = BITX;
            }//else{ /*PX-> OUT &= ~BITX; }*/ g_bAdcOnLight = false; } // Move these "clean flag" into the timer logic

            u32_second1Data = u32_second2Data;
            u32_second2Data = u32_second3Data;
            u32_second3Data = u32_second4Data;
            u32_second4Data = u32_second5Data;
            u32_second5Data = u32_second6Data;
            u32_second6Data = 0;
            g_bAdcCompareNUpdate = false;
            g_bAdcSixthSecond = true;
        }

        __enable_irq();
        return;
    }

}




int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer

    PX-> OUT &= ~BITX;
    // P2OUT register starts dirty when we run the program. Do we need to keep
    // cleaning those kind of registers at the beginning of the code every time?
    PX-> DIR = BITX;

    BlinkSetUp();

    // *********** TIMER A ***********
    // We are using ACLK (REFOCLK)
    //By default TimerA uses __LFXT: 32768 Hz clk source
    /*
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_IE | TIMER_A_CTL_MC_2 ; // ACLK | enable interrupt | up to FFFF |

    NVIC_SetPriority(TA0_N_IRQn,1);
    NVIC_EnableIRQ(TA0_N_IRQn);
    */


    // What is the difference between TA0_N and TA0_0 (irq's name)? The interrupt we coded is just working
    // with the TA0_N
    // We are not getting the frequency we want even if are configuring everything the right way. Why?
    // Test it!

    // *********** TIMER 32_1: ADC Enable ***********
    // To use the timer 32
    TIMER32_1->LOAD = 0x00000753; //~10 ms --> 3 MHz on clk
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn,1);
    NVIC_EnableIRQ(T32_INT1_IRQn);
    // working the right way!!!!

    // *********** TIMER 32_2: LightSensor read ***********
    // To use the timer 32
    TIMER32_2->LOAD = 0x000000ff; //immediately
    TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT2_IRQn,1);
    NVIC_EnableIRQ(T32_INT2_IRQn);

    // *********** BUTTON ***********
    // The button is in the p4.1 pin
    P4-> DIR = ~BIT1;
    P4-> REN = BIT1;
    P4-> OUT = BIT1;
    P4-> DS = ~BIT1;
    P4-> SEL0 = ~BIT1;
    P4-> SEL1 = ~BIT1;
    P4-> IES = ~BIT1;
    P4-> IFG = ~BIT1;
    P4-> IE = BIT1;

    NVIC_SetPriority(PORT4_IRQn,1);
    NVIC_EnableIRQ(PORT4_IRQn);


    // ******* Microphone *******
    // It uses the pin J1.6 -> P4.3 to send data trough an analog In
    // Set P4.3 for Analog input, disabling the I/O circuit.
        P4->SEL0 = BIT3;
        P4->SEL1 = BIT3;
        P4->DIR &= ~BIT3;

        ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
                      ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON
                      | ADC14_CTL0_SHP; // not diving the ADC's clk | using ADC pin to sample and hold | why?
                                        // | why? | time to sample | turn the converter on | why?
        ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0; // Why INCH_10?  | VCC & VSS
        ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC; //ADC14 Conversion Enable
        ADC14->IER0 = ADC14_IER0_IE0;   // Enables ADC14's interrupt
        NVIC_SetPriority(ADC14_IRQn,1); // Interrupt Priority
        NVIC_EnableIRQ(ADC14_IRQn);     // Enable Interrupt Queue

    while(true){

    }

    return 0;
}
