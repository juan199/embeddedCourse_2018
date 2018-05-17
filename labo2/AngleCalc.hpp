#ifndef ANGLECALC_HPP_
#define ANGLECALC_HPP_
#define __NOP __nop // qué hace esto?
#include "msp.h"
#include "Task.hpp"

class AngleCalc : public Task
{
    public:
        AngleCalc();
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:
        int16_t m_iAngleResult;
};


#endif /* ANGLECALC_HPP_ */

