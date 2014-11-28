#include "MultiViewManager.h"
#include <QDebug>


MultiViewManager::MultiViewManager(QObject *parent)
	: QObject(parent)
{
	m_iAskedMultiViewRowCount = 0;
	m_iAskedMultiViewColumnCount = 0;
}

MultiViewManager::~MultiViewManager()
{

}

void MultiViewManager::Attach(Observer* pObserver)
{
	qDebug() << "MultiViewManager::Attach() invoked!!";
	Subject::Attach(pObserver);

	Notify();
}



void MultiViewManager::AskChangeMultiView(int aRowCount, int aColumnCount)
{
	qDebug() << "MultiViewManager::AskChangeMultiView() invoked!!";
	m_iAskedMultiViewRowCount = aRowCount;
	m_iAskedMultiViewColumnCount = aColumnCount;

	Notify();
}

void MultiViewManager::GetAskedMultiViewInfo(int& aRowCount, int& aColumnCount)
{
	aRowCount = m_iAskedMultiViewRowCount;
	aColumnCount = m_iAskedMultiViewColumnCount;
}