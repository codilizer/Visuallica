#include "ThumbnailThread.h"
#include "ThumbnailWorker.h"
#include <QPixmap>
#include <QtCore>
#include <QMutex>

ThumbnailThread::ThumbnailThread() : QThread()
{

}

ThumbnailThread::~ThumbnailThread()
{

}

void ThumbnailThread::run()
{
	m_pThumbnailWorker = new ThumbnailWorker();


	qRegisterMetaType<QPixmap*>("QPixmap*");
	qRegisterMetaType<ThumbnailThread*>("ThumbnailThread*");
	qRegisterMetaType<QMutex*>("QMutex*");
	//qRegisterMetaType<QString>("QString");

	connect(this, &ThumbnailThread::ThumbnailThreadSignalLoadThumbnail,
		m_pThumbnailWorker, &ThumbnailWorker::ThumbnailWorkerLoadThumbnail);
	connect(m_pThumbnailWorker, &ThumbnailWorker::ThumbnailWorkerSignalResultLoadThumbnail,
		this, &ThumbnailThread::ThumbnailThreadResultLoadThumbnail);

	exec();


}

void ThumbnailThread::executeThreadLoadThumbnail(QString imagePath, QMutex* pMutex)
{
	emit ThumbnailThreadSignalLoadThumbnail(imagePath, pMutex);
}
