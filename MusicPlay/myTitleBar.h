#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>
class QLabel;
class QPushButton;

class myTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit myTitleBar(QWidget* parent = nullptr);
    ~myTitleBar();

    void setColor(QString fontColor, QString selectedFontColor);  //设置颜色
protected:

    //界面拖动
    void mousePressEvent(QMouseEvent* event);  //鼠标按下

    void mouseMoveEvent(QMouseEvent* event);  //鼠标移动

    void mouseReleaseEvent(QMouseEvent* event);  //鼠标抬起

    void mouseDoubleClickEvent(QMouseEvent* event);  //双击标题栏进行界面的最大化/还原

    bool eventFilter(QObject* obj, QEvent* event);  //事件过滤器

private slots:

    void onClicked();  //进行最小化、最大化/还原、关闭操作

    void updateMaximize();  //最大化/还原

private:
    QLabel* m_iconLabel;  //显示图标
    QLabel* m_titleLabel;  //显示标题
    QPushButton* m_minimizeButton;  //最小化按键
    QPushButton* m_maximizeButton;   //最大化/还原按键
    QPushButton* m_closeButton;   //关闭按键

    QPoint m_mousePosition;  //鼠标按下时的位置
    bool m_isMousePressed;  //鼠标是否摁下

    QString m_fontColor; //字体色
    QString m_selectedFontColor; //被选中的字体色


protected:
    void paintEvent(QPaintEvent*);

};

#endif 


