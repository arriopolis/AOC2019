#include <iostream>
using namespace std;

int main() {
  int min_range = 158126, max_range = 624574;
  int ctr = 0;
  for (int x = min_range; x <= max_range; x++) {
    int prevd = 10, d;
    int y = x;
    bool success = true;
    bool dbl = false;
    for (int i = 0; i < 6; i++) {
      d = y%10;
      y /= 10;
      if (d > prevd) {
        success = false;
        break;
      }
      if (d == prevd) dbl = true;
      prevd = d;
    }
    if (success && dbl) ctr++;
  }
  cout << ctr << endl;
}
