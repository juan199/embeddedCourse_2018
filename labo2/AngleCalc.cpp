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
    st_taskMessage = getMessage(this->m_u8TaskID);
    if (st_taskMessage.bMessageValid == true)
    {

        m_iAngleResult = ((atan((double)st_taskMessage.i16MessageData1 / (double)*st_taskMessage.pPayload) * (180/PI)));
        if(m_iAngleResult > 0){
            m_iAngleResult = -1*(m_iAngleResult - 90);
        }
        else{
            m_iAngleResult = -1*(m_iAngleResult + 90);
        }
    }

    return(NO_ERR);
}


uint8_t AngleCalc::setup()
{
    return(NO_ERR);
}

