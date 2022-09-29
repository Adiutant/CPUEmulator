#ifndef CPUSTRUCTURE_H
#define CPUSTRUCTURE_H
#include <QMap>
#include <QString>
#include <stdint.h>
#include <QObject>


class CPUStructure : public QObject
{
    Q_OBJECT
public:
    struct CPU{
        QMap<QString, uint32_t > registers{
            {"acc",0},
            {"cnt",0}
        };
        unsigned char cmd = 0;
        unsigned char RAM[255];
    };
    CPUStructure();
    const CPUStructure::CPU& initCPU();

    void runProgramm(unsigned char[]);

private:
    CPU *cpuInstance;


signals:

    void CPUStateChanged(const CPUStructure::CPU&);
};

#endif // CPUSTRUCTURE_H
