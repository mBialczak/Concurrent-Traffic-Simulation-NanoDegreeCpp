#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

// forward declarations to avoid include cycle
class Vehicle;

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

// enum class describing the traffic light state on the current instersection
enum class TrafficLightPhase : short { red, green };

class TrafficLight : public TrafficObject {
  public:
  // constructor
  TrafficLight();

  // holds execution of when called until green light is set
  void waitForGreen();
  // runs a thread continuously changing the traffic lights phase
  void simulate();
  // return the current phase of traffic light
  TrafficLightPhase getCurrentPhase();

  private:
  // in an infinite loop changes the _currentPhase of traffic light and sends
  // update to the member MessageQueue with a random cycle of 4-6 seconds.
  void cycleThroughPhases();

  // gets random interval (in seconds) for light switching
  int getRandomInterval(int min = 4, int max = 6) const;
  // the queue handling the messages of lightPhase changes
  MessageQueue<TrafficLightPhase> _queue;
  // variable used for locking while queue is handling lightPhase changes
  // messages
  std::condition_variable _condition;
  std::mutex _mutex;
  TrafficLightPhase _currentPhase;
};

#endif