#include "mainwindow.h"
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 检查是否已经有实例在运行
    QSharedMemory sharedMemory("TolMqttConnect");
    if (!sharedMemory.create(1)) {
        // 已经有实例在运行，显示已有的实例
        QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("程序已经在运行中！"));
        return 0;
    }
    
    MainWindow w;
    w.show();

    return a.exec();
}
