#ifndef IMAGEVIEWWORKER_H
#define IMAGEVIEWWORKER_H

#include <QtCore>

class BPixmap;
class QMutex;

class ImageViewWorker : public QObject
{
	Q_OBJECT

public:
	ImageViewWorker(QObject *parent = 0);
	~ImageViewWorker();

public slots:
	void ImageViewWorkerLoadImage(QString path, QMutex *pMutex);
signals:
	void ImageViewWorkerSignalResultLoadImage(BPixmap* image, QString path);

};


#endif // IMAGEVIEWWORKER_H