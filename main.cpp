#include "turingmachine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TuringMachine turingMachine;

    turingMachine.show();
    return a.exec();
}
