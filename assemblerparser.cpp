#include "assemblerparser.h"

AssemblerParser::AssemblerParser()
{

}

AssemblerParser::ParseResult AssemblerParser::makeBinary(const QStringList & programm)
{
    ParseResult res;
    int counter = 0;
    for (int i = 0; i < programm.length(); i++ ){
        if (programm.at(i).isEmpty()){
            continue ;
        }
        LineParseResult lRes = parseCommand(programm.at(i), i);
        if (lRes.err != NoErr){
            res.err = lRes.err;
            res.lineErr = lRes.line;
            return res;
        }
        if (counter + 3 >= 255 ){
            res.err = OverflowErr;
            res.lineErr = lRes.line;
            return res;
        }
        res.binary[counter] = lRes.binary.at(0);
        res.binary[counter + 1] = lRes.binary.at(1);
        res.binary[counter + 2] = lRes.binary.at(2);
        counter+=3;
    }
    return res;

}

AssemblerParser::LineParseResult AssemblerParser::parseCommand(const QString & line,int lineNum)
{

    LineParseResult result;
    QList<unsigned char> binaryLine;
    QStringList lineArgs = line.trimmed().split(' ');
    if (!commandDictionary.contains(lineArgs[0])){
        result.binary = binaryLine;
        result.err = UnknownCommand;
        result.line = lineNum;
        return result;
    }
    binaryLine.append(commandDictionary[lineArgs[0]]);
    if (lineArgs[1] >= 255){
        result.binary = binaryLine;
        result.err = InvalidAdress;
        result.line = lineNum;
        return result;
    }
    binaryLine.append(lineArgs[1].toUInt());
    if (lineArgs[2] >= 255){
        result.binary = binaryLine;
        result.err = InvalidConstant;
        result.line = lineNum;
        return result;
    }
    binaryLine.append(lineArgs[2].toUInt());
    result.binary = binaryLine;
    result.err = NoErr;
    result.line = lineNum;
    return result;


}
