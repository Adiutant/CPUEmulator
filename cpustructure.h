#ifndef CPUSTRUCTURE_H
#define CPUSTRUCTURE_H
#include <QMap>
#include <QString>
#include <stdint.h>
class CPUStructure
{
    static QMap<QString,unsigned char> commandDictionary{
        "ADD",
    };
    struct CPU{
        uint32_t acc;
        uint32_t cnt;
        unsigned char cmd;
        unsigned char RAM[1024];
    };

public:
    CPUStructure();
};

#endif // CPUSTRUCTURE_H
