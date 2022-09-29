#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include "stdlib.h"
#include "cpustructure.h"
#include "assemblerparser.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onCPUStateChanged(const CPUStructure::CPU&);
    void on_runButtton_clicked();

    void on_programmText_textChanged();


private:
    CPUStructure *cpuHelper;
    int errorOnLine = -1;
    AssemblerParser *asmParser;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
