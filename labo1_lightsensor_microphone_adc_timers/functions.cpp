/*
 * functions.cpp
 */

#include <ti/devices/msp432p4xx/inc/msp.h>
#include "functions.hpp"

// To configure the button as an interruption
void ButtonSetup()
{
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
}

// To configure the pins microphone uses
void MicrophoneSetup()
{
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;
}

// To configure the ADC module
void AdcSetup()
{
    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
                  ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON
                  | ADC14_CTL0_SHP; // not diving the ADC's clk | using ADC pin to sample and hold
                                    // time to sample | turn the converter on
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0; // VCC & VSS
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC; //ADC14 Conversion Enable
    ADC14->IER0 = ADC14_IER0_IE0;   // Enables ADC14's interrupt
    NVIC_SetPriority(ADC14_IRQn,1); // Interrupt Priority
    NVIC_EnableIRQ(ADC14_IRQn);     // Enable Interrupt Queue
}

// To configure the Timer 32 1
void Timer321Setup()
{
    TIMER32_1->LOAD = 0x00000753; //~10 ms --> 3 MHz on clk
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn,1);
    NVIC_EnableIRQ(T32_INT1_IRQn);
}

// To configure the Timer 32 2
void Timer322Setup()
{
    TIMER32_2->LOAD = 0x000000ff;
    TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT2_IRQn,1);
    NVIC_EnableIRQ(T32_INT2_IRQn);
}

// To have some delay
void DelayMs()
{
    int l_iCOUNTER = 0;
    while (l_iCOUNTER < 50000)
    {
        l_iCOUNTER++;
    }
}

// To do the blink set up
void BlinkSetUp ()
{
    OnOffLed();
    DelayMs();
    OnOffLed();
    DelayMs();
    OnOffLed();
}
