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
        QRegExp match("\\w+ \\$?\\d+ *;$");
        if (!match.exactMatch(programm.at(i))){
            res.err = SyntaxErr;
            res.lineErr = i;
            return res;
        }
        LineParseResult lRes = parseCommand(programm.at(i), i);
        if (lRes.err != NoErr){
            res.err = lRes.err;
            res.lineErr = lRes.line;
            return res;
        }
        if (counter + 2 >= 255 ){
            res.err = OverflowErr;
            res.lineErr = lRes.line;
            return res;
        }
        res.binary[counter] = lRes.binary.at(0);
        res.binary[counter + 1] = lRes.binary.at(1);
//        res.binary[counter + 2] = lRes.binary.at(2);
        counter+=2;
    }
    return res;

}

AssemblerParser::LineParseResult AssemblerParser::parseCommand(const QString & line,int lineNum)
{

    LineParseResult result;
    QList<unsigned char> binaryLine;
    QStringList lineArgs = line.trimmed().split(' ');
    AddressType addrType = Straight;
    if (!commandDictionary.contains(lineArgs[0])){
        result.binary = binaryLine;
        result.err = UnknownCommand;
        result.line = lineNum;
        return result;
    }

    binaryLine.append(commandDictionary[lineArgs[0]]);
    QRegExp refregexp("(\\$)(\\d*)");
    QRegExp strregexp("\\d+");
    if (refregexp.indexIn(lineArgs[1]) == -1)
    {
        if (strregexp.indexIn(lineArgs[1]) == -1){
            result.binary = binaryLine;
            result.err = InvalidAdress;
            result.line = lineNum;
            return result;
        }else if (strregexp.cap(1).toInt() >= 255){
            result.binary = binaryLine;
            result.err = InvalidAdress;
            result.line = lineNum;
            return result;
        }
    }
    else  if (refregexp.cap(1) == "$"){
        addrType = Reference;
    }
    else
    {
        result.binary = binaryLine;
        result.err = SyntaxErr;
        result.line = lineNum;
        return result;

    }






    if (addrType == Reference)
    {
        binaryLine[0] = binaryLine[0] << 2;
    }
    for (auto &ch : lineArgs[1]){
        if (!ch.isDigit())
        {
            lineArgs[1].remove(ch);
        }
    }
    binaryLine.append(lineArgs[1].toUInt());
//    if (lineArgs[2] >= 255){
//        result.binary = binaryLine;
//        result.err = InvalidConstant;
//        result.line = lineNum;
//        return result;
//    }
//    binaryLine.append(lineArgs[2].toUInt());
    result.binary = binaryLine;
    result.err = NoErr;
    result.line = lineNum;
    return result;


}
