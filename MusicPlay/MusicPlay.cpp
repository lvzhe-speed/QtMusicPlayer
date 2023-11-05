#include "MusicPlay.h"
#include <QCoreApplication>
#include <QFileInfoList>
#include <QDir>
#include <QFile>
#include <QFileDialog>


#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#pragma comment (lib,"user32.lib")
#endif

QMediaPlaylist* mediaPlaylist;
QListWidget* listWidget;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    /* ָ���ļ� */
    QFile file(":/style.qss");
    /* �ж��ļ��Ƿ���� */
    if (file.exists()) {
        /* ��ֻ���ķ�ʽ�� */
        file.open(QFile::ReadOnly);
        /* ���ַ����ķ�ʽ��������Ľ�� */
        QString styleSheet = QLatin1String(file.readAll());
        /* ����ȫ����ʽ */
        this->setStyleSheet(styleSheet);
        /* �ر��ļ� */
        file.close();
    }
    /* ���ֳ�ʼ�� */
    musicLayout();

    /* ý�岥������ʼ�� */
    mediaPlayerInit();

    /* ɨ����� */
    scanSongs();

    /* ��ť�źŲ����� */
    connect(pushButton[0], SIGNAL(clicked()),
        this, SLOT(btn_previous_clicked()));
    connect(pushButton[1], SIGNAL(clicked()),
        this, SLOT(btn_play_clicked()));
    connect(pushButton[2], SIGNAL(clicked()),
        this, SLOT(btn_next_clicked()));

    /* ý���źŲ����� */
    connect(musicPlayer,
        SIGNAL(stateChanged(QMediaPlayer::State)),
        this,
        SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    connect(mediaPlaylist,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(mediaPlaylistCurrentIndexChanged(int)));
    connect(musicPlayer, SIGNAL(durationChanged(qint64)),
        this,
        SLOT(musicPlayerDurationChanged(qint64)));
    connect(musicPlayer,
        SIGNAL(positionChanged(qint64)),
        this,
        SLOT(mediaPlayerPositionChanged(qint64)));

    /* �б��źŲ����� */
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(listWidgetCliked(QListWidgetItem*)));

    /* slider�źŲ����� */
    connect(durationSlider, SIGNAL(sliderReleased()),
        this, SLOT(durationSliderReleased()));


    /* ʧȥ���� */
   this->setFocus();
}

