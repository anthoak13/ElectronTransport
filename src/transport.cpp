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

#include <iostream>
#include <string>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TString.h>

#include "Constants.hh"
#include "ConstField.h"
#include "Particle.h"


//Define fundemental constants
auto m = 9.10938356   * pow(10,-31);

void usage();

//Main function
int main(int argc, char *argv[])
{
  TApplication app("app", &argc, argv);

  //Make sure the user passed the right number of argumetns
  if(argc != 5)
  {
    usage();
    return 1;
  }
  
  //Parse the comand line arguments
  double V     = std::stod(argv[2]); //Voltage in volts
  double B     = std::stod(argv[3]); //B-field in gaus
  double En    = std::stod(argv[1]); //in eV
  double VFoil = std::stod(argv[4]); //Voltage on the foil

  //Get the E anf B field from the geometry
  double E = V/(1.6*2.54/100.);
  B *= pow(10,-4);

  //Create the field and the particle
  ConstField *field = new ConstField(E,B);

  //Create an electron with mass 511keV and charge 1e
  Particle electron(511000, -1);

  /*** This makes an assumtion that the particle is emited perpendicular to
   *   the foil. This is wrong and an angular distribution will have
   *   to be added later
   ***/
  //Add the energy from accelerating to the foil
  double EnAtGND = En + VFoil;
  //Get the electron velocity from KE
  double v = TMath::Sqrt(2*EnAtGND/(m*kgToEv))*c;

  //Set the timestep and the max time to simulate
  double dT = 0.1; //Timestep (ns)
  double time = 10; //Time to simulate (ns)
  
  /*** Basic setup is done ***/
  /** This is where the loop for a monte-carlo would start **/
  
  //Set the initial conditions of the particle
  double vel[3] = {0, 0, -v};
  double pos[3] = {0, 0, 0};
  electron.setPosition(pos);
  electron.setVelocity(vel);

  //Get the number of sa,ples from the timestep
  int nSample = (int)time/dT;
  Double_t z[nSample], y[nSample], t[nSample]; 

  // Loop through and populate positions
  // Stops looping if it hits the wall
  int numTSteps = 0;
  for(int i = 0; i < nSample; ++i)
  {
    t[i] = i*dT*pow(10,-9); //Convert from ns to seconds

    //Get the position at this point and record it
    double *pos = electron.getPosition(t[i], field);
    y[i] = pos[1];
    z[i] = -pos[2];

    /*std::cout << "At " << t[i] << " s " << " the particle is at ("
	      << y[i] << ", " << z[i] << ")" << std::endl;
    */

    //clean up the memory used by pos;
    delete [] pos;

    //If we've hit the wall, break from the loop
    if(z[i] < 0 && numTSteps == 0)
      numTSteps = i;
  }// End loop over number of samples

  std::cout << "The particle hit the wall after "
	    << numTSteps * dT << " ns." << std::endl;
  
  //Create the canvas to hold the graph
  TCanvas *c1 = new TCanvas("c1", "Top-down projection", 500, 300);

  //Create the graph and draw it
  TGraph *gr = new TGraph(nSample,y,z);
  gr->SetTitle(Form("En: %f, V: %f, VFoil: %f, B: %f", En, V, VFoil, B));
  gr->Draw("AC*");
  
  app.Run(kTRUE);

  return 0;
}

void usage()
{
  std::cout << "./transport En VMCP B VFoil" << std::endl;
  std::cout << "En is energy of electron in ev" << std::endl;
  std::cout << "VMCP is voltage across the MCP in V" << std::endl;
  std::cout << "B is the magnetic field in gauss" << std::endl;
  std::cout << "VFoil is voltage on the foil in -V" << std::endl;

}
