#ifndef PARTICLE_H
#define PARTICLE_H
// Particle class that can propogate through a Field
#include <iostream>

#include <TMath.h>

#include "Constants.hh"
#include "ConstField.h"


class Particle
{
private:
  //Internal units are SI
  double mass; //mass in kg
  double charge; //charge in C

  //Declared on the stack because these are essentially constant
  double pos[3];
  double vel[3];

public:
  //Takes mass in units ev/c^2 and charge in units e
  Particle(double mass, double charge);
  ~Particle();

  //Set the velocity and position at t = 0
  void setVelocity(double vel[3]);
  void setPosition(double pos[3]);

  // Get the velocity at some later time under influence of
  // field
  double* getVelocity(double t);
  double* getVelocity(double t, ConstField *field);

  // Get the position at some later time under influence of
  // field
  double* getPosition(double t);
  double* getPosition(double t, ConstField *field);
};

#endif
