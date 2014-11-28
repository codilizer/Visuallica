#ifndef THUMBNAILWORKER_H
#define THUMBNAILWORKER_H

#include <QtCore>

class ThumbnailThread;
class QPixmap;
class QMutex;


class ThumbnailWorker : public QObject
{
	Q_OBJECT

public:
	ThumbnailWorker(QObject *parent = 0);
	~ThumbnailWorker();

public slots:
	void ThumbnailWorkerLoadThumbnail(QString path, QMutex *);
signals:
	void ThumbnailWorkerSignalResultLoadThumbnail(QPixmap* thumbnail, QString path);
};

#endif //THUMBNAILWORKER_H