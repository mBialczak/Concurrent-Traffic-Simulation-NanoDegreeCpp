#include "TrafficLight.h"
#include <chrono>
#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

// returns first message waiting in the queue
template <typename T>
T MessageQueue<T>::receive()
{
  // FP.5a : The method receive should use std::unique_lock<std::mutex> and
  // _condition.wait()
  // to wait for and receive new messages and pull them from the queue using
  // move semantics.
  // The received object should then be returned by the receive function.

  // lock the queue for the modification
  std::unique_lock<std::mutex> uLock(_mutex);
  // pass unique lock to condition variable
  // lambda protects against spurious notifies and  ensures proceeding only when
  // queue is non-empty
  _cond.wait(uLock, [this] { return !_messages.empty(); });
  // remove first element from queue
  T msg = std::move(_messages.front());
  _messages.pop_front();

  return msg;
}

// stores new message in the queue and nofifies queue client that new message
// has been received
template <typename T>
void MessageQueue<T>::send(T&& msg)
{
  // FP.4a : The method send should use the mechanisms
  // std::lock_guard<std::mutex> as well as _condition.notify_one() to add a new
  // message to the queue and  afterwards send a notification.

  // lock the queue for the modification
  std::lock_guard<std::mutex> lock(_mutex);
  // add the message to the end of the queue
  _messages.emplace_back(std::move(msg));
  // notifies the receive function that new message has been added to the queue
  // and is pending for processing
  _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
    : _currentPhase(TrafficLightPhase::red)
{
}

void TrafficLight::waitForGreen()
{
  // FP.5b : add the implementation of the method waitForGreen, in which an
  // infinite while-loop runs and repeatedly calls the receive function on the
  // message queue. Once it receives TrafficLightPhase::green, the method
  // returns.
  while (_queue.receive() != TrafficLightPhase::green) { }
  return;
}

// TODO: verify if current phase should be locked of picked from the message
// queue
// REVIEW: think if locking is needed
TrafficLightPhase TrafficLight::getCurrentPhase()
{
  // lock  _currentPhase for reading
  std::lock_guard<std::mutex> lightLock(_mutex);
  return _currentPhase;
}

void TrafficLight::simulate()
{
  // FP.2b : Finally, the private method „cycleThroughPhases“ should be started
  // in a thread when the public method „simulate“ is called. To do this, use
  // the thread queue in the base class.
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// REVIEW: virtual?
// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
  // FP.2a : Implement the function with an infinite loop that measures the time
  // between two loop cycles and toggles the current phase of the traffic light
  // between red and green and sends an update method to the message queue using
  // move semantics. The cycle duration should be a random value between 4 and 6
  // seconds. Also, the while-loop should use std::this_thread::sleep_for to
  // wait 1ms between two cycles.

  // initialize stop watch
  auto lastUpdate = std::chrono::system_clock::now();

  while (true) {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // calculate time difference
    long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now() - lastUpdate)
                                   .count();

    if (timeSinceLastUpdate >= getRandomInterval()) {

      TrafficLightPhase newPhase = getCurrentPhase() == TrafficLightPhase::green
          ? TrafficLightPhase::red
          : TrafficLightPhase::green;

      // TODO: Verify if switch should be here or elsewhere
      // REVIEW: think if locking is needed
      // lock  for modification of _currentPhase
      {
        std::lock_guard<std::mutex> lightLock(_mutex);
        _currentPhase = newPhase;
      }

      // send lightphase modification message to the queue
      _queue.send(std::move(newPhase));

      // reset stopwatch
      lastUpdate = std::chrono::system_clock::now();
    }
  }
}

// gets random interval for light switching
int TrafficLight::getRandomInterval(int min, int max) const
{
  std::random_device rdev;
  std::default_random_engine reng { rdev() };
  std::uniform_int_distribution<> distrib(min, max);

  return distrib(reng);
}
