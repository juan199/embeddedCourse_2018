// *********** MAIN.CPP ***********

#include "msp.h"

   void DelayMs () {                // Approximately 1 s
        int l_iCOUNTER = 0;         // Check variable's name. Is it OK?
        while (l_iCOUNTER < 50000)  // Where can we find the cycles number per instruction?
            // We want this function to have a duration of 1 s (at 3 MHz)
        {
            l_iCOUNTER++;
        }
    }

   void OnOffLed(){
       P5-> OUT = BIT6;
       P2-> OUT = BIT4 | BIT6;
       DelayMs();
       P5-> OUT = ~BIT6;
       P2-> OUT = ~(BIT4 | BIT6);
       DelayMs();
}

   void BlinkSetUp ()
    {
       OnOffLed();
       OnOffLed();
       OnOffLed();
    }

int contador = 0; // change variable's name

int main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer
    P1-> IE = 0;

    //  Leds: P5.6 Blue, P2.4 Green, P2.6 Red
    P5-> OUT = ~BIT6;
    P2-> OUT = ~(BIT4 | BIT6); // P2OUT register starts dirty when we run the program. Do we need to keep
        // cleaning those kind of registers at the beginning of the code every time?
    P5-> DIR = BIT6;
    P2-> DIR = BIT4 | BIT6;

    BlinkSetUp();

    // *********** LIGHT SENSOR ***********
    // Light sensor pins: P4.6 J8, P6.5 J9 and P6.4 J10
    // p4.6 j8 GPIO
    // p6.5 j9 I2C
    // p6.4 j10 i2C

    // To start using the light sensor
    P6-> DIR = 0;
    P4-> DIR = 0;

    // To receive data from peripherals
    P6-> SEL1 = 1;
    P4-> SEL1 = 1;

    // To enable write access on map registers
    //PMAP-> KEYID = 0x02D52;


    // *********** TIMER 32 ***********
    // To use the timer 32
    /*
    TIMER32_1->LOAD = 0x0002dce1; //~1 s --> a 3 Mhz en el clk, 187.5 kHz para cada cuenta
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn,1);
    NVIC_EnableIRQ(T32_INT1_IRQn);
    */

    // *********** BUTTON ***********
    // The button is in the p3.5 pin
    P3-> DIR = ~BIT5;
    P3-> REN = BIT5;
    P3-> OUT = BIT5;
    P3-> DS = ~BIT5;
    P3-> SEL0 = ~BIT5;
    P3-> SEL1 = ~BIT5;
    P3-> IES = ~BIT5;
    P3-> IFG = ~BIT5;
    P3-> IE = BIT5;

    NVIC_SetPriority(PORT3_IRQn,1);
    NVIC_EnableIRQ(PORT3_IRQn);


    // Button in the black board: p1.1 pin
    P1-> DIR = 0x00;
    P1-> OUT = 0xff; // pull down or pull up? Check this!
    P1-> REN = 0xff;
    P1-> DS = 0x00;
    P1-> SEL0 = 0x00;
    P1-> SEL1 = 0x00;
    P1-> IES = 0xff;
    P1-> IFG = 0x00;
    P1-> IE = 0xff;
    //P1-> IFG = 1; // set interrupt flag

    NVIC_SetPriority(PORT1_IRQn,1);
    NVIC_EnableIRQ(PORT1_IRQn);

    while(true){

    }

    return 0;
}

extern "C"
{
    void T32_INT1_IRQHandler(void)
    {
        __disable_irq();
        TIMER32_1->INTCLR = 0U; // clear interrupt flag
        // Why are we using OU here?
        //P1->OUT ^= BIT0;
        //OnOffLed();
        if (contador == 0) {
            P5-> OUT = BIT6;
            contador = 1;
        } else {
            P5-> OUT = ~BIT6;
            contador = 0;
        }
        //ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
        __enable_irq();
        return;
    }

    void PORT1_IRQHandler (void)
        {
            __disable_irq();
            P1-> IFG = 0; // clear interrupt flag
            //P1->OUT ^= BIT0;
            OnOffLed();
            //P5-> OUT = BIT6;
            //ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
            __enable_irq();
            return;
        }

    void PORT3_IRQHandler (void)
        {
            __disable_irq();
            P3-> IFG = 0; // clear interrupt flag
            //P1->OUT ^= BIT0;
            OnOffLed();
            //P5-> OUT = BIT6;
            //ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
            __enable_irq();
            return;
        }

    /*
    void ADC14_IRQHandler(void)
    {
        __disable_irq();
        ADC14Result = ADC14->MEM[0];
        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
        __enable_irq();
        return;
    }
    */
}
