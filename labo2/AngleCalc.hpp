#ifndef ANGLECALC_HPP_
#define ANGLECALC_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"

class AngleCalc : public Task
{
    public:
        AngleCalc();
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
        int16_t m_iAngleResult;
    protected:
    private:

};


#endif /* ANGLECALC_HPP_ */

