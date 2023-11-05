#include "QtMusic.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QPropertyAnimation>
#include <QListWidget>

extern QMediaPlaylist* mediaPlaylist;
extern QListWidget* listWidget;

QtMusic::QtMusic(QWidget *parent)
    : QMainWindow(parent),
	musicFileDir(QApplication::applicationDirPath() + "/musics")
{
    ui.setupUi(this);
	initForm();
	initPageSong();
	signalSlotDefine();
}

void QtMusic::initForm()
{
	QString strDir = QCoreApplication::applicationDirPath() + "//" + "musics";
	QDir dir(strDir);

	if (!dir.exists())
		dir.mkdir(strDir);

}

void QtMusic::initPageSong()
{
	auto pTable = ui.tableWidgetSong;
	pTable->setMouseTracking(true);
	pTable->horizontalHeader()->setMinimumSectionSize(50);
	pTable->horizontalHeader()->setMaximumHeight(32);
	pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	pTable->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	pTable->horizontalHeader()->setStretchLastSection(true);
	pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	pTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void QtMusic::signalSlotDefine()
{
	connect(ui.tbnSearch, SIGNAL(clicked()), this, SLOT(slotOnSerach()));
	connect(ui.tableWidgetSong, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(slotTableWidgetSongDoubleClicked(int, int)));
}

void QtMusic::slotOnSerach()
{
	QString strSongName = ui.lineEditSearch->text();
	if (strSongName.trimmed().isEmpty())
		return;

	QString url = "http://iwxyi.com:3000/search?keywords=" + strSongName.toUtf8().toPercentEncoding();
	QNetworkAccessManager* manager = new QNetworkAccessManager;
	QNetworkRequest* request = new QNetworkRequest(url);

	request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
	
	connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply)
	{
		QByteArray data = reply->readAll();
		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(data, &error);
		if (error.error != QJsonParseError::NoError)
		{
			qDebug() << error.errorString();
			return;
		}

		QJsonObject json = document.object();
		if (json.value("code").toInt() != 200)
		{
			qDebug() << ("返回结果不为200：") << json.value("message").toString();
			return;
		}

		QJsonArray musics = json.value("result").toObject().value("songs").toArray();


		searchResultSongs.clear();
		foreach(QJsonValue val, musics)
			searchResultSongs << Music::fromJson(val.toObject());
			
		setSearchResultTable(searchResultSongs);

	});
	manager->get(*request);
}

/**
 * 搜索数据结果到Table
 */
void QtMusic::setSearchResultTable(SongList songs)
{
	// 建立表
	QTableWidget* table = ui.tableWidgetSong;
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	// 清空旧数据
	table->clear();
	searchResultPlayLists.clear();

	// 设置Table标题
	enum 
	{
		titleCol,
		artistCol,
		albumCol,
		durationCol
	};
	table->setColumnCount(4);
	QStringList headers{ QString::fromLocal8Bit("标题"), QString::fromLocal8Bit("作者"), QString::fromLocal8Bit("专辑"), QString::fromLocal8Bit("时长") };
	table->setHorizontalHeaderLabels(headers);

	// 设置列长度
	QFontMetrics fm(font());
	int fw = fm.horizontalAdvance("一二三四五六七八九十十一十二十三十四十五");
	auto createItem = [=](QString s) {
		QTableWidgetItem *item = new QTableWidgetItem();
		if (s.length() > 16 && fm.horizontalAdvance(s) > fw)
		{
			item->setToolTip(s);
			s = s.left(15) + "...";
		}
		item->setText(s);
		return item;
	};

	table->setRowCount(songs.size());
	for (int row = 0; row < songs.size(); row++)
	{
		Music song = songs.at(row);
		table->setItem(row, titleCol, createItem(song.name));
		table->setItem(row, artistCol, createItem(song.artistNames));
		table->setItem(row, albumCol, createItem(song.album.name));
		table->setItem(row, durationCol, createItem(msecondToString(song.duration)));
	}

	QTimer::singleShot(0, [=] 
	{
		table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	});
}