void MainWindow::musicLayout()
{

    m_titleBar = new myTitleBar(this);
    m_titleBar->resize(this->width(), 30);
    m_titleBar->move(0, 0);
    installEventFilter(m_titleBar);
    this->setWindowTitle("Music Player");  //���ñ�������
    this->setWindowIcon(QIcon(":/font/capricorn.png"));  //����ͼ��

    QString fontColor = "white"; //����ɫ
    QString selectedFontColor = "red"; //��ѡ�е�����ɫ

    //���ñ�������ɫ
    m_titleBar->setColor(fontColor, selectedFontColor);
    //��һ����ȥ��ԭ�߿򼰱��������ڶ����Ǳ�����С������ԭ���ܣ���Ҫ��Ϊ�˻�ԭ����С������ʵ����
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setMenuWidget(m_titleBar);

    /* ����λ�����С,����̶�Ϊ1000, 600 */
    this->setGeometry(100, 200, ScreenWidth, ScreenHighth);
    QPalette pal;//��ɫ��

    /* ��ť */
    for (int i = 0; i < 7; i++)
        pushButton[i] = new QPushButton();

    /* ��ǩ */
    for (int i = 0; i < 3; i++)
        label[i] = new QLabel();

    for (int i = 0; i < 3; i++) {
        /* ��ֱ���� */
        vWidget[i] = new QWidget();
        vWidget[i]->setAutoFillBackground(true);
        /* ��ֱ���� */
        vBoxLayout[i] = new QVBoxLayout();
    }

    for (int i = 0; i < 4; i++) {
        /* ˮƽ���� */
        hWidget[i] = new QWidget();
        hWidget[i]->setAutoFillBackground(true);
        /* ˮƽ���� */
        hBoxLayout[i] = new QHBoxLayout();
    }

    /* ���Ž����� */
    durationSlider = new mySlider(Qt::Horizontal);
    durationSlider->setMinimumSize(300, 15);
    durationSlider->setMaximumHeight(15);
    durationSlider->setObjectName("durationSlider");

    /* �����б� */
    listWidget = new QListWidget();
    listWidget->setObjectName("listWidget");
    listWidget->setVerticalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff);
    

    /* ���ö������� */
    pushButton[0]->setObjectName("btn_previous");
    pushButton[1]->setObjectName("btn_play");
    pushButton[2]->setObjectName("btn_next");
    pushButton[3]->setObjectName("btn_favorite");
    pushButton[4]->setObjectName("btn_mode");
    pushButton[5]->setObjectName("btn_menu");
    pushButton[6]->setObjectName("btn_volume");

    /* ���ð�ť���� */
    pushButton[1]->setCheckable(true);
    pushButton[3]->setCheckable(true);

    connect(pushButton[5], SIGNAL(clicked(bool)), this, SLOT(readMusicfile()));

    /*���������ؼ�*/
    searchButton=new QPushButton();
    searchButton->setFixedSize(25, 25);
    searchButton->setObjectName("searchButton");
    searchButton->setCheckable(true);
    connect(searchButton, SIGNAL(clicked(bool)),this,SLOT(creatSearchTask()));

    /* H0���� */
    vWidget[0]->setMinimumSize(400, ScreenHighth);
    vWidget[0]->setMaximumWidth(400);
    vWidget[1]->setMinimumSize(400, ScreenHighth);
    QSpacerItem* hSpacer0 = new
        QSpacerItem(50, ScreenHighth,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);//����Qt����

    QSpacerItem* hSpacer1 = new
        QSpacerItem(60, ScreenHighth,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);

    QSpacerItem* hSpacer2 = new
        QSpacerItem(50, ScreenHighth,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);

    hBoxLayout[0]->addSpacerItem(hSpacer0);;
    hBoxLayout[0]->addWidget(vWidget[0]);
    hBoxLayout[0]->addSpacerItem(hSpacer1);
    hBoxLayout[0]->addWidget(vWidget[1]);
    hBoxLayout[0]->addSpacerItem(hSpacer2);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    hWidget[0]->setLayout(hBoxLayout[0]);
    setCentralWidget(hWidget[0]);

    /* V0���� */
    listWidget->setMinimumSize(400, 265);
    listWidget->setMaximumHeight(265);
    hWidget[1]->setMinimumSize(400, 80);
    hWidget[1]->setMaximumHeight(80);
    label[0]->setMinimumSize(400, 50);
    label[0]->setMaximumHeight(50);
    QSpacerItem* vSpacer0 = new
        QSpacerItem(400, 10,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);
    QSpacerItem* vSpacer1 = new
        QSpacerItem(400, 20,
            QSizePolicy::Minimum,
            QSizePolicy::Minimum);
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->addWidget(searchButton);
    vBoxLayout[0]->addWidget(label[0]);
    vBoxLayout[0]->addWidget(listWidget);
    vBoxLayout[0]->addWidget(hWidget[1]);
    vBoxLayout[0]->addSpacerItem(vSpacer1);
    vBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    vWidget[0]->setLayout(vBoxLayout[0]);

    /* H1���� */
    for (int i = 0; i < 3; i++) {
        pushButton[i]->setMinimumSize(80, 80);
    }
    QSpacerItem* hSpacer3 = new
        QSpacerItem(40, 80,
            QSizePolicy::Expanding,
            QSizePolicy::Expanding);
    QSpacerItem* hSpacer4 = new
        QSpacerItem(40, 80,
            QSizePolicy::Expanding,
            QSizePolicy::Expanding);
    hBoxLayout[1]->addWidget(pushButton[0]);
    hBoxLayout[1]->addSpacerItem(hSpacer3);
    hBoxLayout[1]->addWidget(pushButton[1]);
    hBoxLayout[1]->addSpacerItem(hSpacer4);
    hBoxLayout[1]->addWidget(pushButton[2]);
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);

    hWidget[1]->setLayout(hBoxLayout[1]);

    /* V1���� */
    QSpacerItem* vSpacer2 = new
        QSpacerItem(320, 40,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);
    QSpacerItem* vSpacer3 = new
        QSpacerItem(320, 20,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);
    QSpacerItem* vSpacer4 = new
        QSpacerItem(320, 30,
            QSizePolicy::Minimum,
            QSizePolicy::Minimum);

    vWidget[2]->setMinimumSize(300, 80);
    vWidget[2]->setMaximumHeight(80);
    vBoxLayout[1]->addSpacerItem(vSpacer2);

    m_recorder = new WidgetRecord();
    m_recorder->setMinimumSize(320, 320);
    vBoxLayout[1]->addWidget(m_recorder);
    vBoxLayout[1]->addSpacerItem(vSpacer3);
    vBoxLayout[1]->addWidget(durationSlider);
    vBoxLayout[1]->addWidget(vWidget[2]);
    vBoxLayout[1]->addSpacerItem(vSpacer4);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);

    vWidget[1]->setLayout(vBoxLayout[1]);

    /* V2���� */
    QSpacerItem* vSpacer5 = new
        QSpacerItem(300, 10,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);
    hWidget[2]->setMinimumSize(320, 20);
    hWidget[3]->setMinimumSize(320, 60);
    vBoxLayout[2]->addWidget(hWidget[2]);
    vBoxLayout[2]->addSpacerItem(vSpacer5);
    vBoxLayout[2]->addWidget(hWidget[3]);
    vBoxLayout[2]->setContentsMargins(0, 0, 0, 0);

    vWidget[2]->setLayout(vBoxLayout[2]);

    /* H2���� */
    QFont font;

    font.setPixelSize(10);

    /* ���ñ�ǩ�ı� */
    label[0]->setText(u8"LQ music,enjoy it!\n�����б�");
    label[1]->setText("00:00");
    label[2]->setText("00:00");
    label[1]->setSizePolicy(QSizePolicy::Expanding,
        QSizePolicy::Expanding);
    label[2]->setSizePolicy(QSizePolicy::Expanding,
        QSizePolicy::Expanding);
    label[2]->setAlignment(Qt::AlignRight);
    label[1]->setAlignment(Qt::AlignLeft);
    label[1]->setFont(font);
    label[2]->setFont(font);

    pal.setColor(QPalette::WindowText, Qt::white);
    label[0]->setPalette(pal);
    label[1]->setPalette(pal);
    label[2]->setPalette(pal);

    hBoxLayout[2]->addWidget(label[1]);
    hBoxLayout[2]->addWidget(label[2]);

    hBoxLayout[2]->setContentsMargins(0, 0, 0, 0);
    hWidget[2]->setLayout(hBoxLayout[2]);

    /* H3���� */
    QSpacerItem* hSpacer5 = new
        QSpacerItem(0, 60,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);
    QSpacerItem* hSpacer6 = new
        QSpacerItem(80, 60,
            QSizePolicy::Maximum,
            QSizePolicy::Maximum);
    QSpacerItem* hSpacer7 = new
        QSpacerItem(80, 60,
            QSizePolicy::Maximum,
            QSizePolicy::Maximum);
    QSpacerItem* hSpacer8 = new
        QSpacerItem(80, 60,
            QSizePolicy::Maximum,
            QSizePolicy::Maximum);
    QSpacerItem* hSpacer9 = new
        QSpacerItem(0, 60,
            QSizePolicy::Minimum,
            QSizePolicy::Maximum);

    for (int i = 3; i < 7; i++) {
        pushButton[i]->setMinimumSize(25, 25);
        pushButton[i]->setMaximumSize(25, 25);
    }

    hBoxLayout[3]->addSpacerItem(hSpacer5);
    hBoxLayout[3]->addWidget(pushButton[3]);
    hBoxLayout[3]->addSpacerItem(hSpacer6);
    hBoxLayout[3]->addWidget(pushButton[4]);
    hBoxLayout[3]->addSpacerItem(hSpacer7);
    hBoxLayout[3]->addWidget(pushButton[5]);
    hBoxLayout[3]->addSpacerItem(hSpacer8);
    hBoxLayout[3]->addWidget(pushButton[6]);
    hBoxLayout[3]->addSpacerItem(hSpacer9);
    hBoxLayout[3]->setContentsMargins(0, 0, 0, 0);
    hBoxLayout[3]->setAlignment(Qt::AlignHCenter);

    hWidget[3]->setLayout(hBoxLayout[3]);


    /* �б����� */
    listMask = new QWidget(listWidget);
    listMask->setMinimumSize(400, 50);
    listMask->setMinimumHeight(50);
    listMask->setObjectName("listMask");
    listMask->setGeometry(0,
        listWidget->height() - 50,
        400,
        50);

