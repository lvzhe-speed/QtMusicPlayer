#ifndef MUSICPLAY_H
#define MUSICPLAY_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QDebug>
#include "widgetrecord.h"
#include "mySlider.h"
#include "myTitleBar.h"
#include "QtMusic.h"

/* ý����Ϣ�ṹ�� */
struct MediaObjectInfo {
    /* ���ڱ�������ļ��� */
    QString fileName;
    /* ���ڱ�������ļ�·�� */
    QString filePath;
};

class MainWindow : public QMainWindow
{
    //ʹ��Qt�źŲ۱�����������
   Q_OBJECT   

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
  
private:
    /* ý�岥���������ڲ������� */
    QMediaPlayer* musicPlayer;

    /* ���ֲ�������ť */
    QPushButton* pushButton[7];

    /* ��ֱ���� */
    QVBoxLayout* vBoxLayout[3];

    /* ˮƽ���� */
    QHBoxLayout* hBoxLayout[4];

    /* ��ֱ���� */
    QWidget* vWidget[3];

    /* ˮƽ���� */
    QWidget* hWidget[4];

    /* ��ǩ�ı� */
    QLabel* label[3];

    /* �������� */
    QWidget* listMask;

    QPushButton* searchButton;

    /* ���Ž����� */
    mySlider* durationSlider;

    myTitleBar* m_titleBar;  //�Զ��������

    WidgetRecord* m_recorder;

    QtMusic* musicSearch;
    /* ���ֲ��ֺ��� */
    void musicLayout();

    /* �������С�����С������д */
    void resizeEvent(QResizeEvent* event);

    /* ý����Ϣ�洢 */
    QVector<MediaObjectInfo> mediaObjectInfo;
   
    /* ɨ����� */
    void scanSongs();


    /* ý�岥�������ʼ�� */
    void mediaPlayerInit();

    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private slots:
    /* ���Ű�ť��� */
    void btn_play_clicked();

    /* ��һ����ť���*/
    void btn_next_clicked();

    /* ��һ����ť��� */
    void btn_previous_clicked();

    /* ý��״̬�ı� */
    void mediaPlayerStateChanged(QMediaPlayer::State);

    /* �б��� */
    void listWidgetCliked(QListWidgetItem*);

    /* ý���б���ı� */
    void mediaPlaylistCurrentIndexChanged(int);

    /* ý���ܳ��ȸı� */
    void musicPlayerDurationChanged(qint64);

    /* ý�岥��λ�øı� */
    void mediaPlayerPositionChanged(qint64);

    /* ���Ž������ɿ� */
    void durationSliderReleased();

    void readMusicfile();

    void creatSearchTask();
   
};



#endif // MAINWINDOW_H
