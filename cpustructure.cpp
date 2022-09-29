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

    for (int i = 0; i <256;i++){
        cpuInstance->RAM[i] = bin[i];
    }
    emit CPUStateChanged(*cpuInstance);
    if(tickTimer->isActive())
    {
        cpuInstance->registers["cnt"] = 0;
        disconnect(tickTimer,&QTimer::timeout,this,&CPUStructure::onCPUTick);
        tickTimer->stop();

    }
    connect(tickTimer,&QTimer::timeout,this,&CPUStructure::onCPUTick);
    tickTimer->start(20);

}

void CPUStructure::onCPUTick()
{
    emit memoryFocus(cpuInstance->registers["cnt"]);
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
        case 0x15:
            cpuInstance->registers["acc"] = cpuInstance->RAM[address];
        break;
        case 0x16:
             cpuInstance->RAM[address] = cpuInstance->registers["acc"];
        break;
        case 0x17:
        //fix
             cpuInstance->registers["acc"] = ((uint32_t) cpuInstance->RAM[cpuInstance->registers["cnt"]+1])<<8;
        break;
        case 0x18:
             cpuInstance->registers["acc"] = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];

        break;
        case 0x19:
            if (cpuInstance->registers["acc"] > 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
            }

        break;
        case 0x20:
            if (cpuInstance->registers["acc"] <= 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
            }

        break;
        case 0x11 << 2:
            cpuInstance->registers["acc"] += cpuInstance->RAM[ cpuInstance->RAM[address]];
        break;
        case 0x12 << 2:
            cpuInstance->registers["acc"] -= cpuInstance->RAM[cpuInstance->RAM[address]];
        break;
        case 0x15 << 2:
            cpuInstance->registers["acc"] = cpuInstance->RAM[cpuInstance->RAM[address]];
        break;
        case 0x16 << 2:
             cpuInstance->RAM[cpuInstance->RAM[address]] = cpuInstance->registers["acc"];
        break;
        case 0x19 << 2:
            if (cpuInstance->registers["acc"] > 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->RAM[cpuInstance->registers["cnt"]+1]];
            }

        break;
        case 0x20 << 2:
            if (cpuInstance->registers["acc"] <= 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->RAM[cpuInstance->registers["cnt"]+1]];
            }

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
