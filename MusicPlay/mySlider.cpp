#include "mySlider.h"
#include <QMouseEvent>
#include <QDebug>

mySlider::mySlider(Qt::Orientation orientation,QWidget* parent) :QSlider(parent)
{
    this->setOrientation(orientation);
}

mySlider::~mySlider()
{


}


void mySlider::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)	//�ж����
    {
        //�������ƶ��¼����¼�Ҳ�õ���mousePressEvent,����仰��Ϊ�˲��������Ӱ�죬�ǵ�Slider��������Ӧ��������¼�
        QSlider::mousePressEvent(event);
        //��ȡ��ǰ���λ��
        int currentX = event->pos().x();

        //��ȡ��ǰ�����λ��ռ����Slider�İٷֱ�
        double per = currentX * 1.0 / this->width();


        //������õİٷֱȵõ���������
        int value = per * (this->maximum() - this->minimum()) + this->minimum();

        //�趨������λ��
        this->setValue(value);
        emit sliderReleased();

    }
}

