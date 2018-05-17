#include "AngleCalc.hpp"
#include <math.h>
//#define PI 3.14159265
#include "global.hpp"
//#include "main.cpp"

//constructor
// hay algo que está mal, porque el constructor no
// debería tener que revisar NADA en esta clase
AngleCalc::AngleCalc(uint16_t i_BITN)
{
    m_u16BITNNN = i_BITN;
};

uint8_t AngleCalc::run() // por qué estamos usando uint8_t?
{
    //double result = (double)g_i16AdcYResult / (double)g_i16AdcZResult;
    //AngleCalc::get_variables(int16_t adcX, int16_t adcY, int16_t adcZ);
    g_iAngleResult = ((atan((double)g_i16AdcYResult / (double)g_i16AdcZResult)) * (180/3.14159265));
    //double prueba = atan(0.5);
    if(g_iAngleResult > 0){
        g_iAngleResult = g_iAngleResult - 90;
    }
    else{
        g_iAngleResult = g_iAngleResult + 90;
    }

            /*
            int16_t g_i16AdcXResult = 0U;
            int16_t g_i16AdcYResult = 0U;
            int16_t g_i16AdcZResult = 0U;
            */
    return(NO_ERR);
}

/*
uint8_t AngleCalc::get_variables(int16_t adcX, int16_t adcY, int16_t adcZ)
{
    m_i16AdcXResult = adcX;
    m_i16AdcYResult = adcY;
    m_i16AdcZResult = adcZ;
    return(NO_ERR);
}
*/

uint8_t AngleCalc::setup()
{
    return(NO_ERR);
}

