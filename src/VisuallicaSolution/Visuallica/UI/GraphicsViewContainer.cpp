#include "GraphicsViewContainer.h"
#include "BGraphicsView.h"
#include <QHBoxLayout>
#include "MultiViewManager.h"
#include "CommonDefines.h"
#include "StateManager.h"

GraphicsViewContainer::GraphicsViewContainer(MultiViewManager *pMultiViewManager, QWidget *parent)
	: QWidget(parent)
{
	m_iRowCount = 1;
	m_iColumnCount = 1;

	m_iCountGraphicsView = m_iRowCount * m_iColumnCount;

	initLayout();

	m_pMultiViewManager = pMultiViewManager;
	m_pMultiViewManager->Attach(this);
}


GraphicsViewContainer::~GraphicsViewContainer()
{

}

void GraphicsViewContainer::Update(Subject * paChangeSubject)
{
	if (paChangeSubject == m_pMultiViewManager)
	{
		int iAskedRowCount = 0;
		int iAskedColumnCount = 0;

		m_pMultiViewManager->GetAskedMultiViewInfo(iAskedRowCount, iAskedColumnCount);

		if (iAskedRowCount <= 0 || iAskedColumnCount <= 0)
			return;

		changeMultiView(iAskedRowCount, iAskedColumnCount);
		StateManager::GetInstance().SetBehaviorMode(PICK);
	}

}

void GraphicsViewContainer::initLayout()
{
	m_pGridLayout = new QGridLayout(this);

	setLayout(m_pGridLayout);

	m_pGraphicsView = new BGraphicsView*[MAX_GRAPHICS_VIEW_ROW_COUNT * MAX_GRAPHICS_VIEW_COLUMN_COUNT];
	
	for (int i = 0; i < MAX_GRAPHICS_VIEW_ROW_COUNT * MAX_GRAPHICS_VIEW_COLUMN_COUNT; i++)
	{
		*(m_pGraphicsView + i) = NULL;
	}


	for (int i = 0; i < m_iCountGraphicsView; i++)
	{
		*(m_pGraphicsView + i) = new BGraphicsView(&StateManager::GetInstance(), this);		
	}

	int indexWidget = 0;
	for (int row = 0; row < m_iRowCount; row++)
	{
		for (int col = 0; col < m_iColumnCount; col++)
		{
			m_pGridLayout->addWidget(*(m_pGraphicsView + indexWidget), row, col);
			indexWidget++;
		}
	}
}

void GraphicsViewContainer::changeMultiView(int aAskedRowCount, int aAskedColumnCount)
{
	int iAskedCountGraphicsView = aAskedRowCount * aAskedColumnCount;

	for (int i = 0; i < m_iCountGraphicsView; i++)
	{
		m_pGridLayout->removeWidget(*(m_pGraphicsView + i));
	}

	for (int i = 0; i < m_iCountGraphicsView; i++)
	{
		if (*(m_pGraphicsView + i))
		{
			delete *(m_pGraphicsView + i);
			*(m_pGraphicsView + i) = NULL;
		}
	}


	for (int i = 0; i < iAskedCountGraphicsView; i++)
	{
		*(m_pGraphicsView + i) = new BGraphicsView(&StateManager::GetInstance(), this);
	}
	
	m_iCountGraphicsView = iAskedCountGraphicsView;

	int indexWidget = 0;
	for (int row = 0; row < aAskedRowCount; row++)
	{
		for (int col = 0; col < aAskedColumnCount; col++)
		{
			m_pGridLayout->addWidget(*(m_pGraphicsView + indexWidget), row, col);
			indexWidget++;
		}
	}

	m_iRowCount = aAskedRowCount;
	m_iColumnCount = aAskedColumnCount;


}

