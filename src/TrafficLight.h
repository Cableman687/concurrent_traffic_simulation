#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"
using namespace std;
enum TrafficLightPhase{red,green};

// forward declarations to avoid include cycle
class Vehicle;



// FP.3 Define a class „MessageQueue“ which has the public methods send and receive.  (DONE)
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. (DONE)
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase.  (DONE)
// Also, there should be an std::condition_variable as well as an std::mutex as private members.  (DONE)

template <class T> // "Templatised Class" meaning we will be able to take arbitrary pieces of information E.g. Traffic Light Phases
class MessageQueue
{
public:

void send (T &&phase);
deque<TrafficLightPhase> _queue;
T receive();


private:
condition_variable _condition;
mutex mtx;

    
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. (DONE)
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ (DONE)
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that  (DONE)
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. (DONE)
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 



class TrafficLight : public TrafficObject
{
public:
    // constructor / deconstructor
    TrafficLight():_currentPhase(red){};
    ~TrafficLight(){};

    // getters / setters
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
    void setCurrentPhase(TrafficLightPhase phase);

    // typical behaviour methods
   


private:
    // typical behaviour methods
    void cycleThroughPhases();
    

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.

    condition_variable _condition;
    mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> message_queue;
};

#endif