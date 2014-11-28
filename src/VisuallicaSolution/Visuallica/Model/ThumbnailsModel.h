#ifndef THUMBNAILSMODEL_H
#define THUMBNAILSMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPixmap>
//#include "ThumbnailThread.h"
#include <QString>
#include <QQueue>
#include <QStack>
#include <QMutex>

class QMimeData;
class ThumbnailThread;


class ThumbnailsModel : public QAbstractListModel
{
	Q_OBJECT


public:
	ThumbnailsModel(/*int thumbnailSize,*/ QObject *parent = 0);
	~ThumbnailsModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QStringList mimeTypes() const;
	QMimeData *mimeData(const QModelIndexList &indexes) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent) const;

	void ThumbnailsModel::setThumbnailFileList(QString selectedDir);
	Qt::DropActions supportedDropActions() const;

	void InsertThumbnail(QPixmap *thumbnail, QString filePath);


	QString MimeTypeThumbnail;
	

private:

	void processThumbnailLoad();

	QStringList m_thumbnailFileList;

	QList<QPixmap> m_thumbnailPixmapList;
	QList<QString> m_loadedThumbnailOriginalFileList;
	
	int m_iThumbnailCount;

	ThumbnailThread * m_pthumbnailThread;
	//QQueue<QString> m_ImagePathQueue;
	QList<QString> m_ImagePathList;

	//QMap<ThumbnailThread*, bool> m_mapThreadPool; // <쓰레드포인터, 사용여부>
	QQueue<QString> m_queueFilesNeedToLoad;

	int m_iMaxThreadPool;

	QMutex *m_pMutex;

};

#endif // THUMBNAILSMODEL_H