#include "TrafficLight.h"
#include <chrono>
#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

// returns first message waiting in the queue
template <typename T>
T MessageQueue<T>::receive()
{
  // lock the queue for the modification
  std::unique_lock<std::mutex> uLock(_mutex);
  //_cond.wait() holds execution until new message can be retrieved from the
  // queue
  // lambda protects against spurious notifies and  ensures proceeding only when
  // queue is non-empty
  _cond.wait(uLock, [this] { return !_messages.empty(); });
  // remove first element from queue
  T msg = std::move(_messages.front());
  _messages.clear();

  return msg;
}

// stores new message in the queue and nofifies queue client that new message
// has been received
template <typename T>
void MessageQueue<T>::send(T&& msg)
{
  // lock the queue for the modification
  std::lock_guard<std::mutex> lock(_mutex);
  // add the message to the end of the queue
  _messages.emplace_back(std::move(msg));
  // notifies the receive function that new message has been added to the queue
  // and is pending for processing
  _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

// initialization of static random device
std::random_device TrafficLight::rdev;

// initialization of static random engine generator
std::mt19937 TrafficLight::reng { TrafficLight::rdev() };

// initialization of static uniform int distribution
std::uniform_int_distribution<> TrafficLight::distrib(4000, 6000);

TrafficLight::TrafficLight()
    : _currentPhase(TrafficLightPhase::red)

{
}

// holds execution of when called until green light is set
void TrafficLight::waitForGreen()
{
  while (_queue.receive() != TrafficLightPhase::green) { }
  return;
}

// return the current phase of traffic light
TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

// runs a thread continuously changing the traffic lights phase
void TrafficLight::simulate()
{
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// in an infinite loop changes the _currentPhase of traffic light and sends
// update to the member MessageQueue with a random cycle of 4-6 seconds.
void TrafficLight::cycleThroughPhases()
{
  // initialize stop watch
  auto lastUpdate = std::chrono::system_clock::now();
  // interval of changing light phase (random 4-6 seconds)
  int switchInterval = getRandomInterval();
  long timeSinceLastUpdate {};
  while (true) {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // calculate time difference
    timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - lastUpdate)
                              .count();

    // compare the cycle time with a random interval of 4-6 s (in milliseconds)
    if (timeSinceLastUpdate >= switchInterval) {
      TrafficLightPhase newPhase = getCurrentPhase() == TrafficLightPhase::green
          ? TrafficLightPhase::red
          : TrafficLightPhase::green;

      // update current phase of the traffic light
      _currentPhase = newPhase;

      // send lightPhase modification message to the queue
      _queue.send(std::move(newPhase));

      // reset stopwatch
      lastUpdate = std::chrono::system_clock::now();
    }
  }
}

// static function returning random interval for light switching
int TrafficLight::getRandomInterval() { return distrib(reng); }
