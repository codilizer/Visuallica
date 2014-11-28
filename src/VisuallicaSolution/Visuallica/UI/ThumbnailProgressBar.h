#ifndef THUMBNAILPROGRESSBAR_H
#define THUMBNAILPROGRESSBAR_H

#include <QProgressBar>
#include "Observer.h"

class ThumbnailProgressManager;

class ThumbnailProgressBar : public QProgressBar, public Observer
{
	Q_OBJECT

public:
	ThumbnailProgressBar(ThumbnailProgressManager* pManager, QWidget *parent = 0);
	~ThumbnailProgressBar();


protected:
	virtual void Update(Subject * theChangeSubject);

private:

	ThumbnailProgressManager* m_pThumbnailProgressManager;
};



#endif //THUMBNAILPROGRESSBAR_H