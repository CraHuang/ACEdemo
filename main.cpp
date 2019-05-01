#include "acegui.h"
#include <QApplication>
#include<QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator transltor;
    transltor.load("acegui_translation_zh_cn.qm");
    a.installTranslator(&transltor);
    ACEGUI w;
    w.show();


    return a.exec();
}
