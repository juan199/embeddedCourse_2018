#include "Scheduler.hpp"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    m_pMailbox = Mailbox::getMailbox();
    for(int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pTask = (uintptr_t) 0; // Init to an invalid pointer
    }


    return;
}

// - The attach function, inserts the task into the schedule slots.
uint8_t Scheduler::attach(Task * i_pTask, TaskType i_enTaskType, TaskActive i_enTaskIsActive,uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    st_TaskInfo l_stTaskInfo;

    l_stTaskInfo.pTask = i_pTask;
    l_stTaskInfo.bTaskIsActive = i_enTaskIsActive;
    l_stTaskInfo.u64ticks = this->m_u64ticks;
    l_stTaskInfo.u64TickInterval = 0;
	l_stTaskInfo.u64TickIntervalInitValue = i_u64TickInterval;
	l_stTaskInfo.enTaskType = i_enTaskType;

    if((m_u8OpenSlots>0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] =  l_stTaskInfo;
        m_u8OpenSlots--;
        m_u8NextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}
// - Execute the current schedule
uint8_t Scheduler::run(void)
{
    uint8_t l_u8NextSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_u8NextSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_u8NextSlot].pTask);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            if(m_aSchedule[l_u8NextSlot].bTaskIsActive)
            {
                // - Execute the task slot
                if(m_aSchedule[l_u8NextSlot].u64TickInterval == 0)
                {
                    l_pNextTask->run();
                }

                // - Depending on the task type, handle the tick count
                switch(m_aSchedule[l_u8NextSlot].enTaskType)
                {
                    case(TaskType_OneShot):
                        m_aSchedule[l_u8NextSlot].bTaskIsActive = TaskActiveFalse;
                        // - Intentional fall-through
                    case(TaskType_Always):
                        m_aSchedule[l_u8NextSlot].u64TickInterval = 0;
                        break;
                    case(TaskType_Periodic):
                        m_aSchedule[l_u8NextSlot].u64TickInterval ++;
                        if(m_aSchedule[l_u8NextSlot].u64TickInterval > m_aSchedule[l_u8NextSlot].u64TickIntervalInitValue)
                        {
                            m_aSchedule[l_u8NextSlot].u64TickInterval = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            // - If we found an uninitialized slot, break from the execution loop
            break;
        }
		l_u8NextSlot++;
    }
    //CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}
// - Execute the setup function for all tasks
uint8_t Scheduler::setup(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;
    // - Run the setup function for all available tasks.
    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pTask);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }


    return l_u8ReturnCode;
}

uint8_t Scheduler::DefineDestination(Task * t_source, Task * t_destination){
    // UniqueADC.MessageADC.u8DestinationID = Angle.m_u8TaskID;
    t_source->st_taskMessage.u8DestinationID = t_destination->m_u8TaskID;
    return(NO_ERR);
}


uint8_t Scheduler::CalculateNextSchedule(void)
{
    return(NO_ERR);
}
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
