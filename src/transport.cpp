/* Program to simulate the transport of electrons in a 
 * constant ExB field.
 * Assumptions:
 * \hat{E} = \hat{z}
 * \hat{B} = \hat{x}
 * x(t) = x(0) (translationaly invarient)
 * z(0) = 0 (can always redefine coords so true)
 *
 * Input:
 * 
 * ./transport En V B
 * En is electrons initial energy in eV
 * V is the voltage applied to the MCP
 * B is the B-field in gauss
 * 
 * Note: Requires GCC version 5+, default on fishtank is 4.9.2
 * Can load acceptable version with "module load gnu/gcc/6.5"
 * Adam Anthony 5/2/2019
 */

#include <cmath>
#include <iostream>
#include <string>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>

//Function definitions
constexpr
double pow(double base, int exp) noexcept
{
  double result = 1.0;
  if (exp > 0)
    for (int i = 0; i < exp; ++i)
      result *= base;
  else
    for (int i = 0; i < -exp; ++i)
      result /= base;
  return result;
}

//Define fundemental constants
constexpr auto e = 1.6021766208 * pow(10, -19);
constexpr auto m = 9.10938356   * pow(10,-31);
constexpr auto c = 299792458;
constexpr auto kgToEv = 5.60958865 * pow(10,35);

double yPos(double E, double B, double t,
	    double *pos, double *vel);
double zPos(double E, double B, double t,
	    double *pos, double *vel);
void usage();

//Main function
int main(int argc, char *argv[])
{
  TApplication app("app", &argc, argv);

  if(argc != 5)
  {
    usage();
    return 1;
  }
  
  
  
  //Set the initial conditions
  double V     = std::stod(argv[2]); //Voltage in volts
  double B     = std::stod(argv[3]); //B-field in gaus
  double En    = std::stod(argv[1]); //in eV
  double VFoil = std::stod(argv[4]); //Voltage on the foil

  //Add the energy from accelerating to the foil
  double EnAtGND = En +VFoil;
  
  //Get the electron velocity from KE
  double v = TMath::Sqrt(2*EnAtGND/(m*kgToEv))*c;
  double pos[3] = {0,0,0};
  double vel[3] = {0,0, -v};
  double E = V/(1.6*2.54/100.);
  B *= pow(10,-4);

  //Print out the speed of the electons
  std::cout <<"Electron moving at " << v << "m/s" << std::endl;

  std::cout << E << " " << B << std::endl;
  
  //Run from 0 to 100 ns
  int nSample = 1000;
  Double_t z[nSample], y[nSample], t[nSample]; 

  //Found collision
  bool col = false;

  //Loop through and populate positions, also look for colision location
  for(int i = 0; i < nSample; ++i)
  {
    //t runs from 0 to 100 ns
    t[i] = i*pow(10,-10);
    y[i] = yPos(E,B,t[i],pos,vel);
    z[i] = -zPos(E,B,t[i],pos,vel);

    if( !col && z[i] < 0 )
    {
      col = true;
      std::cout << "Hit wall at " << y[i]*100 << " cm in " << t[i]*pow(10,9) << " ns"
		<< std::endl;
    }
  }

  //Create the canvas to hold the graph
  TCanvas *c1 = new TCanvas("c1", "Top-down projection", 500, 300);
  //Create the graph and draw it
  TGraph *gr = new TGraph(100,y,z);
  gr->SetTitle(Form("En: %f, V: %f, VFoil: %f, B: %f", En, V, VFoil, B));
  gr->Draw("AC*");
  
  app.Run(kTRUE);

  return 0;
}

double yPos(double E, double B, double t,
	    double *pos, double *vel)
{
  //Setup the constant for the calculation
  const double w = -e*B/m;
  const double D1 = -vel[2]/w;
  const double D2 = 1./w*(vel[1] - E/B);
  const double D3 = pos[1] - D1;
  const double D4 = -D2;

  return E/B*t + D1*TMath::Cos(w*t) + D2*TMath::Sin(w*t) + D3;
}

double zPos(double E, double B, double t,
	    double *pos, double *vel)
{
  //Setup the constant for the calculation
  const double w = -e*B/m;
  const double D1 = -vel[2]/w;
  const double D2 = 1./w*(vel[1] - E/B);
  const double D3 = pos[1] - D1;
  const double D4 = -D2;

  return D2*TMath::Cos(w*t) - D1*TMath::Sin(w*t) + D4;
}

void usage()
{
  std::cout << "./transport En VMCP B VFoil" << std::endl;
  std::cout << "En is energy of electron in ev" << std::endl;
  std::cout << "VMCP is voltage across the MCP in V" << std::endl;
  std::cout << "B is the magnetic field in gauss" << std::endl;
  std::cout << "VFoil is voltage on the foil in -V" << std::endl;

}
