#ifndef THUMBNAILTHREAD_H
#define THUMBNAILTHREAD_H

#include <QtCore>
#include <QThread>
#include <QString>


class ThumbnailWorker;
class QMutex;

class ThumbnailThread : public QThread 
{
	Q_OBJECT

public:
	ThumbnailThread();
	~ThumbnailThread();


public:
	void executeThreadLoadThumbnail(QString imagePath, QMutex *pMutex);
signals:
	void ThumbnailThreadSignalLoadThumbnail(QString imagePath, QMutex* pMutex);
	void ThumbnailThreadResultLoadThumbnail(QPixmap *thumbnail, QString path);


protected:
	virtual void run();

private:
	ThumbnailWorker * m_pThumbnailWorker;

};

#endif // THUMBNAILTHREAD_H