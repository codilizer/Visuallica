#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "Subject.h"
#include <QObject>


static enum E_BEHAVIORMODE {FITWINDOW, ORG_RESOLUTION, NORMAL, PICK, PAN, 
							ZOOM, WINDOW, REVERSE_VERTICAL, REVERSE_HORIZONTAL,
							ROTATE_LEFT, ROTATE_RIGHT, RESTORE_MATRIX };


class StateManager : public QObject, public Subject
{
	Q_OBJECT
public:
	static StateManager& GetInstance()
	{
		static StateManager instance;
		return instance;
	}

	virtual void Attach(Observer*);
	void SetBehaviorMode(E_BEHAVIORMODE);
	E_BEHAVIORMODE GetBehaviorMode();

	void NewImageLoaded();
	
signals:
	void BehaviorModeStateChanged(E_BEHAVIORMODE);


private:
	StateManager(QObject *parent = 0);
	~StateManager();

	E_BEHAVIORMODE m_behaviorMode;
};

#endif //STATEMANAGER_H