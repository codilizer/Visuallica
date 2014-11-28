#include "StateManager.h"
#include <QDebug>

StateManager::StateManager(QObject *parent)
	: QObject(parent)
{

}

StateManager::~StateManager()
{

}

void StateManager::Attach(Observer* pObserver)
{
	qDebug() << "StateManager::Attach() invoked!!";
	Subject::Attach(pObserver);

	Notify();
}

void StateManager::SetBehaviorMode(E_BEHAVIORMODE aBehaviorMode)
{
	qDebug() << "StateManager::SetBehaviorMode() invoked!!";
	m_behaviorMode = aBehaviorMode;

	emit BehaviorModeStateChanged(aBehaviorMode);
	Notify();
}

E_BEHAVIORMODE StateManager::GetBehaviorMode()
{
	qDebug() << "StateManager::GetBehaviorMode() invoked!!";
	return m_behaviorMode;	
}

void StateManager::NewImageLoaded()
{
	//SetViewMode(m_ePrevSettedViewMode);
	//SetBehaviorMode(PICK);
}

