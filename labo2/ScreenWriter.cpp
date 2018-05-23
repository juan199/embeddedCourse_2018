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

        if((3 < m_i16currentAngle) && (m_i16currentAngle < 177))
        {
            BlueRect.yMin = 0;
            BlueRect.yMax = 127 + (m_i16currentAngle * -0.70555555);
            GreenRect.yMin = BlueRect.yMax + 1;
            GreenRect.yMax = 127;
        }
        else if ((183 < m_i16currentAngle) && (m_i16currentAngle< 357))
        {
            BlueRect.yMax = 127;
            BlueRect.yMin = 254 + (m_i16currentAngle *-0.70555555);
            GreenRect.yMin = 0;
            GreenRect.yMax = BlueRect.yMin - 1;

        }
        else if((m_i16currentAngle <= 3) || ((m_i16currentAngle >= 357)))
        {
            BlueRect.yMin = 0;
            BlueRect.yMax = 127;
            GreenRect.yMin = 128;
            GreenRect.yMax = 128;
        }
        else if((177 <= m_i16currentAngle) && (m_i16currentAngle <= 183))
        {
            BlueRect.yMin = 128;
            BlueRect.yMax = 128;
            GreenRect.yMin = 0;
            GreenRect.yMax = 127;
        }

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
        Graphics_fillRectangle(&g_sContext, &BlueRect);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
        Graphics_fillRectangle(&g_sContext, &GreenRect);

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





