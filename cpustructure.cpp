#include "cpustructure.h"

CPUStructure::CPUStructure()
{
    cpuInstance = new CPU();

}

const CPUStructure::CPU &CPUStructure::initCPU()
{
    return *cpuInstance;
}

void CPUStructure::runProgramm(unsigned char * bin)
{
    for (int i = 0; i <255;i++){
        cpuInstance->RAM[i] = bin[i];
    }
    emit CPUStateChanged(*cpuInstance);


}
