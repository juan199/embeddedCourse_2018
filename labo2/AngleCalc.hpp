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
        int16_t m_iAngleResult;
        st_Message MessageAngleCalc_Rx;
    protected:
    private:

};


#endif /* ANGLECALC_HPP_ */

