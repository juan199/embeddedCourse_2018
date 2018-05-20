#include "AngleCalc.hpp"
#include <math.h>
#define PI 3.14159265

//constructor
AngleCalc::AngleCalc()
{
    m_iAngleResult = 0U;
};

uint8_t AngleCalc::run() // por qué estamos usando uint8_t?
{
    st_taskMessage_Rx = getMessage(this->m_u8TaskID);
    if (st_taskMessage_Rx.bMessageValid == true)
    {

        m_iAngleResult = ((atan((double)st_taskMessage_Rx.i16MessageData1 / (double)*st_taskMessage_Rx.pPayload) * (180/PI)));
        if (*st_taskMessage_Rx.pPayload > 0) {
            m_iAngleResult = m_iAngleResult + 180;
        }
        st_taskMessage.bMessageValid = true;
        /*
        if(m_iAngleResult > 0){
            m_iAngleResult = -1*(m_iAngleResult - 90);
        }
        else{
            m_iAngleResult = -1*(m_iAngleResult + 90);
        }
        */


        st_taskMessage.u8SourceID = this->m_u8TaskID;
        //MessageADC.u8MessageCode =
        //MessageADC.u32MessageData =  m_i16AdcYResult; //(((double)m_i16AdcYResult / (double)m_i16AdcZResult));
        //st_taskMessage.pPayload = &m_i16AdcZResult;
        st_taskMessage.i16MessageData1 =  m_iAngleResult;
        //st_taskMessage.u8DestinationID = 4;
        sendMessage(st_taskMessage);
    }

    return(NO_ERR);
}


uint8_t AngleCalc::setup()
{
    return(NO_ERR);
}

