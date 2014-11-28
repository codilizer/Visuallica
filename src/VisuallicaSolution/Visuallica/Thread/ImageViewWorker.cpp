#include "ImageViewWorker.h"
//#include <QPixmap>
#include "BPixmap.h"
#include <QDebug>
#include "BDcm.h"
#include "ImagePixmap.h"
#include "DcmPixmap.h"
#include <QMutex>


ImageViewWorker::ImageViewWorker(QObject *parent)
{


}

ImageViewWorker::~ImageViewWorker()
{

}

void ImageViewWorker::ImageViewWorkerLoadImage(QString path, QMutex *pMutex)
{
	pMutex->lock();
	qDebug() << "ImageViewWorkerLoadImage Invoked! : [" + path + "]";
	ImagePixmap *pNewImage = new ImagePixmap();

	if (pNewImage->load(path))
	{
		qDebug() << "Image loaded : [" + path + "]";
		emit ImageViewWorkerSignalResultLoadImage(pNewImage, path);
	}
	else // dcm
	{
		if (pNewImage)
		{
			delete pNewImage;
			pNewImage = NULL;
		}

		DcmPixmap* pNewDcmImage = new DcmPixmap();

		BDcm::GetInstance().Load(pNewDcmImage, path);
		emit ImageViewWorkerSignalResultLoadImage(pNewDcmImage, path);
	}
	pMutex->unlock();
}