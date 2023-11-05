#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QSlider>

class mySlider :public QSlider
{
public:
    mySlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~mySlider();


protected:
    void mousePressEvent(QMouseEvent* event);  //µ¥»÷
};

#endif
