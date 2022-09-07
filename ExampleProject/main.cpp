
#include <QApplication>
#include "Frontend/MainWindow/MainWindow.h"
#include "Backend/ExampleBackend.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
	MainWindow w;

    w.setWindowTitle("QtPlot Example");
    w.show();

    return app.exec();
}
