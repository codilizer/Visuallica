#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>

using namespace std ; 
class Observer;

class Subject 
{ 
public: 
	Subject() {}; 
	virtual ~Subject() {}; 
	virtual void Attach(Observer*); 
	virtual void Detach(Observer*); 
	virtual void Notify();  
private: 
	vector<Observer*> _observers; 
}; 



#endif // SUBJECT_H
