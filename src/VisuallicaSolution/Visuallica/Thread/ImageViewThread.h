#ifndef IMAGEVIEWTHREAD_H
#define IMAGEVIEWTHREAD_H

#include <QtCore>
#include <QThread>
#include <QString>

class ImageViewWorker;
class BPixmap;

class ImageViewThread : public QThread
{
	Q_OBJECT

public:
	ImageViewThread();
	~ImageViewThread();


public:
	void executeThreadLoadImage(QString imagePath, QMutex *pMutex);
signals:
	void ImageViewThreadSignalLoadImage(QString imagePath, QMutex *pMutex);
	void ImageViewThreadResultLoadImage(BPixmap* image, QString path);


protected:
	virtual void run();

private:
	ImageViewWorker * m_pImageViewWorker;


};

#endif // IMAGEVIEWTHREAD_H