#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);

  int w = 25;
  int h = 6;
  int image[w][h];
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      image[x][y] = 2;
    }
  }

  int y = 0, x = 0;
  char c;
  while (data_file >> c) {
    if (c == '0' && image[x][y] == 2) image[x][y] = 0;
    if (c == '1' && image[x][y] == 2) image[x][y] = 1;
    x++;
    if (x == w) {
      x = 0;
      y++;
      if (y == h) {
        y = 0;
      }
    }
  }

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      cout << (image[x][y] == 1 ? 'X' : '.');
    }
    cout << endl;
  }
  return 0;
}
