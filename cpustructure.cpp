#include "cpustructure.h"

CPUStructure::CPUStructure()
{
    cpuInstance = new CPU();
    tickTimer = new QTimer();


}

const CPUStructure::CPU &CPUStructure::initCPU()
{
    return *cpuInstance;
}

void CPUStructure::runProgramm(unsigned char * bin)
{
    for (auto key :cpuInstance->registers.keys()){
        cpuInstance->registers[key]= 0;
    }

    for (int i = 0; i <255;i++){
        cpuInstance->RAM[i] = bin[i];
    }
    emit CPUStateChanged(*cpuInstance);
    connect(tickTimer,&QTimer::timeout,this,&CPUStructure::onCPUTick);
    tickTimer->start(200);

}

void CPUStructure::onCPUTick()
{
    unsigned char currentCommand = cpuInstance->RAM[cpuInstance->registers["cnt"]];
    unsigned char address = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
    switch (currentCommand) {
        case 0x11:
            cpuInstance->registers["acc"] += cpuInstance->RAM[address];
        break;
        case 0x12:
            cpuInstance->registers["acc"] -= cpuInstance->RAM[address];
        break;
        case 0x13:
            cpuInstance->registers["acc"] += 1;
        break;
        case 0x14:
            cpuInstance->registers["acc"] -= 1;
        break;
    }
    if ( cpuInstance->registers["cnt"] == 254){
         cpuInstance->registers["cnt"] = 0;
         disconnect(tickTimer,&QTimer::timeout,this,&CPUStructure::onCPUTick);
         tickTimer->stop();
    }
    else{
        cpuInstance->registers["cnt"]+=2;
    }
    emit CPUStateChanged(*cpuInstance);

}
