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
        ADC_en(uint16_t);
        uint16_t m_u16BITNN;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:

};


#endif /* ADC_EN_HPP_ */

