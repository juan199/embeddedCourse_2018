#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#include "msp.h"
#include "Task.hpp"
#include "Mailbox.hpp"
#include <stdlib.h>

#define NUMBER_OF_SLOTS 255
#define NULL            0

enum TaskType
{
    TaskType_OneShot = 0,
    TaskType_Always,
    TaskType_Periodic
};

enum TaskActive
{
    TaskActiveTrue = true,
    TaskActiveFalse = false
};

// - This structure defines the Task Information
struct st_TaskInfo {
	Task * pTask;                       // - Pointer to the Task
	TaskType enTaskType;                // - Task type
	bool bTaskIsActive;                 // - True when the task is available
	uint64_t u64TickInterval;           // - How often the task is executed
	uint64_t u64ticks;                  // - Current tick count
	uint64_t u64TickIntervalInitValue;  // - Value to reset
};



class Scheduler
{
public:
    Scheduler();
    uint64_t m_u64ticks;
    uint8_t attach(Task * i_pTask, TaskType i_enType, TaskActive i_enTaskActive, uint64_t i_u64TickInterval = 0U);
    uint8_t run(void);
    uint8_t setup(void);
    uint8_t DefineDestination(Task * t_source, Task * t_destination); // Function that attaches destinations
private:
    uint8_t m_u8OpenSlots; // - Available slots
    uint8_t m_u8NextSlot;  // - Next available slot
    uint8_t m_u8TaskCount; // - Number of tasks attached
    st_TaskInfo m_aSchedule[NUMBER_OF_SLOTS]; // - Current schedule to be executed
    st_TaskInfo m_aNextSchedule[NUMBER_OF_SLOTS]; // - Next schedule to be executed (not implemented)
    uint8_t CalculateNextSchedule(void); // - Calculate next schedule tasks (not implemented)
    uint8_t SortScheduleByPriority(Task * i_pSchedule); // - Sorts a schedule based on priority (not implemented)
    uintptr_t m_pNextSchedule; // - Pointer to the next schedule.
    Mailbox* m_pMailbox;
};



#endif /* TASKS_SCHEDULER_HPP_ */
