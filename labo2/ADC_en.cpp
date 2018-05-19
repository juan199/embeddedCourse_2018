#include "ADC_en.hpp"

//constructor
// hay algo que está mal, porque el constructor no
// debería tener que revisar NADA en esta clase

ADC_en::ADC_en()
{
    m_i16AdcXResult = 0;
    m_i16AdcYResult = 0;
    m_i16AdcZResult = 0;
    m_iTickCount = 0;
};

uint8_t ADC_en::run() // por qué estamos usando uint8_t?
{
    // manda a correr al ADC
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC;

    if (m_iTickCount == 32)
    {

        st_taskMessage.bMessageValid = true;
        //MessageADC.u8DestinationID =
        st_taskMessage.u8SourceID = this->m_u8TaskID;
        //MessageADC.u8MessageCode =
        //MessageADC.u32MessageData =  m_i16AdcYResult; //(((double)m_i16AdcYResult / (double)m_i16AdcZResult));
        st_taskMessage.pPayload = &m_i16AdcZResult;
        st_taskMessage.i16MessageData1 =  m_i16AdcYResult;
        st_taskMessage.i16MessageData2 =  m_i16AdcZResult;
        sendMessage(st_taskMessage);
        m_iTickCount = 0;
    } else {
        m_iTickCount++;
    }

    return(NO_ERR);
}

uint8_t ADC_en::setup()
{
    // configura el ADC
    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
                  ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON
                  | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_3; // not diving the ADC's clk | using ADC pin to sample and hold
                                    // time to sample | turn the converter on | ADC14_CTL0_CONSEQ_1 para que lea los 32 canales
    ADC14->CTL1 = ADC14_CTL1_DF;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0; // X | VCC & VSS
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0; // Y | VCC & VSS
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0; // Z | VCC & VSS
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC; //ADC14 Conversion Enable
    ADC14->IER0 = ADC14_IER0_IE0 | ADC14_IER0_IE1 | ADC14_IER0_IE2;   // Enables ADC14's interrupt

    NVIC_SetPriority(ADC14_IRQn,1); // Interrupt Priority
    NVIC_EnableIRQ(ADC14_IRQn);     // Enable Interrupt Queue
    return(NO_ERR);
}

