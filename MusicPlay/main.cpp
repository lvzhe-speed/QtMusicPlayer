#include "MusicPlay.h"
#include <QApplication>
#include <Windows.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);



#ifdef DEBUG
    // ����һ���µĿ���̨����
    AllocConsole();

    // �ض����׼����ͱ�׼�������������̨
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif 

    MainWindow w;
    w.show();
    return a.exec();
}