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

signals:

private:
    Ui::MainWindowClass ui;
    ExampleBackend      backend;
};
