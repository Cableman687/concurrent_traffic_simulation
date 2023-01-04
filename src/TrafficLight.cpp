#include <iostream>
#include <random>
#include "TrafficLight.h"
#include<thread>
#include<chrono>
#include<mutex>
#include<string>
using namespace std;

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()  (DONE)
    // to wait for and receive new messages and pull them from the queue using move semantics.  (DONE)
    // The received object should then be returned by the receive function. (DONE)

    unique_lock<std::mutex> lock(mtx);

    _condition.wait(lock,[this]{return !_queue.empty();});
    TrafficLightPhase returnMessage = move(_queue.front());
    return returnMessage;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>  (DONE)
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    lock_guard<mutex> lockg(mtx);
    _queue.emplace_back(msg);
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */


void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop  (DONE)
    // runs and repeatedly calls the receive function on the message queue. (DONE)
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true){
        TrafficLightPhase currentPhase = message_queue.receive();
        if(currentPhase == green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::setCurrentPhase(TrafficLightPhase phase){
    _currentPhase = phase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.  (DONE)
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles (DONE)
    // and toggles the current phase of the traffic light between red and green and sends an update method (DONE)
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. (DONE)
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. (DONE)
    
    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution<> dist(4000,6000);

    while(true)
    {
        if (_currentPhase == red){
            this_thread::sleep_until(chrono::system_clock::now() + chrono::milliseconds(dist(gen)));
            setCurrentPhase(green);
            
        } else {
            this_thread::sleep_until(chrono::system_clock::now() + chrono::milliseconds(dist(gen)));
            setCurrentPhase(red);
            

        }
        this_thread::sleep_for(chrono::milliseconds(1));

        message_queue.send(move(getCurrentPhase()));
    }

}

