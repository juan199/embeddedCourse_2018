/*
 * ADC_en.hpp
 *
 *  Created on: May 10, 2018
 *      Author: sama
 */

#ifndef ADC_EN_HPP_
#define ADC_EN_HPP_
#define __NOP __nop // qué hace esto?
#include "msp.h"
#include "Task.hpp"

class ADC_en : public Task
{
    public:
        ADC_en();
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
        //st_Message MessageADC;
        int16_t m_i16AdcXResult;
        int16_t m_i16AdcYResult;
        int16_t m_i16AdcZResult;
        int m_iTickCount;
    protected:
    private:

};


#endif /* ADC_EN_HPP_ */

