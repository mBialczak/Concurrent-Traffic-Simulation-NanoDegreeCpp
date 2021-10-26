#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

// forward declarations to avoid include cycle
class Vehicle;

// FP.3 Define a class „MessageQueue“ which has the public methods send and
// receive. Send should take an rvalue reference of type TrafficLightPhase
// whereas receive should return this type. Also, the class should define an
// std::dequeue called _queue, which stores objects of type TrafficLightPhase.
// Also, there should be an std::condition_variable as well as an std::mutex as
// private members.

template <typename T>
class MessageQueue {
  public:
  // returns first message waiting in the queue
  T receive();
  // stores new message in the queue and nofifies queue client that new message
  // has been received
  void send(T&& msg);

  private:
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _messages;
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject.
// The class shall have the public methods „void waitForGreen()“ and „void
// simulate()“ as well as „TrafficLightPhase getCurrentPhase()“, where
// TrafficLightPhase is an enum that can be either „red“ or „green“. Also, add
// the private method „void cycleThroughPhases()“. Furthermore, there shall be
// the private member _currentPhase which can take „red“ or „green“ as its
// value.

// enum class describing the traffic light state on the current instersection
enum class TrafficLightPhase { red, green };
// REVIEW: verify if better to  be put inside the class

class TrafficLight : public TrafficObject {
  public:
  // TODO: verify constructor
  // constructor / desctructor
  TrafficLight();

  // getters / setters

  // typical behaviour methods
  // TODO: verify which functions can be const
  void waitForGreen();
  void simulate();
  TrafficLightPhase getCurrentPhase();

  private:
  // typical behaviour methods

  // FP.4b : create a private member of type MessageQueue for messages of type
  // TrafficLightPhase and use it within the infinite loop to push each new
  // TrafficLightPhase into it by calling send in conjunction with move
  // semantics.
  void cycleThroughPhases();

  // gets random interval for light switching
  int getRandomInterval(int min = 4, int max = 6) const;

  MessageQueue<TrafficLightPhase> _queue;
  std::condition_variable _condition;
  std::mutex _mutex;
  TrafficLightPhase _currentPhase;
};

#endif