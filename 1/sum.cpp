#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc < 2) {
    cout << "Please supply the input file as a command line argument." << endl;
    return 0;
  }
  ifstream infile(argv[1]);
  int m, t = 0;
  while (infile >> m) {
    t += (m / 3) - 2;
  }
  cout << t << endl;
}
