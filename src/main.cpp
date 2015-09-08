#include <iostream>
#include <iomanip>
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <complex>
#include <plotter.h>

using namespace std;
using namespace NTL;

const int maxorder = 12;

void draw_c_curve(Plotter &plotter, double dx, double dy, int order) {
  if (order >= maxorder)
    plotter.fcontrel(dx, dy); // continue path along (dx, dy)
  else {
    draw_c_curve(plotter, 0.5 * (dx - dy), 0.5 * (dx + dy), order + 1);
    draw_c_curve(plotter, 0.5 * (dx + dy), 0.5 * (dy - dx), order + 1);
  }
}

// Check Period of 2^r == 1 (mod p), (smallest r > 0)
// Input p
// Output r
ZZ FindPeriod(const ZZ& prime, const ZZ& base = ZZ(2))
{
    ZZ r(1);
    ZZ rhs = MulMod(1, base, prime);
   
    while(rhs != 1) {
       rhs = MulMod(rhs, base, prime);
       r++;
       if(rhs == 0) {
           return ZZ(0);
       }
       //cout << rhs << endl;
    }
    return r;
}

int main() {

    ZZ p(2);
    for(ZZ i = ZZ(2); i <= 1000; i++) {
        p *= 2;
        ZZ b(3);
        ZZ prime = p-b;
        if(prime > 0 && ProbPrime(prime)) {
            cout << "Prime " << prime << "\t";
            cout << "i " << i << "\t";
            cout << "Period2 " << FindPeriod(prime) << "\t";
            cout << "Period" << b << " " << FindPeriod(prime, b) << endl;
        }
    }
    return 0;
}

int main2() {

  RR::SetPrecision(1024);

  ZZ prime = GenPrime_ZZ(1024);
  if (!ProbPrime(prime)) {
    return 1;
  }

  ZZ base = RandomBits_ZZ(512);
  ZZ pw = RandomBits_ZZ(512);
  ZZ res = PowerMod(base, pw, prime);

  cout << "Prime" << endl;
  cout << prime << endl;

  cout << "Base" << endl;
  cout << base << endl;

  cout << "Power" << endl;
  cout << pw << endl;

  cout << "Result" << endl;
  cout << res << endl;

  ///////////////////

  RR pi(M_PI);

  // std::cout << "exp(i, pi) = " << omega << '\n';
  // cout << omega << endl;
  /*
  PlotterParams params;
  params.setplparam("PAGESIZE", (char *)"letter");

  XPlotter plotter(cin, cout, cerr, params); // declare Plotter
  if (plotter.openpl() < 0)                   // open Plotter
  {
    cerr << "Couldn't open Plotter\n";
    return 1;
  }

  plotter.fspace(0.0, 0.0, 1000.0, 1000.0); // specify user coor system
  plotter.flinewidth(0.25);    // line thickness in user coordinates
  plotter.pencolorname("red"); // path will be drawn in red
  plotter.erase();             // erase Plotter's graphics display
  plotter.fmove(600.0, 300.0); // position the graphics cursor
  draw_c_curve(plotter, 0.0, 400.0, 0);
  if (plotter.closepl() < 0) // close Plotter
  {
    cerr << "Couldn't close Plotter\n";
    return 1;
  }
  */

  return 0;
}
