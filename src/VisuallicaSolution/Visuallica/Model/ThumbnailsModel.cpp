#include "ThumbnailsModel.h"
#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QDir>
#include <QMimeData>
#include "ThumbnailThread.h"
#include <QPainter>
#include "ThumbnailProgressManager.h"

ThumbnailsModel::ThumbnailsModel(/*int thumbnailSize, */ QObject *parent /* = 0 */)
	: QAbstractListModel(parent)//, m_iThumbnailSize(thumbnailSize)
{
	m_pMutex = new QMutex();


	m_pthumbnailThread = new ThumbnailThread();

	connect(m_pthumbnailThread, &ThumbnailThread::ThumbnailThreadResultLoadThumbnail,
		this, &ThumbnailsModel::InsertThumbnail);

	m_pthumbnailThread->start();

	MimeTypeThumbnail = "image/x-thumbnail";


	m_thumbnailFileList.clear();
	m_thumbnailPixmapList.clear();
	m_loadedThumbnailOriginalFileList.clear();
}

ThumbnailsModel::~ThumbnailsModel()
{

}

QVariant ThumbnailsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DecorationRole)
		return QIcon(m_thumbnailPixmapList.value(index.row()));

	//else if (role == Qt::UserRole)
		//return m_imagePixmapList.value(index.row());
		//return;
	else if (role == Qt::UserRole + 1)		
		return m_loadedThumbnailOriginalFileList.value(index.row());

	return QVariant();

}

QStringList ThumbnailsModel::mimeTypes() const
{
	QStringList types;
	types << MimeTypeThumbnail;
	return types;
}

QMimeData *ThumbnailsModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach(QModelIndex index, indexes) {
		if (index.isValid()) {
			// QPixmap은 큰사이즈로 다시 읽어올것이기 때문에 Mimedata에 넣을 필요가 없다.
			QPixmap pixmap = qvariant_cast<QPixmap>(data(index, Qt::UserRole));
			QString filePath = data(index, Qt::UserRole + 1).toString();
			//stream << pixmap << filePath;
			stream << filePath;
		}
	}

	mimeData->setData(MimeTypeThumbnail, encodedData);
	return mimeData;
}

Qt::ItemFlags ThumbnailsModel::flags(const QModelIndex &index) const
{
	if (index.isValid() && m_queueFilesNeedToLoad.isEmpty())
		return (QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled);

	return Qt::ItemIsDragEnabled;
}

Qt::DropActions ThumbnailsModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction | Qt::TargetMoveAction;
}


int ThumbnailsModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return m_thumbnailPixmapList.size();
}

void ThumbnailsModel::setThumbnailFileList(QString selectedDir)
{
	if (selectedDir.isNull() || selectedDir.isEmpty())
		return;

	if (!m_thumbnailPixmapList.isEmpty())
	{
		beginRemoveRows(QModelIndex(), 0, 24);
		m_thumbnailPixmapList.clear();
		m_loadedThumbnailOriginalFileList.clear();
		endRemoveRows();
	}

	if(!m_ImagePathList.isEmpty())
		m_ImagePathList.clear();

	if (!m_queueFilesNeedToLoad.isEmpty())
		m_queueFilesNeedToLoad.clear();


	QDir dir(selectedDir);
	QStringList fileList;

	QStringList nameFilter("*.*");
	fileList = dir.entryList(nameFilter, QDir::Files | QDir::NoDotAndDotDot);
	
	QStringList canonicalPathList;

	// 차후 여기서 1차 필터링 해야 할 듯.
	foreach(QString path, fileList)
	{
		QString canonicalPath = dir.canonicalPath() + "\\" + path;
		canonicalPathList.append(canonicalPath);
		m_ImagePathList.append(canonicalPath);
		m_queueFilesNeedToLoad.enqueue(canonicalPath);
	}

	qDebug() << canonicalPathList;
		

	if (fileList.empty())
		return;

	else
		processThumbnailLoad();
}


void ThumbnailsModel::InsertThumbnail(QPixmap *thumbnail, QString filePath)
{
	if (m_ImagePathList.contains(filePath) && thumbnail)
	{		
		if (!thumbnail->isNull() && !filePath.isNull())
		{
			qDebug() << "thumbnail will be add!!";
			int index = m_ImagePathList.indexOf(filePath);

			int row = 0;
			beginInsertRows(QModelIndex(), row, row);

			m_loadedThumbnailOriginalFileList.insert(index, filePath);
			QPixmap scaledPixmap(160, 160);
			QPainter painter(&scaledPixmap);
			painter.fillRect(0, 0, 160, 160, Qt::gray);
			painter.drawPixmap(QPoint(0, 0), thumbnail->scaled(160, 130,
				Qt::IgnoreAspectRatio, Qt::FastTransformation));

			QFont serifFont("Times", 9, QFont::Normal);
			painter.setFont(serifFont);
			painter.drawText(QRectF(3, 130, 152, 30), Qt::TextWordWrap,QFileInfo(filePath).fileName());

			m_thumbnailPixmapList.insert(index, scaledPixmap);

			endInsertRows();
		}

	}
	if (!thumbnail->isNull()) {
		qDebug() << "thumbnail will be deleted!!";
		delete thumbnail;
		qDebug() << "thumbnail deleted!!";
	}
		

	processThumbnailLoad();
}

void ThumbnailsModel::processThumbnailLoad()
{
	if (NULL == m_pthumbnailThread) {
		qDebug() << "Error! m_pthumbnailThread is NULL!!";
	}
	else
	{
		if (!m_queueFilesNeedToLoad.isEmpty())
		{
			float count_m_queueFilesNeedToLoad = m_queueFilesNeedToLoad.count();
			float count_m_ImagePathList = m_ImagePathList.count();
			float valProgress = ((count_m_ImagePathList - count_m_queueFilesNeedToLoad) / count_m_ImagePathList) * 100;

			ThumbnailProgressManager::GetInstance().Progress(valProgress);

			QString filePath = m_queueFilesNeedToLoad.dequeue();
			if (!filePath.isEmpty())
			{
				qDebug() << "call executeThreadLoadThumbnail[" << filePath << "]";

				m_pthumbnailThread->executeThreadLoadThumbnail(filePath, m_pMutex);
			}
		}
		else
		{
			ThumbnailProgressManager::GetInstance().Finish();
		}
	}
}