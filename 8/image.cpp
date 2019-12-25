#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

int w = 25;
int h = 6;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  int fewest_zeros = numeric_limits<int>::max();
  int best_value;
  int y = 0, x = 0;
  int num_zeros = 0, num_ones = 0, num_twos = 0;
  char c;
  while (data_file >> c) {
    if (c == '0') num_zeros++;
    if (c == '1') num_ones++;
    if (c == '2') num_twos++;
    x++;
    if (x == w) {
      x = 0;
      y++;
      if (y == h) {
        cout << "Layer finished." << endl;
        if (num_zeros < fewest_zeros) {
          fewest_zeros = num_zeros;
          best_value = num_ones * num_twos;
        }
        y = 0;
        num_zeros = 0;
        num_ones = 0;
        num_twos = 0;
      }
    }
  }

  cout << best_value << endl;
  return 0;
}
