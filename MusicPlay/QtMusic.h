#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMusic.h"
#include "MusicBeans.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>
#include <QDir>

class QtMusic : public QMainWindow
{
    Q_OBJECT

public:
	QtMusic(QWidget *parent = Q_NULLPTR);

private:
	void initForm();
	void initPageSong();
	void signalSlotDefine();
	void setSearchResultTable(SongList songs);
	QString msecondToString(qint64 msecond);
	void downloadSong(Music music);
	void downloadSongLyric(Music music);
	void downloadSongCover(Music music);
	void downloadNext();
	QString coverPath(const Music &music) const;
	QString lyricPath(const Music &music) const;
	QString songPath(const Music& music) const;
private slots:
	void slotOnSerach();
	void slotTableWidgetSongDoubleClicked(int nRow, int nCol);


private:
    Ui::QtMusicClass ui;
	const QString API_DOMAIN = "http://iwxyi.com:3000/";
	SongList searchResultSongs;
	PlayListList searchResultPlayLists;
	Music downloadingSong;
	Music playAfterDownloaded;

	QDir musicFileDir;   // 用于程序识别
	
	SongList orderSongs;             // 播放列表
	//SongList favoriteSongs;          // 我的喜欢
	//SongList localSongs;             // 本地歌曲
	SongList toDownLoadSongs;        // 即将下载
};
