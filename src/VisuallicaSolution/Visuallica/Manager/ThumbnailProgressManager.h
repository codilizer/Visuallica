#ifndef THUMBNAILPROGRESSMANAGER_H
#define THUMBNAILPROGRESSMANAGER_H

#include "Subject.h"


enum E_PROGRESSSTATE  { E_NONE, E_PROGRESS };

class ThumbnailProgressManager : public Subject
{
public:
	static ThumbnailProgressManager& GetInstance()
	{
		static ThumbnailProgressManager instance;
		return instance;
	}

	virtual void Attach(Observer*);

	void Start();
	void Progress(int val);
	void Finish();

	E_PROGRESSSTATE GetState();
	int GetProgressValue();

protected:

private:
	ThumbnailProgressManager();
	~ThumbnailProgressManager();

	E_PROGRESSSTATE m_state;
	int m_progressValue;
};



#endif //THUMBNAILPROGRESSMANAGER_H