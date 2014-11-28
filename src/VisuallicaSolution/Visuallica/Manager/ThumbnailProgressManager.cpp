#include "ThumbnailProgressManager.h"

ThumbnailProgressManager::ThumbnailProgressManager()
{
	m_state = E_NONE;
	m_progressValue = 0;
}

ThumbnailProgressManager::~ThumbnailProgressManager()
{

}

void ThumbnailProgressManager::Attach(Observer* pObserver)
{
	Subject::Attach(pObserver);

	Notify();
}

void ThumbnailProgressManager::Start()
{
	m_state = E_PROGRESS;
	m_progressValue = 0;

	Notify();
}

void ThumbnailProgressManager::Progress(int val)
{
	if (E_NONE == m_state)
		m_state = E_PROGRESS;

	m_progressValue = val;
	Notify();
}

void ThumbnailProgressManager::Finish()
{
	m_state = E_NONE;
	Notify();
}

E_PROGRESSSTATE ThumbnailProgressManager::GetState()
{
	return m_state;
}

int ThumbnailProgressManager::GetProgressValue()
{
	return m_progressValue;
}