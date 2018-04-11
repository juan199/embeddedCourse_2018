#include "msp.h"
/**
 * main.cpp
 */

   void DelayMs () {                // Aproximately 1 s
        int l_iCOUNTER = 0;         // Check variable's name
        while (l_iCOUNTER < 50000)
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

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer

    //  P5.6 Blue, P2.4 Green, P2.6 RED
    P5-> OUT = ~BIT6;
    P2-> OUT = ~(BIT4 | BIT6); // preguntar al profe que porqué P2OUT empieza sucio, siempre entonces es buena
    // práctica limpiar las salidas?
    P5-> DIR = BIT6;
    P2-> DIR = BIT4 | BIT6;


    BlinkSetUp();

    //P4.6 J8, P6.5 J9 y P6.4 J10 LigthSensor
    // p4.6 j8 GPIO
    //p6.5 j9 I2C
    //p6.4 j10 i2C

    // Para el sensor de luz
    P6-> DIR = 0;
    P4-> DIR = 0;

    // Para que reciban varas de periféricos
    P6-> SEL1 = 1;
    P4-> SEL1 = 1;

    // Para habilitar escritura
    PMAP-> KEYID = 0x02D52;
    //PMAP-> P1 = 0;





    int counter = 0;
    while(true){
        counter++;
    }

}
