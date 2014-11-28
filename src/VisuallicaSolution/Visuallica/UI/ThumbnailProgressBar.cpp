#include "ThumbnailProgressBar.h"
#include "ThumbnailProgressManager.h"

ThumbnailProgressBar::ThumbnailProgressBar(ThumbnailProgressManager* pManager, QWidget *parent)
	: QProgressBar(parent)
{
	setMinimum(0);
	setMaximum(100);

	m_pThumbnailProgressManager = pManager;
	m_pThumbnailProgressManager->Attach(this);
}
ThumbnailProgressBar::~ThumbnailProgressBar()
{
}

void ThumbnailProgressBar::Update(Subject * theChangeSubject)
{
	if (E_PROGRESS == m_pThumbnailProgressManager->GetState())
	{
		if (!isVisible())
		{
			setVisible(true);
		}

		setValue(m_pThumbnailProgressManager->GetProgressValue());
	}
	else if (E_NONE == m_pThumbnailProgressManager->GetState())
	{
		if (isVisible())
		{
			setVisible(false);
			reset();
		}
	}
}