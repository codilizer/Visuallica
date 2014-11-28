#ifndef OBSERVER_H
#define OBSERVER_H


class Subject; 

class Observer 
{ 
public: 
 Observer() {}; 
 ~Observer() {}; 
 virtual void Update(Subject* theChangeSubject) = 0; 
}; 


#endif // OBSERVER_H