#ifndef WIDGETRECORD_H
#define WIDGETRECORD_H

#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>

#define ScreenWidth  1000
#define ScreenHighth 600

class PixmapItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ratote READ ratote WRITE setRatote)
public:
    PixmapItem(const QPixmap &pix, QObject *parent = 0) :
        QObject(parent),m_nRatote(0),m_pixmap(pix)
    {
    }

    int ratote() {return m_nRatote;}
    void setRatote(int value) {m_nRatote = value;}
    QPixmap pixmap() {return m_pixmap;}
private:
    int m_nRatote;
    QPixmap m_pixmap;
};

class WidgetRecord : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetRecord(QWidget *parent = 0);
    ~WidgetRecord();

    void SetImage(const QImage &image);
    void Start();
    void Stop();

signals:

public slots:

private:
    PixmapItem *m_handlePixmap;
    QPixmap     m_pixmapRecord;
    QPixmap     m_pixmapArtist;

    int         m_nRotateCD;
    int         m_nRotateHandle;

    QTimer     *m_timer;


    QPropertyAnimation  *m_animationHandle;

private slots:
    void SltCicleRun();
    void SltHandleMove(const QVariant &);

protected:
    void paintEvent(QPaintEvent *);
};



#endif // WIDGETRECORD_H
