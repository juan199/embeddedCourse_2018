#ifndef ANGLECALC_HPP_
#define ANGLECALC_HPP_
#define __NOP __nop // qué hace esto?
#include "msp.h"
#include "Task.hpp"

class AngleCalc : public Task
{
    public:
        AngleCalc(uint16_t);
        uint16_t m_u16BITNNN;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:

};


#endif /* ANGLECALC_HPP_ */

