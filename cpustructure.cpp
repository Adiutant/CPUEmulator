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

void CPUStructure::compile(unsigned char * bin )
{
    for (auto key :cpuInstance->registers.keys()){
        cpuInstance->registers[key]= 0;
    }

    for (int i = 0; i <256;i++){
        cpuInstance->RAM[i] = bin[i];
    }
    emit CPUStateChanged(*cpuInstance);
}

void CPUStructure::onCPUTick()
{
    emit memoryFocus(cpuInstance->registers["cnt"]);
    bool jmp=false;
    unsigned char currentCommand = cpuInstance->RAM[cpuInstance->registers["cnt"]];
    unsigned char address = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
    uint32_t x, y;
    switch (currentCommand) {
        case 0x11:
            cpuInstance->registers["acc"] += cpuInstance->RAM[address];
        break;
        case 0x12:
            if (cpuInstance->registers["acc"] < cpuInstance->RAM[address] )
            {
                cpuInstance->registers["sgf"] = 1;
            }
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
                jmp  = true;
            }

        break;
        case 0x20:
            if (cpuInstance->registers["acc"] <= 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
                jmp  = true;
            }

        break;
        case 0x21:
            if (cpuInstance->registers["sgf"] > 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->registers["cnt"]+1];
                cpuInstance->registers["sgf"] = 0;
                jmp  = true;
            }

        break;
        case 0x22:
            cpuInstance->registers["EBX"] = (unsigned short)((uint32_t)(cpuInstance->registers["acc"] * cpuInstance->RAM[address]) >> 16);
            cpuInstance->registers["acc"] = (unsigned short) ((uint32_t)cpuInstance->registers["acc"] * cpuInstance->RAM[address]);
        break;
    case 0x23:
        cpuInstance->RAM[address] = cpuInstance->registers["acc"] >> 8;
    case 0x24:
        cpuInstance->registers["acc"] &= 0x00FF;
        cpuInstance->registers["acc"] |=  cpuInstance->RAM[address]<<8;
    break;
    case 0x25:
        x= ((uint8_t)cpuInstance->RAM[address+3] << 24) | ((uint8_t)cpuInstance->RAM[address+2] << 16) | ((uint8_t)cpuInstance->RAM[address+1] << 8) | (uint8_t)cpuInstance->RAM[address];;
         y = ((uint16_t)cpuInstance->registers["acc"]) | ((uint16_t)cpuInstance->registers["EBX"] <<16);

        cpuInstance->registers["acc"] = (x + y);
        cpuInstance->registers["EBX"] = (x+y) >>16;
    break;
        case 0x11 +0x20:
            cpuInstance->registers["acc"] += cpuInstance->RAM[ cpuInstance->RAM[address]];
        break;
        case 0x12 +0x20:
            if (cpuInstance->registers["acc"] < cpuInstance->RAM[cpuInstance->RAM[address]] )
            {
                cpuInstance->registers["sgf"] = 1;
            }
            cpuInstance->registers["acc"] -= cpuInstance->RAM[cpuInstance->RAM[address]];
        break;
        case 0x15 +0x20:
            cpuInstance->registers["acc"] = cpuInstance->RAM[cpuInstance->RAM[address]];
        break;
        case 0x16 +0x20:
             cpuInstance->RAM[cpuInstance->RAM[address]] = cpuInstance->registers["acc"];
        break;
        case 0x19 +0x20:
            if (cpuInstance->registers["acc"] > 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->RAM[cpuInstance->registers["cnt"]+1]];
                jmp  = true;
            }

        break;
        case 0x20 +0x20:
            if (cpuInstance->registers["acc"] <= 0){
                cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->RAM[cpuInstance->registers["cnt"]+1]];
                jmp  = true;
            }

        break;

    case 0x21 +0x20:
        if (cpuInstance->registers["sgf"] > 0){
            cpuInstance->registers["cnt"] = cpuInstance->RAM[cpuInstance->RAM[cpuInstance->registers["cnt"]+1]];
            cpuInstance->registers["sgf"] = 0;
            jmp  = true;
        }
        break;
    case 0x22+0x20:
        cpuInstance->registers["EBX"] = (unsigned short)((uint32_t)(cpuInstance->registers["acc"] * cpuInstance->RAM[cpuInstance->RAM[address]]) >> 16);
        cpuInstance->registers["acc"] = (unsigned short) ((uint32_t)cpuInstance->registers["acc"] * cpuInstance->RAM[cpuInstance->RAM[address]]);
    break;

        //reg
    case 0x11 +0x40:
        cpuInstance->registers["acc"] += cpuInstance->registers[registersDecode[address]];
    break;
    case 0x12 +0x40:
        if (cpuInstance->registers["acc"] < cpuInstance->registers[registersDecode[address]] )
        {
            cpuInstance->registers["sgf"] = 1;
        }
        cpuInstance->registers["acc"] -= cpuInstance->registers[registersDecode[address]];
    break;
    case 0x15 +0x40:
        cpuInstance->registers["acc"] = cpuInstance->registers[registersDecode[address]];
    break;
    case 0x16 +0x40:
         cpuInstance->registers[registersDecode[address]] = cpuInstance->registers["acc"];
    break;
    case 0x19 +0x40:
        if (cpuInstance->registers["acc"] > 0){
            cpuInstance->registers["cnt"] = cpuInstance->registers[registersDecode[address]];
            jmp  = true;
        }

    break;
    case 0x20 +0x40:
        if (cpuInstance->registers["acc"] <= 0){
            cpuInstance->registers["cnt"] = cpuInstance->registers[registersDecode[address]];
            jmp  = true;
        }

    break;
case 0x21 +0x40:
    if (cpuInstance->registers["sgf"] > 0){
        cpuInstance->registers["cnt"] = cpuInstance->registers[registersDecode[address]];
        cpuInstance->registers["sgf"] = 0;
        jmp  = true;
    }
    break;
    case 0x22 +0x40:
        cpuInstance->registers["EBX"] = (unsigned short)(((uint32_t)cpuInstance->registers["acc"] * cpuInstance->registers[registersDecode[address]]) >> 16);
        cpuInstance->registers["acc"] = (unsigned short) ((uint32_t)cpuInstance->registers["acc"] * cpuInstance->registers[registersDecode[address]]);
    break;
    }
    if ( cpuInstance->registers["cnt"] == 254){
         cpuInstance->registers["cnt"] = 0;
         disconnect(tickTimer,&QTimer::timeout,this,&CPUStructure::onCPUTick);
         tickTimer->stop();
    }
    else if(!jmp){
        cpuInstance->registers["cnt"]+=2;
    }

    emit CPUStateChanged(*cpuInstance);

}
