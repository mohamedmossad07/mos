#include <multitasking.h>

using namespace mos;
using namespace mos::common;
Task::Task(GlobalDescriptorTable * gdt, uint32_t entry){
    cpuState = (CPUState*) (stack+4096-sizeof(CPUState));
    cpuState -> eax = 0;
    cpuState -> ebx = 0;
    cpuState -> ecx = 0;
    cpuState -> edx = 0;
    cpuState -> esi = 0;
    cpuState -> edi = 0;
    cpuState -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpuState -> eip = entry;
    cpuState -> cs = gdt->CodeSegmentSelector();
    cpuState -> eflags = 0x202;
    // cpustate -> ss = ;
}
Task::~Task(){

}
TaskManager::TaskManager():
numTasks(0),
currentTask(-1)
{

}
TaskManager::~TaskManager(){

}
bool TaskManager::addTask(Task * task){
 if(numTasks>=256)return false;
 tasks[numTasks++]=task;
 return true;    
}
CPUState * TaskManager::schedule(GlobalDescriptorTable* gdt,CPUState * cpu){
    if(numTasks <= 0)
        return cpu;

//    if(currentTask==-1){
//     Task t(gdt,cpu->eip);
//     tasks[++numTasks]=&t;
//    }
    if(currentTask >= 0)
        tasks[currentTask]->cpuState = cpu;
   
    if(++currentTask >= numTasks)
        currentTask %= numTasks;

    return tasks[currentTask]->cpuState;
}  