//debug��
    //hWidget[0]->setStyleSheet("background-color:red");
    //hWidget[1]->setStyleSheet("background-color:white");
    //hWidget[2]->setStyleSheet("background-color:yellow");
    //hWidget[3]->setStyleSheet("background-color:black");
    //vWidget[0]->setStyleSheet("background-color:blue");
    //vWidget[1]->setStyleSheet("background-color:green");
    //vWidget[2]->setStyleSheet("background-color:pink");
    
}


MainWindow::~MainWindow()
{
   // delete 
}

void MainWindow::btn_play_clicked()
{
    if (!mediaPlaylist->isEmpty()) {
        int state = musicPlayer->state();

        switch (state) {
        case QMediaPlayer::StoppedState:
            /* ý�岥�� */
            musicPlayer->play();
            m_recorder->Start();
            break;

        case QMediaPlayer::PlayingState:
            /* ý����ͣ */
            musicPlayer->pause();
            m_recorder->Stop();
            break;

        case QMediaPlayer::PausedState:
            musicPlayer->play();
            m_recorder->Start();
            break;
        }
    }
}

void MainWindow::btn_next_clicked()
{
    if (!mediaPlaylist->isEmpty()) {
        musicPlayer->stop();
        int count = mediaPlaylist->mediaCount();
        if (0 == count)
            return;

        /* �б���һ�� */
        mediaPlaylist->next();
        musicPlayer->play();
        m_recorder->Start();
    }
}

