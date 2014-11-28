#include "ThumbnailWorker.h"
#include "ThumbnailThread.h"
#include <QPixmap>
#include <QDebug>
#include "BDcm.h"
#include "ImagePixmap.h"


ThumbnailWorker::ThumbnailWorker(QObject *parent)
{


}

ThumbnailWorker::~ThumbnailWorker()
{


}

void ThumbnailWorker::ThumbnailWorkerLoadThumbnail(QString path, QMutex *pMutex)

{
	pMutex->lock();
	qDebug() << "ThumbnailWorkerLoadThumbnail Invoked! : [" + path + "]";

	ImagePixmap *pNewImage = new ImagePixmap();
	if (pNewImage->load(path))
	{
		qDebug() << "Image loaded : [" + path + "]";
		emit ThumbnailWorkerSignalResultLoadThumbnail(pNewImage, path);
	}
	else
	{
		if (pNewImage)
		{
			delete pNewImage;
			pNewImage = NULL;
		}

		ImagePixmap *pNewImage = new ImagePixmap();

		BDcm::GetInstance().Load(pNewImage, path);
		emit ThumbnailWorkerSignalResultLoadThumbnail(pNewImage, path);
	}
	pMutex->unlock();
}