#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Backend/ExampleBackend.h"


class ExampleBackend;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initializePlot();
    void initializeWaterfall();
    void initializeSyncPlots();

    void initializePlot(QtPlot* plot, QString plotName);

signals:

private:
    Ui::MainWindowClass ui;
    ExampleBackend      backend;
};
