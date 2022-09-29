#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cpuHelper = new CPUStructure();
    asmParser = new AssemblerParser;
    CPUStructure::CPU cpuState = cpuHelper->initCPU();
    ui->registersTable->verticalHeader()->hide();
    QStringList list{"Регистер", "Значение"};
    ui->registersTable->setHorizontalHeaderLabels(list);
    connect(cpuHelper, &CPUStructure::CPUStateChanged, this, &MainWindow::onCPUStateChanged);

    ui->registersTable->insertColumn(0);
    ui->registersTable->insertColumn(1);
    ui->registersTable->setColumnWidth(0,ui->registersTable->width());
    ui->registersTable->setColumnWidth(1,ui->registersTable->width());
    for (int i =0; i < cpuState.registers.keys().length(); i++){
        ui->registersTable->insertRow(i);
        QString currentKey = cpuState.registers.keys()[i];
        for (int j = 0; j<2 ;j++){
            QTableWidgetItem *item = new QTableWidgetItem;
            if (j != 0){
                item->setText(QString::number(cpuState.registers[currentKey]));
            }
            else{
                item->setText(currentKey);
                item->setFlags(Qt::ItemIsEnabled);
            }
            ui->registersTable->setItem(i, j ,item);
        }
    }

    for (int i =0; i < 16; i ++){
        ui->memoryTable->insertColumn(i);
        ui->memoryTable->setColumnWidth(i,10);
    }

    for (int i =0; i <16; i++){
        ui->memoryTable->insertRow(i);
        for (int j = 0; j<16 ;j++){
            QTableWidgetItem *item = new QTableWidgetItem;
            QByteArray byte;
            byte.append(cpuState.RAM[i]);

            item->setText(QString("0x%1").arg(QString::fromLocal8Bit(byte.toHex())));
            item->setFlags(Qt::ItemIsEnabled);

            ui->memoryTable->setItem(i, j ,item);
        }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCPUStateChanged(const CPUStructure::CPU &cpuState)
{



    for (int i =0; i < cpuState.registers.keys().length(); i++){
        QString currentKey = cpuState.registers.keys()[i];
        for (int j = 0; j<2 ;j++){
            QTableWidgetItem *item = ui->registersTable->item(i,j);
            if (j != 0){
                item->setText(QString::number(cpuState.registers[currentKey]));
            }
            else{
                item->setText(currentKey);
                item->setFlags(Qt::ItemIsEnabled);
            }
        }
    }

    for (int i =0; i < 16; i ++){
            for (int j = 0; j<16 ;j++){
                qDebug() << i << " " <<j;
                QTableWidgetItem *item = ui->memoryTable->item(i,j);
                QByteArray byte;
                byte.append(cpuState.RAM[(i*16)+j]);

                item->setText(QString("0x%1").arg(QString::fromLocal8Bit(byte.toHex())));
                item->setFlags(Qt::ItemIsEnabled);

                //ui->memoryTable->setItem(i, j ,item);
            }
        }

}


void MainWindow::on_runButtton_clicked()
{
    AssemblerParser::ParseResult parseRes = asmParser->makeBinary(ui->programmText->toPlainText().split(';'));
    if (parseRes.err != AssemblerParser::ParseError::NoErr){
//        QTextBlockFormat fmt;
//        fmt.setBackground(Qt::red);

//        QTextCursor cursor(ui->programmText->document());
//        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, parseRes.lineErr);
//            cursor.select(QTextCursor::LineUnderCursor);
//            cursor.setBlockFormat(fmt);
//            errorOnLine = parseRes.lineErr;
        return;
    }
    cpuHelper->runProgramm(asmParser->makeBinary(ui->programmText->toPlainText().split(';')).binary);

}


void MainWindow::on_programmText_textChanged()
{
//    if (errorOnLine == -1){
//        return;
//    }
//    QTextBlockFormat fmt;
//    fmt.setBackground(Qt::transparent);

//    QTextCursor cursor(ui->programmText->document());
//    cursor.movePosition(QTextCursor::Start);
//    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, errorOnLine);
//        cursor.select(QTextCursor::LineUnderCursor);
//        cursor.setBlockFormat(fmt);
//        errorOnLine = -1;

}



