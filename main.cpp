#include "entranceguide.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EntranceGuide w;
    w.show();

    return a.exec();
}
