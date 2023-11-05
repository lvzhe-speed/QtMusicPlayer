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
			qDebug() << ("���ؽ����Ϊ200��") << json.value("message").toString();
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
 * �������ݽ����Table
 */
void QtMusic::setSearchResultTable(SongList songs)
{
	// ������
	QTableWidget* table = ui.tableWidgetSong;
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	// ��վ�����
	table->clear();
	searchResultPlayLists.clear();

	// ����Table����
	enum 
	{
		titleCol,
		artistCol,
		albumCol,
		durationCol
	};
	table->setColumnCount(4);
	QStringList headers{ QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����"), QString::fromLocal8Bit("ר��"), QString::fromLocal8Bit("ʱ��") };
	table->setHorizontalHeaderLabels(headers);

	// �����г���
	QFontMetrics fm(font());
	int fw = fm.horizontalAdvance("һ�����������߰˾�ʮʮһʮ��ʮ��ʮ��ʮ��");
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
		/* ��ӵ����������ﴢ�� */
		//mediaObjectInfo.append(info);
		/* ��Ӹ����������б� */
		listWidget->addItem(orderSongs.at(0).name);
	}

}

void QtMusic::downloadSong(Music music)
{
	downloadingSong = music;
	QString url = API_DOMAIN + "/song/url?id=" + snum(music.id);
	qDebug() << QString::fromLocal8Bit("��ȡ������Ϣ��") << music.simpleString();
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
			qDebug() << QString::fromLocal8Bit("���ؽ����Ϊ200��") << json.value("message").toString();
			return;
		}
		QJsonArray array = json.value("data").toArray();
		if (!array.size())
		{
			qDebug() << QString::fromLocal8Bit("δ�ҵ�������") << music.simpleString();
			downloadingSong = Music();  // ���
			downloadNext();
			return;
		}

		json = array.first().toObject();
		QString url = JVAL_STR(url);
		int br = JVAL_INT(br); // ����320000
		int size = JVAL_INT(size);
		QString type = JVAL_STR(type); // mp3
		QString encodeType = JVAL_STR(encodeType); // mp3
		qDebug() << QString::fromLocal8Bit(" ��Ϣ��" )<< br << size << type << encodeType << url;
		if (size == 0)
		{
			qDebug() << QString::fromLocal8Bit("����ʧ�ܣ� ����û�а�Ȩ") << music.simpleString();
			if (playAfterDownloaded == music)
			{
				if (orderSongs.contains(music))
				{
					orderSongs.removeOne(music);
					
					// ������Ϣ
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

		// ���ظ�������
		QNetworkAccessManager manager;
		QEventLoop loop;
		QNetworkReply *reply1 = manager.get(QNetworkRequest(QUrl(url)));
		// ���������������غ��˳����¼�ѭ��
		connect(reply1, &QNetworkReply::finished, &loop, &QEventLoop::quit);

		// �������¼�ѭ��
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
			qDebug() << ("���ؽ����Ϊ200��") << json.value("message").toString();
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

			qDebug() << QString::fromLocal8Bit("���ظ����ɣ�") << music.simpleString();
		}
		else
			qDebug() << QString::fromLocal8Bit("warning: ���صĸ���ǿյ�") << music.simpleString();

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
			qDebug() << QString::fromLocal8Bit("���ؽ����Ϊ200��") << json.value("message").toString();
			return;
		}

		QJsonArray array = json.value("songs").toArray();
		if (!array.size())
		{
			qDebug() << QString::fromLocal8Bit("δ�ҵ�������") << music.simpleString();
			downloadingSong = Music();
			downloadNext();
			return;
		}

		json = array.first().toObject();
		QString url = json.value("al").toObject().value("picUrl").toString();
		qDebug() << QString::fromLocal8Bit("�����ַ��") << url;

		// ��ʼ���ظ�������
		static QNetworkAccessManager manager;
		static QEventLoop loop;
		QNetworkReply *reply1 = manager.get(QNetworkRequest(QUrl(url)));

		//���������������ɺ��˳����¼�ѭ��
		connect(reply1, &QNetworkReply::finished, &loop, &QEventLoop::quit);

		//�������¼�ѭ��
		loop.exec();
	});
	manager->get(*request);
}

/**
 * �������ض��С���һ�׸����ؽ�����������һ��
 */
void QtMusic::downloadNext()
{
	// �������صĸ��Ѿ�������/�������صĶ����ǿյ� bool 0 false 1 true
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


