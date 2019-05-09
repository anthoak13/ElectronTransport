/* Implementation of the particle class
 *
 * Adam Anthony
 */
#include "Particle.h"


//Constructor. Takes mass in units eV/c^2 and charge in units of e
Particle::Particle(double mass, double charge)
{
  std::cout << "Creating particle with mass " << mass * evToKg
	    << " kg and charge " << charge * e << " C" << std::endl;
  this->mass = mass * evToKg;
  this->charge = charge * e;
}

Particle::~Particle() { }

void Particle::setVelocity(double velIn[3])
{
  std::cout << "Setting initial velocity to: ";
  for( int i = 0; i < 3; ++i)
  {
    vel[i] = velIn[i];
    std::cout << vel[i] << ", ";
  }
  std::cout << std::endl;
}

void Particle::setPosition(double posIn[3])
{
  std::cout << "Setting initial position to: ";
  for( int i = 0; i < 3; ++i)
  {
    pos[i] = posIn[i];
    std::cout << pos[i] << ", ";
  }
  std::cout << std::endl;
}

double* Particle::getVelocity(double t)
{
  double *ret = new double[3];
  for( int i = 0; i < 3; ++i )
    ret[i] = vel[i];
  return ret;
}

double* Particle::getVelocity(double t, ConstField *field)
{
  //Create array to return 
  double *ret = new double[3];

  //Setup the integration constants given I.C.s
  const double w = -e*field->getB()/mass;
  const double D1 = -vel[2]/w;
  const double D2 = 1./w*( vel[1] - field->getE()/field->getB() );
  const double D3 = pos[1] - D1;
  const double D4 = -D2;
  
  //X velocity is unchanged 
  ret[0] = vel[0];

  // set y velocity
  ret[1] =
    field->getE()/field->getB()
    - D1*w*TMath::Sin(w*t)
    + D2*w*TMath::Cos(w*t);

  // set z velocity
  ret[2] =
    - D2*w*TMath::Sin(w*t)
    - D1*w*TMath::Cos(w*t);
  
  return ret;
}

double* Particle::getPosition(double t)
{
  double *ret = new double[3];
  for( int i = 0; i < 3; ++i )
    ret[i] = vel[i] * t;
  return ret;
}

double* Particle::getPosition(double t, ConstField *field)
{
  //Create array to return 
  double *ret = new double[3];

  //Setup the integration constants given I.C.s
  const double w = -e*field->getB()/mass;
  const double D1 = -vel[2]/w;
  const double D2 = 1./w*( vel[1] - field->getE()/field->getB() );
  const double D3 = pos[1] - D1;
  const double D4 = -D2;
  
  //X velocity is unchanged 
  ret[0] = vel[0]*t;

  // Get the Y position
  ret[1] =
    field->getE()/field->getB()*t
    + D1*TMath::Cos(w*t)
    + D2*TMath::Sin(w*t)
    + D3;
  
  //Get the Z position
  ret[2] =
    D2*TMath::Cos(w*t)
    - D1*TMath::Sin(w*t)
    + D4;

  return ret;
}
