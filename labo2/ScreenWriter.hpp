/*
 * ScreenWriter.hpp
 *
 *  Created on: May 19, 2018
 *      Author: sama
 */

#ifndef SCREENWRITER_HPP_
#define SCREENWRITER_HPP_
#define __NOP __nop // qué hace esto?
#include "msp.h"
#include "Task.hpp"

extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
}

class ScreenWriter : public Task
{
    public:
        ScreenWriter();
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
        Graphics_Context g_sContext;
        //void drawTitle(void);
        Graphics_Rectangle GreenRect;
        Graphics_Rectangle BlueRect;
    protected:
    private:
        int16_t m_i16lastAngle;
        int16_t m_i16currentAngle;
        int16_t resultado1;
        int16_t resultado2;
        int16_t resultado3;


};

#endif /* SCREENWRITER_HPP_ */