QString QtMusic::msecondToString(qint64 msecond)
{
	return QString("%1:%2").arg(msecond / 1000 / 60, 2, 10, QLatin1Char('0'))
		.arg(msecond / 1000 % 60, 2, 10, QLatin1Char('0'));
}

void QtMusic::slotTableWidgetSongDoubleClicked(int nRow, int nCol)
{
	int row = nRow;
	int index = nRow;
	Music currentsong;
	if (row > -1)
		currentsong = searchResultSongs.at(index);

	if (orderSongs.contains(currentsong))
		orderSongs.removeOne(currentsong);
	else
		orderSongs.insert(0, currentsong);

	playAfterDownloaded = currentsong;
	downloadSong(currentsong);


	if (!orderSongs.contains(currentsong))
		orderSongs.append(currentsong);

	//qDebug() << "orderSongs:" << orderSongs.at(0).name;



	if (mediaPlaylist->addMedia(
		QUrl::fromLocalFile(songPath(orderSongs.at(0))))) {
		/* 添加到容器数组里储存 */
		//mediaObjectInfo.append(info);
		/* 添加歌曲名字至列表 */
		listWidget->addItem(orderSongs.at(0).name);
	}

}

void QtMusic::downloadSong(Music music)
{
	downloadingSong = music;
	QString url = API_DOMAIN + "/song/url?id=" + snum(music.id);
	qDebug() << QString::fromLocal8Bit("获取歌曲信息：") << music.simpleString();
	QNetworkAccessManager* manager = new QNetworkAccessManager;
	QNetworkRequest* request = new QNetworkRequest(url);
	request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
	connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply)
	{
		QByteArray baData = reply->readAll();
		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(baData, &error);
		if (error.error != QJsonParseError::NoError)
		{
			qDebug() << error.errorString();
			return;
		}
		QJsonObject json = document.object();
		if (json.value("code").toInt() != 200)
		{
			qDebug() << QString::fromLocal8Bit("返回结果不为200：") << json.value("message").toString();
			return;
		}
		QJsonArray array = json.value("data").toArray();
		if (!array.size())
		{
			qDebug() << QString::fromLocal8Bit("未找到歌曲：") << music.simpleString();
			downloadingSong = Music();  // 清空
			downloadNext();
			return;
		}

		json = array.first().toObject();
		QString url = JVAL_STR(url);
		int br = JVAL_INT(br); // 比率320000
		int size = JVAL_INT(size);
		QString type = JVAL_STR(type); // mp3
		QString encodeType = JVAL_STR(encodeType); // mp3
		qDebug() << QString::fromLocal8Bit(" 信息：" )<< br << size << type << encodeType << url;
		if (size == 0)
		{
			qDebug() << QString::fromLocal8Bit("下载失败， 可能没有版权") << music.simpleString();
			if (playAfterDownloaded == music)
			{
				if (orderSongs.contains(music))
				{
					orderSongs.removeOne(music);
					
					// 设置信息
					auto max16 = [=](QString s)
					{
						if (s.length() > 16)
							s = s.left(15) + "...";
						return s;
					};
					//player->stop();
				}
			}
			downloadingSong = Music();
			downloadNext();
			return;
		}

		// 下载歌曲本身
		QNetworkAccessManager manager;
		QEventLoop loop;
		QNetworkReply *reply1 = manager.get(QNetworkRequest(QUrl(url)));
		// 请求结束并完成下载后退出子事件循环
		connect(reply1, &QNetworkReply::finished, &loop, &QEventLoop::quit);

		// 开启子事件循环
		loop.exec();
		QByteArray baData1 = reply1->readAll();

		QString strPath = songPath(music);
		QFile file(strPath);
		file.open(QIODevice::WriteOnly);
		file.write(baData1);
		file.flush();
		file.close();

		downloadingSong = Music();
		downloadNext();
	});
	manager->get(*request);

	downloadSongLyric(music);
	downloadSongCover(music);
}

