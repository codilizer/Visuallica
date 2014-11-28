#include "ImageViewThread.h"
#include "ImageViewWorker.h"
#include "BPixmap.h"
#include <QtCore>

ImageViewThread::ImageViewThread() : QThread()
{


}

ImageViewThread::~ImageViewThread()
{

}

void ImageViewThread::run()
{
	m_pImageViewWorker = new ImageViewWorker();

	qRegisterMetaType<BPixmap*>("BPixmap*");

	connect(this, &ImageViewThread::ImageViewThreadSignalLoadImage,
		m_pImageViewWorker, &ImageViewWorker::ImageViewWorkerLoadImage);
	connect(m_pImageViewWorker, &ImageViewWorker::ImageViewWorkerSignalResultLoadImage,
		this, &ImageViewThread::ImageViewThreadResultLoadImage);

	exec();

}

void ImageViewThread::executeThreadLoadImage(QString imagePath, QMutex *pMutex)
{
	emit ImageViewThreadSignalLoadImage(imagePath, pMutex);
}