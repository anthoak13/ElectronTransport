#ifndef CONSTFIELD_H
#define CONSTFIELD_H
// Class to hold the field values for a constant
// ExB field.
// Will also propagate a charged particle through the field
// Take coordinate system so B=B\hat{x} and E=E\hat{z}

#include <iostream>

class ConstField
{
private:
  double E;
  double B;

public:
  ConstField(double E, double B);
  ~ConstField();

  double getE() { return E; }
  double getB() { return B; }

  double getE(double x, double y, double z) { return E; }
  double getB(double x, double y, double z) { return E; }
    
};
#endif

