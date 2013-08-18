#include <iostream>
#include <cmath>

using namespace std;


float density(float fTemp, float fPress, int fFluidCheck);
float dViscosity(float fTemp, float fPress, int fFluidCheck);
float cFriction(float fRe, int fFlowCheck);
float calcSurfCellSize(float fyHeight, float fAspectRatio, int fCellType);

int main ()
{
	float xVel, lScale, temp, press, rho, mu, Re, tauStress, tauVel, yPlus, yHeight, Cf, surfCellSize, aspectRatio;
	int fluidCheck, flowCheck, cellType;
	
	cout << endl;
	cout << "Choose fluid (air=1 water=2): "; cin >> fluidCheck;
	cout << endl;

	switch(fluidCheck)
	case 1:
	{
	cout << "Define operating temperature [C]: "; cin >> temp;
	cout << "Define operating relative pressure [Pa]: "; cin >> press;
	cout << endl;
	break;
	
	case 2:
	
	cout << "Define operating temperature <5,95> [C]: "; cin >> temp;
	cout << "Water considered incompressible." << endl; press = 1.0;
	cout << endl;
	break;
	
	default:
	return -1;
	}

	// Define material properties based on operating conditions
	rho = density(temp, press, fluidCheck);
	mu  = dViscosity(temp, press, fluidCheck);

	cout << "Density [kg/m^3]: " << rho << endl;
	cout << "Dynamic viscosity [Pa.s]: " << mu << endl;
	cout << endl;
	cout << "Free stream velocity [m/s]: "; cin >> xVel;
	cout << "Lenght scale [m]: "; cin >> lScale;

	// Reynolds number evaluation
	Re = rho * xVel * lScale / mu;
	cout << "Reynold's number: " << Re << endl;
	cout << endl;
	
	// Skin friction coefficient estimate
	cout << "Choose flow type (internal=1 external=2): "; cin >> flowCheck;
	cout << endl;

	Cf = cFriction(Re, flowCheck);

	// Wall shear stress
	tauStress = 0.5 * Cf * rho * pow(xVel,2.0);

	// Frictional velocity
	tauVel = sqrt(tauStress/rho);

	cout << "Target yPlus value: "; cin >> yPlus;

	// Height of the first cell [mm]
	yHeight = 1000.0 * yPlus * mu / rho / tauVel;
	
	cout << "Estimated first cell height [mm]: " << yHeight << endl;
	cout << endl;
	
	// Surface cell size estimate based on aspect ratio
	cout << "Choose cell type (tri=1 quad=2): "; cin >> cellType;
	cout << endl;

	cout << "Target aspect ratio: "; cin >> aspectRatio;

	surfCellSize = calcSurfCellSize(yHeight, aspectRatio, cellType);

	cout << "Estimated surface cell size [mm]: " << surfCellSize << endl;
	cout << endl;

	return 0;
}

// Formulas from http://www-mdp.eng.cam.ac.uk/web/library/enginfo/aerothermal_dvd_only/aero/fprops/propsoffluids/node5.html

// Density calculator
float density(float fTemp, float fPress, int fFluidCheck)
{
	float fRho;
	
	switch(fFluidCheck)
	{
	//Air, Ideal Gas Equation
	case 1:
		fRho = (101325.0 + fPress) / 287.05 / (273.15 + fTemp);
		break;
	
	//Water
	case 2:
		fRho = 1001.1 - 0.0867 * fTemp - 0.0035 * pow(fTemp, 2.0); // Temp in Celsius
		break;

	default:
		return -1;
	}

	return fRho;
}

// Dynamic viscosity calculator
float dViscosity(float fTemp, float fPress, int fFluidCheck)
{
	float fMu;

	switch(fFluidCheck)
	{
	//Air, Sutherland Equation
	case 1:
		fMu = 1.458e-6 * pow(273.15 + fTemp, 1.5) / (273.15 + fTemp + 110.4);
		break;
	
	//Water
	case 2:	
		fMu = 0.001792 * exp(6.74 * pow(273.16 / (273.15 + fTemp), 2.0) - 4.8 * (273.16 / (273.15 + fTemp)) - 1.94);
		break;

	default:
		return -1;
	}

	return fMu;
}

// Skin friction coefficient calculator
float cFriction(float fRe, int fFlowCheck)
{
	float fCf;	

	switch(fFlowCheck)
	{
	// Internal
	case 1:
		fCf = 0.079 * pow(fRe, -0.25);
		break;
	
	//External
	case 2:
		fCf = 0.058 * pow(fRe, -0.2);
		break;

	default:
		return -1;
	}

	return fCf;
}

// Surface cell size calculator
float calcSurfCellSize(float fyHeight, float fAspectRatio, int fCellType)
{
	float fSurfCellSize, triCellSide;
	
	switch(fCellType)
	{
	// Triangular
	case 1:
		triCellSide = fyHeight * sqrt((pow(fAspectRatio, 2.0) - 1.0) / 0.75);
		fSurfCellSize = 2.0 * triCellSide * 0.866;
		break;	

	// Quadratic
	case 2:
		fSurfCellSize = fyHeight * sqrt((pow(fAspectRatio, 2.0) - 1.0) / 2.0);
		break;

	default:
		return -1;
	}

	return fSurfCellSize;
}