void QtMusic::downloadSongLyric(Music music)
{
	downloadingSong = music;
	QString url = API_DOMAIN + "/lyric?id=" + snum(music.id);
	QNetworkAccessManager* manager = new QNetworkAccessManager;
	QNetworkRequest* request = new QNetworkRequest(url);
	request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
	connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply) {
		QByteArray baData = reply->readAll();
		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(baData, &error);
		if (error.error != QJsonParseError::NoError)
		{
			qDebug() << error.errorString();
			return;
		}
		QJsonObject json = document.object();
		if (json.value("code").toInt() != 200)
		{
			qDebug() << ("返回结果不为200：") << json.value("message").toString();
			return;
		}

		QString lrc = json.value("lrc").toObject().value("lyric").toString();
		if (!lrc.isEmpty())
		{
			QFile file(lyricPath(music));
			file.open(QIODevice::WriteOnly);
			QTextStream stream(&file);
			stream << lrc;
			file.flush();
			file.close();

			qDebug() << QString::fromLocal8Bit("下载歌词完成：") << music.simpleString();
		}
		else
			qDebug() << QString::fromLocal8Bit("warning: 下载的歌词是空的") << music.simpleString();

	});
	manager->get(*request);
}

void QtMusic::downloadSongCover(Music music)
{
	if (QFileInfo(coverPath(music)).exists())
		return;
	downloadingSong = music;
	QString url = API_DOMAIN + "/song/detail?ids=" + snum(music.id);
	QNetworkAccessManager* manager = new QNetworkAccessManager;
	QNetworkRequest* request = new QNetworkRequest(url);
	request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
	request->setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
	connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply) {
		QByteArray baData = reply->readAll();
		QJsonParseError error;
		QJsonDocument document = QJsonDocument::fromJson(baData, &error);
		if (error.error != QJsonParseError::NoError)
		{
			qDebug() << error.errorString();
			return;
		}
		QJsonObject json = document.object();
		if (json.value("code").toInt() != 200)
		{
			qDebug() << QString::fromLocal8Bit("返回结果不为200：") << json.value("message").toString();
			return;
		}

		QJsonArray array = json.value("songs").toArray();
		if (!array.size())
		{
			qDebug() << QString::fromLocal8Bit("未找到歌曲：") << music.simpleString();
			downloadingSong = Music();
			downloadNext();
			return;
		}

		json = array.first().toObject();
		QString url = json.value("al").toObject().value("picUrl").toString();
		qDebug() << QString::fromLocal8Bit("封面地址：") << url;

		// 开始下载歌曲本身
		static QNetworkAccessManager manager;
		static QEventLoop loop;
		QNetworkReply *reply1 = manager.get(QNetworkRequest(QUrl(url)));

		//请求结束并下载完成后，退出子事件循环
		connect(reply1, &QNetworkReply::finished, &loop, &QEventLoop::quit);

		//开启子事件循环
		loop.exec();
	});
	manager->get(*request);
}

/**
 * 放入下载队列、或一首歌下载结束后，下载下一个
 */
void QtMusic::downloadNext()
{
	// 正在下载的歌已经下载了/即将下载的队列是空的 bool 0 false 1 true
	if (downloadingSong.isValid() || !toDownLoadSongs.size())
		return;
	Music music = toDownLoadSongs.takeFirst();
	if (!music.isValid())
		return downloadNext();
	downloadSong(music);
}

QString QtMusic::coverPath(const Music &music) const
{
	return musicFileDir.absoluteFilePath(music.name + ".jpg");
}

QString QtMusic::lyricPath(const Music &music) const
{
	return musicFileDir.absoluteFilePath(music.name + ".lrc");
}

QString QtMusic::songPath(const Music &music) const
{
	return musicFileDir.absoluteFilePath(music.name + ".mp3");
}


