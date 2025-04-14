#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 加载QSS文件
    QString appDir = QCoreApplication::applicationDirPath();
    QString qssPath = appDir + "/qss/myQSS.qss";
    QFile styleFile(qssPath);
    qDebug() << "Loading QSS from:" << qssPath;
    if (styleFile.open(QFile::ReadOnly))
    {
        qDebug() << "QSS file loaded successfully.";
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }
    else
    {
        qDebug() << "Failed to load QSS file.";
    }

    // 其他代码
    MainWindow w;
    w.show();
    return app.exec();
}
