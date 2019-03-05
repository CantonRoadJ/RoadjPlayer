#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  //  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);

    a.setOrganizationName("Roadj"); //1
    a.setOrganizationDomain("Roadj.com"); //2
    a.setApplicationName("RoadjPlayer"); //3
    qRegisterMetaTypeStreamOperators<QList<QPointF> >("QList<QPointF>");
    MainWindow w;
    w.show();

    return a.exec();
}
