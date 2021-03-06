#include "AngleCalc.hpp"
#include <math.h>
#define PI 3.14159265

// - AngleCalc constructor
AngleCalc::AngleCalc()
{
    m_iAngleResult = 0U;
};

uint8_t AngleCalc::run()
{
    // Y and Z data is received. The angle is calculated using atan function
    st_taskMessage_Rx = getMessage(this->m_u8TaskID);
    if (st_taskMessage_Rx.bMessageValid == true)
    {

        //
        if((*st_taskMessage_Rx.pPayload < 0)  && (st_taskMessage_Rx.i16MessageData1 < 0))
        {
            m_iAngleResult = ((atan((double)st_taskMessage_Rx.i16MessageData1 / (double)*st_taskMessage_Rx.pPayload) * (180/PI)));
        }
        else if((*st_taskMessage_Rx.pPayload > 0)  && (st_taskMessage_Rx.i16MessageData1 < 0))
        {
            m_iAngleResult = -1*((atan((double)*st_taskMessage_Rx.pPayload /(double)st_taskMessage_Rx.i16MessageData1) * (180/PI))) + 90;
        }
        else if((*st_taskMessage_Rx.pPayload > 0)  && (st_taskMessage_Rx.i16MessageData1 > 0))
        {
            m_iAngleResult = ((atan((double)st_taskMessage_Rx.i16MessageData1 / (double)*st_taskMessage_Rx.pPayload) * (180/PI))) + 180;
        }
        else if ((*st_taskMessage_Rx.pPayload < 0)  && (st_taskMessage_Rx.i16MessageData1 > 0))
        {
            m_iAngleResult = -1*((atan((double)*st_taskMessage_Rx.pPayload /(double)st_taskMessage_Rx.i16MessageData1) * (180/PI))) + 270;
        }

        st_taskMessage.bMessageValid = true;
        st_taskMessage.u8SourceID = this->m_u8TaskID;
        //MessageADC.u8MessageCode =
        //MessageADC.u32MessageData =
        //st_taskMessage.pPayload =
        st_taskMessage.i16MessageData1 =  m_iAngleResult;
        //st_taskMessage.u8DestinationID =
        sendMessage(st_taskMessage);
    }

    return(NO_ERR);
}


uint8_t AngleCalc::setup()
{
    return(NO_ERR);
}

