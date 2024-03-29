#ifndef CPUSTRUCTURE_H
#define CPUSTRUCTURE_H
#include <QMap>
#include <QString>
#include <stdint.h>
#include <QObject>
#include <QTimer>
#include <QStack>
#include "assemblerparser.h"

const static QMap<char,QString> registersDecode{
    { 0x01,"EBX"},
    {0x02,"ECX"}
};
class CPUStructure : public QObject
{

    Q_OBJECT
public:

    struct CPU{
        QMap<QString, uint16_t > registers{
            {"acc",0},
            {"cnt",0},
            {"EBX",0},
            {"ECX",0},
            {"sgf",0}  //signflag
        };

        unsigned char cmd = 0;
        unsigned char RAM[256];
    };
    CPUStructure();
    const CPUStructure::CPU& initCPU();

    void runProgramm(unsigned char*);
    void compile(unsigned char*);
public slots:
    void onCPUTick();

private:
    CPU *cpuInstance;
    QStack<unsigned char> *stack;
    QTimer *tickTimer;


signals:

    void CPUStateChanged(const CPUStructure::CPU&);
    void memoryFocus(uint32_t);
};

#endif // CPUSTRUCTURE_H
