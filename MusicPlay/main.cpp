#include "MusicPlay.h"
#include <QApplication>
#include <Windows.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);



#ifdef DEBUG
    // 分配一个新的控制台窗口
    AllocConsole();

    // 重定向标准输出和标准错误输出到控制台
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif 

    MainWindow w;
    w.show();
    return a.exec();
}