void MainWindow::btn_previous_clicked()
{
    if (!mediaPlaylist->isEmpty()) {
        musicPlayer->stop();
        int count = mediaPlaylist->mediaCount();
        if (0 == count)
            return;

        /* �б���һ�� */
        mediaPlaylist->previous();
        musicPlayer->play();
        m_recorder->Start();
    }
}

void MainWindow::mediaPlayerStateChanged(
    QMediaPlayer::State
    state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        pushButton[1]->setChecked(false);
        break;

    case QMediaPlayer::PlayingState:
        pushButton[1]->setChecked(true);
        break;

    case QMediaPlayer::PausedState:
        pushButton[1]->setChecked(false);
        break;
    }
}

void MainWindow::listWidgetCliked(QListWidgetItem* item)
{
    musicPlayer->stop();
    mediaPlaylist->setCurrentIndex(listWidget->row(item));
    musicPlayer->play();
    m_recorder->Start();
}

void MainWindow::mediaPlaylistCurrentIndexChanged(
    int index)
{
    if (-1 == index)
        return;

    /* �����б����ڲ��ŵ��� */
    listWidget->setCurrentRow(index);

}

void MainWindow::musicPlayerDurationChanged(
    qint64 duration)
{
    durationSlider->setRange(0, duration / 1000);
    int second = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute, 10);
    else
        mediaDuration = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaDuration = mediaDuration
        + ":" + QString::number(second, 10);
    else
        mediaDuration = mediaDuration
        + ":0" + QString::number(second, 10);

    /* ��ʾý���ܳ���ʱ�� */
    label[2]->setText(mediaDuration);
}

void MainWindow::mediaPlayerPositionChanged(
    qint64 position)
{
    if (!durationSlider->isSliderDown())
        durationSlider->setValue(position / 1000);

    int second = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaPosition;
   mediaPosition.clear();

    if (minute >= 10)
        mediaPosition = QString::number(minute, 10);
    else
        mediaPosition = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaPosition = mediaPosition
        + ":" + QString::number(second, 10);
    else
        mediaPosition = mediaPosition
        + ":0" + QString::number(second, 10);

    /* ��ʾ���ڲ��ŵ�ʱ�� */
    label[1]->setText(mediaPosition);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    listMask->setGeometry(0,
        listWidget->height() - 50,
        400,
        50);
    m_titleBar->resize(this->width(), 30);  //���ı�������С
}

