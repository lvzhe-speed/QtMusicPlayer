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

/* 媒体信息结构体 */
struct MediaObjectInfo {
    /* 用于保存歌曲文件名 */
    QString fileName;
    /* 用于保存歌曲文件路径 */
    QString filePath;
};

class MainWindow : public QMainWindow
{
    //使用Qt信号槽必须加上这个宏
   Q_OBJECT   

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
  
private:
    /* 媒体播放器，用于播放音乐 */
    QMediaPlayer* musicPlayer;

    /* 音乐播放器按钮 */
    QPushButton* pushButton[7];

    /* 垂直布局 */
    QVBoxLayout* vBoxLayout[3];

    /* 水平布局 */
    QHBoxLayout* hBoxLayout[4];

    /* 垂直容器 */
    QWidget* vWidget[3];

    /* 水平容器 */
    QWidget* hWidget[4];

    /* 标签文本 */
    QLabel* label[3];

    /* 用于遮罩 */
    QWidget* listMask;

    QPushButton* searchButton;

    /* 播放进度条 */
    mySlider* durationSlider;

    myTitleBar* m_titleBar;  //自定义标题栏

    WidgetRecord* m_recorder;

    QtMusic* musicSearch;
    /* 音乐布局函数 */
    void musicLayout();

    /* 主窗体大小重设大小函数重写 */
    void resizeEvent(QResizeEvent* event);

    /* 媒体信息存储 */
    QVector<MediaObjectInfo> mediaObjectInfo;
   
    /* 扫描歌曲 */
    void scanSongs();


    /* 媒体播放器类初始化 */
    void mediaPlayerInit();

    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);

private slots:
    /* 播放按钮点击 */
    void btn_play_clicked();

    /* 下一曲按钮点击*/
    void btn_next_clicked();

    /* 上一曲按钮点击 */
    void btn_previous_clicked();

    /* 媒体状态改变 */
    void mediaPlayerStateChanged(QMediaPlayer::State);

    /* 列表单击 */
    void listWidgetCliked(QListWidgetItem*);

    /* 媒体列表项改变 */
    void mediaPlaylistCurrentIndexChanged(int);

    /* 媒体总长度改变 */
    void musicPlayerDurationChanged(qint64);

    /* 媒体播放位置改变 */
    void mediaPlayerPositionChanged(qint64);

    /* 播放进度条松开 */
    void durationSliderReleased();

    void readMusicfile();

    void creatSearchTask();
   
};



#endif // MAINWINDOW_H
