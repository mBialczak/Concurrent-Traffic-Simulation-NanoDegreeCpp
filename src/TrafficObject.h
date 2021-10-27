#ifndef TRAFFICOBJECT_H
#define TRAFFICOBJECT_H

#include <mutex>
#include <thread>
#include <vector>

enum ObjectType {
  noObject,
  objectVehicle,
  objectIntersection,
  objectStreet,
};

class TrafficObject {
  public:
  // constructor / desctructor
  TrafficObject();
  virtual ~TrafficObject();

  // getter and setter
  int getID() { return _id; }
  void setPosition(double x, double y);
  void getPosition(double& x, double& y);
  ObjectType getType() { return _type; }

  // typical behaviour methods
  virtual void simulate() {};

  protected:
  // identifies the class type
  ObjectType _type;
  // every traffic object has its own unique id
  int _id;
  // traffic object position in pixels
  double _posX, _posY;
  // holds all threads that have been launched within this object
  std::vector<std::thread> threads;
  // mutex shared by all traffic objects for protecting cout
  static std::mutex _mtx;

  private:
  // static variable for counting object ids
  static int _idCnt;
};

#endif