void MainWindow::durationSliderReleased()
{
    /* ����ý�岥�ŵ�λ�� */
    musicPlayer->setPosition(durationSlider->value() * 1000);
}


void MainWindow::scanSongs()
{
    QDir dir(QApplication::applicationDirPath() + "/musics");
    QDir dirbsolutePath(dir.absolutePath());
    /* ���Ŀ¼���� */
    if (dirbsolutePath.exists()) {
        /* ��������� */
        QStringList filter;
        /* ��������.mp3��׺���ļ� */
        filter << "*.mp3";
        /* ��ȡ��Ŀ¼�µ������ļ� */
        QFileInfoList files =
            dirbsolutePath.entryInfoList(filter, QDir::Files,QDir::Name);
        /* ���� */
        for (int i = 0; i < files.count(); i++) {
            MediaObjectInfo info;
            /* ʹ��utf-8���� */
            QString fileName = QString::fromUtf8(files.at(i)
                .fileName()
                .replace(".mp3", "")
                .toUtf8()
                .data());//��.mp3ȥ��
            info.fileName = fileName;
           /* if (fileName.contains("-")) {
                info.fileName = fileName + "\n"
                    + fileName.split("-").at(1);
            }
            else
            {
                info.fileName = fileName + "\n" + "Unknown";
            }*/
            info.filePath = QString::fromUtf8(files.at(i)
                .filePath()
                .toUtf8()
                .data());
            /* ý���б���Ӹ��� */
            if (mediaPlaylist->addMedia(
                QUrl::fromLocalFile(info.filePath))) {
                /* ��ӵ����������ﴢ�� */
                mediaObjectInfo.append(info);
                /* ��Ӹ����������б� */
                listWidget->addItem(info.fileName);
            }
        }
    }
    
}


void MainWindow::readMusicfile()
{
    QString fileName;
    int index;
    QStringList files = QFileDialog::getOpenFileNames(this, u8"�������ļ�", ".", "Music (*.mp3)");
    for (int i = 0; i < files.count(); i++) {
        MediaObjectInfo info;
        /* ʹ��utf-8���� */
        index = files.at(i).lastIndexOf("/");
        fileName = files.at(i).right(files.at(i).length()-index-1);
        fileName = fileName.replace(".mp3", "").toUtf8().data();//��.mp3ȥ��
        info.fileName = fileName;
        info.filePath = files.at(i);
        /* ý���б���Ӹ��� */
        if (mediaPlaylist->addMedia(
            QUrl::fromLocalFile(info.filePath))) {
            /* ��ӵ����������ﴢ�� */
            mediaObjectInfo.append(info);
            /* ��Ӹ����������б� */
            listWidget->addItem(info.fileName);
        }
        else {
            qDebug() <<
                mediaPlaylist->errorString()
                .toUtf8().data()
                << endl;
            qDebug() << "  Error number:"
                << mediaPlaylist->error()
                << endl;
        }
    }

}



void MainWindow::creatSearchTask() 
{
    musicSearch = new QtMusic();

    musicSearch->show();
    
}


void MainWindow::mediaPlayerInit()
{
    musicPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);
    /* ȷ���б��ǿյ� */
    mediaPlaylist->clear();
    /* �������ֲ��������б�ΪmediaPlaylist */
    musicPlayer->setPlaylist(mediaPlaylist);
    /* ���ò���ģʽ��Loop����ѭ�� */
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
}

bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{

#ifdef Q_OS_WIN
    MSG* msg = (MSG*)message;
    if (msg->message == WM_SYSCOMMAND)
    {
        if (61587 == msg->wParam)//���õ���
        {
            return true;
        }
        if (61539 == msg->wParam)//����˫��
        {
            return true;
        }
    }
#endif
    return QMainWindow::nativeEvent(eventType, message, result);
}
