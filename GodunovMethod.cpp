#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIV 1048576 
#define WIDTH 7
#endif

#ifdef linux
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

////////////////////////// prototype of functions /////////////////////////////////

double flux_e(double ,double );

double flux_w(double ,double );

double Flux(double );

double dFlux_du(double );

/////////////////////////////////////////////////////////////////////////////////////////

int main()
{

    /***************************** input data section *************************************/

	int i, n, count;
	n = 1000;    // number of nodes
	double u0[n], u[n], L, dx, t, dt, t_end;
	L = 1.0;
	t = 0.0;
	dt = 0.001;
	t_end = 2.0;

	//mesh generation:

	dx=L/(n-1);

	//initioal condition:

	for(i=0;i<n;i++)
	{
		if(i<=n/2)
			u[i]= 0.7;
		else
			u[i]= 0.3;
	}

	/*******************************************************************************************/

    cout << "input data of the problem:" << endl << endl;
    cout << "length of computational domain: " << L << endl;
    cout << "initial time (t0) = " << t << endl;
    cout << "delta_t: " << dt << endl;
    cout << "t_end: " << t_end << endl;
    cout << "number of nodes:: " << n << endl << endl;
  
	// creating folder name "result" to write the output

	#ifdef _WIN32

	if (mkdir("results") != -1)
        cout << "Directory of results was created." << endl;
        
	#endif
	
	#ifdef linux
	
	if (opendir("results"))
		system("rm -r results");

	if (mkdir("results", 0777) != -1)
        cout << "Directory of results was created." << endl;
        
	#endif

	//solution algorithm:

	count=0;

	do
	{
		if( (count%100)==0 )
		{
			stringstream ss;
			ss << t;
			string str = ss.str();
			string myName = "results/t=" + str + ".txt";
			const char* fileName = myName.c_str();	;

			ofstream file;
			file.open(fileName);

			file<<"x" << '\t' << '\t' << "u" <<endl<<endl;

			for(i=0;i<n;i++)
				file << std::fixed << std::setprecision(6) <<i*dx <<'\t'<<u[i]<<endl;

			file.close();
		}

		t = t + dt;
		count++;

		for (i=0;i<n;i++)
			u0[i] = u[i];

		for (i=1;i<n-1;i++)
			u[i] = u0[i] - dt/dx * (flux_e(u0[i+1],u0[i]) - flux_w(u0[i],u0[i-1]));

		u[0] = u[1];
		u[n-1] = u[n-2];

	}
	while(t<=t_end);

	cout << "result data was written." << endl << endl;
	cout << "end program." << endl;

}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//claculation of flux_e
double flux_e(double u0_i_plus_1,double u0_i)
{
	double U_l, U_r;
	U_l = dFlux_du(u0_i);
	U_r = dFlux_du(u0_i_plus_1);

	if ( (U_l*U_r) > 0 && U_l > 0)
		return Flux(u0_i);

	else if ( (U_l*U_r) > 0 && U_l < 0)
		return Flux(u0_i_plus_1);

	else if ( (U_l*U_r) < 0 && U_l < 0)
		return 0.0;

	else if ( (U_l*U_r) < 0 && U_l > 0)
	{
		if ( (U_l + U_r)/2.0 > 0 )
			return Flux(u0_i);
		else
			return Flux(u0_i_plus_1);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////

//claculation of flux_w
double flux_w(double u0_i,double u0_i_minus_1)
{
	double U_l, U_r;
	U_l = dFlux_du(u0_i_minus_1);
	U_r = dFlux_du(u0_i);

	if ( (U_l*U_r) > 0 && U_l > 0)
		return Flux(u0_i_minus_1);

	else if ( (U_l*U_r) > 0 && U_l < 0)
		return Flux(u0_i);

	else if ( (U_l*U_r) < 0 && U_l < 0)
		return 0.0;

	else if ( (U_l*U_r) < 0 && U_l > 0)
	{
		if ( (U_l + U_r)/2.0 > 0 )
			return Flux(u0_i_minus_1);
		else
			return Flux(u0_i);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////

//claculation of Flux F(u)
double Flux(double u_)
{
	return 1.0/2 *u_*u_;
}

/////////////////////////////////////////////////////////////////////////////////////////

//claculation of (dF/du)
double dFlux_du(double u_)
{
	return u_;
}
