#include <QCoreApplication>

#include "event-monitor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    EventMonitor em;
    em.startMonitor();

    return a.exec();
}
