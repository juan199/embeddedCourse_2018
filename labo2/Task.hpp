#ifndef TASK_HPP_
#define TASK_HPP_

#include "msp.h"
#include "Mailbox.hpp"

#define NO_ERR 0
#define RET_ERR 1

class Task
{
	public:
		Task();
		uint8_t m_u8TaskID;
		virtual uint8_t     run(void){return(0);};
		virtual uint8_t     setup(void){return(0);};
		bool                IsTaskFinished(void){return m_bIsFinished;};
		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
		st_Message          getMessage(uint8_t i_u8TaskID);
		st_Message          st_taskMessage;
		st_Message          st_taskMessage_Rx;
		bool                sendMessage(st_Message i_stMessage);

	private:
	   static uint8_t m_u8NextTaskID;
	   uint8_t m_u8Priority;
	   Mailbox* m_pMailbox;
	protected:
	   bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
