/*
 * ScreenWriter.cpp
 *
 *  Created on: May 19, 2018
 *      Author: sama
 */
#include "ScreenWriter.hpp"
extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
}



//constructor
ScreenWriter::ScreenWriter()
{
    m_i16lastAngle = 0;
    m_i16currentAngle = 0;
    resultado1 = 0;
    resultado2 = 0;
    resultado3 = 0;
    GreenRect.xMax = 127;
    GreenRect.xMin = 0;
    BlueRect.xMax = 127;
    BlueRect.xMin = 0;
};

uint8_t ScreenWriter::run()
{
    st_taskMessage = getMessage(this->m_u8TaskID);
    if (st_taskMessage.bMessageValid == true)
    {
        m_i16currentAngle = st_taskMessage.i16MessageData1;
        //resultado2 = m_i16currentAngle + 90;
        resultado1 = m_i16currentAngle - m_i16lastAngle;
        m_i16lastAngle = m_i16currentAngle;

        if (abs(resultado1) > 1)
        {
            BlueRect.yMin = 0;
            BlueRect.yMax = 127 + (m_i16currentAngle * -0.70555555);
            GreenRect.yMin = BlueRect.yMax + 1;
            GreenRect.yMax = 127;
        }

        /*
        if(resultado1 > 0){
            if (m_i16currentAngle > 0) {
                BlueRect.yMin = 0;
                BlueRect.yMax = 63 + (m_i16currentAngle * 0.7);
                GreenRect.yMin = BlueRect.yMax + 1;
                GreenRect.yMax = 127;
            } else {
                BlueRect.yMin = 0;
                BlueRect.yMax = (90-(-1*m_i16currentAngle) * 0.7);
                GreenRect.yMin = BlueRect.yMax + 1;
                GreenRect.yMax = 127;
            }
        } else if (resultado1 < 0) {
            if (m_i16currentAngle > 0) {
                BlueRect.yMin = 0;
                BlueRect.yMax = 63 + (m_i16currentAngle * 0.7);
                GreenRect.yMin = BlueRect.yMax + 1;
                GreenRect.yMax = 127;
            } else {
                BlueRect.yMin = 0;
                BlueRect.yMax = (90-(-1*m_i16currentAngle) * 0.7);
                GreenRect.yMin = BlueRect.yMax+1;
                GreenRect.yMax = 127;
            }
        }*/
        //Graphics_clearDisplay(&g_sContext);
        //Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
        Graphics_fillRectangle(&g_sContext, &BlueRect);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
        Graphics_fillRectangle(&g_sContext, &GreenRect);
        //Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_drawLineH(&g_sContext, 0, 127, BlueRect.yMax);
    }



    return(NO_ERR);
}


uint8_t ScreenWriter::setup()
{
    /* Initializes display */
    Crystalfontz128x128_Init();
    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    Graphics_clearDisplay(&g_sContext);
    return(NO_ERR);
}





