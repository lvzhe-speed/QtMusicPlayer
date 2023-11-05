#include "widgetrecord.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

WidgetRecord::WidgetRecord(QWidget *parent) : QWidget(parent)
{
    m_nRotateCD = 0;
    m_nRotateHandle = 0;

    m_handlePixmap = new PixmapItem(QPixmap(":/images/ic_handle.png"), this);
    m_pixmapRecord = QPixmap(":/images/ic_blackrecord.png");
    m_pixmapArtist = QPixmap(":/images/cd.png");

    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(SltCicleRun()));

    m_animationHandle = new QPropertyAnimation(m_handlePixmap, "ratote");
    m_animationHandle->setDuration(300);
    m_animationHandle->setEasingCurve(QEasingCurve::Linear);
    connect(m_animationHandle, SIGNAL(valueChanged(QVariant)), this, SLOT(SltHandleMove(QVariant)));
}

WidgetRecord::~WidgetRecord()
{

}

void WidgetRecord::SetImage(const QImage &image)
{

    QImage Image;
    Image.load(":/images/cd.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    int with = 320;
    int height = 320;
    QPixmap fitpixmap = pixmap.scaled(with, height,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation);

    this->update();
}

void WidgetRecord::Start()
{
    m_timer->start();

    m_animationHandle->setStartValue(0);
    m_animationHandle->setEndValue(30);
    m_animationHandle->start();
    this->update();
}

void WidgetRecord::Stop()
{
    m_timer->stop();
    m_animationHandle->setStartValue(30);
    m_animationHandle->setEndValue(0);
    m_animationHandle->start();
    this->update();
}

void WidgetRecord::SltCicleRun()
{
    m_nRotateCD += 1;
    if (m_nRotateCD >= 360) {
        m_nRotateCD = 0;
    }
    this->update();
}

void WidgetRecord::SltHandleMove(const QVariant &)
{
    this->update();
}

void WidgetRecord::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    int nX = this->width() / 2;
    int nY = this->height() / 2;

    painter.save();
    painter.translate(nX, nY);
    painter.rotate(m_nRotateCD);
    painter.translate(-nX, -nY);

    painter.drawPixmap(nX - m_pixmapArtist.width() / 2, nY - m_pixmapArtist.height() / 2, m_pixmapArtist.scaled(320, 320));
    painter.drawPixmap(nX - m_pixmapRecord.width() / 2, nY - m_pixmapRecord.height() / 2, m_pixmapRecord);
    painter.restore();

    painter.save();
    nY = nY - m_handlePixmap->pixmap().height() / 2 - m_pixmapArtist.height() / 2;
    painter.translate(this->width() / 2, nY);
    painter.rotate(m_handlePixmap->ratote());
    painter.translate(-this->width() / 2, -nY);
    painter.drawPixmap(this->width() / 2, nY, m_handlePixmap->pixmap());
    painter.restore();
}
