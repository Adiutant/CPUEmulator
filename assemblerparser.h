#ifndef ASSEMBLERPARSER_H
#define ASSEMBLERPARSER_H

#include <QString>
#include <QList>
#include <QMap>


const static QMap<QString,unsigned char> commandDictionary{
    {"ADD", 0x11},
    {"SUB", 0x12},
    {"INC", 0x13},
    {"DEC", 0x14},
    {"RD", 0x15},
    {"WR", 0x16},
    {"SAH", 0x17},
    {"SAL", 0x18},
    {"GTZ", 0x19},
    {"LOEZ", 0x20},
    {"SFGS", 0x21}, //signflagset
    {"MUL",0x22},
    {"WRH", 0x23},
    {"RDH",0x24},
    {"ADDA", 0x25}


};
const static QMap<QString, char> registersDictionary{
    {"EBX", 0x01},
    {"ECX",0x02}
};
class AssemblerParser
{
public:
    enum AddressType{
      Straight,
      Reference,
      Register
    };
    enum ParseError{
      UnknownCommand,
      InvalidAdress,
      InvalidConstant,
      SyntaxErr,
        OverflowErr,
        NoErr
    };
    struct ParseResult{
        unsigned char binary[255]{0};
        ParseError err = NoErr;
        int lineErr;
    };
    AssemblerParser();
    ParseResult makeBinary(const QStringList&);


private:

    struct LineParseResult{
        QList<unsigned char> binary;
        ParseError err = NoErr;
        int line;
    };
public:
    LineParseResult parseCommand(const QString&, int);

};

#endif // ASSEMBLERPARSER_H
