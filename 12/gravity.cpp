#include <iostream>
using namespace std;

int main() {
  // Test:
  // int xs[4] = {-1,2,4,3};
  // int ys[4] = {0,-10,-8,5};
  // int zs[4] = {2,-7,8,-1};

  int xs[4] = {-9,-14,1,-19};
  int ys[4] = {10,-8,5,7};
  int zs[4] = {-1,14,6,8};
  int vxs[4] = {0};
  int vys[4] = {0};
  int vzs[4] = {0};

  for (int t = 0; t < 1000; t++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < i; j++) {
        if (xs[i] < xs[j]) {
          vxs[i]++;
          vxs[j]--;
        } else if (xs[i] > xs[j]) {
          vxs[i]--;
          vxs[j]++;
        }

        if (ys[i] < ys[j]) {
          vys[i]++;
          vys[j]--;
        } else if (ys[i] > ys[j]) {
          vys[i]--;
          vys[j]++;
        }

        if (zs[i] < zs[j]) {
          vzs[i]++;
          vzs[j]--;
        } else if (zs[i] > zs[j]) {
          vzs[i]--;
          vzs[j]++;
        }
      }
    }

    for (int i = 0; i < 4; i++) {
      xs[i] += vxs[i];
      ys[i] += vys[i];
      zs[i] += vzs[i];
    }
  }

  int total_energy = 0;
  for (int i = 0; i < 4; i++) {
    int energy = (abs(xs[i]) + abs(ys[i]) + abs(zs[i])) * (abs(vxs[i]) + abs(vys[i]) + abs(vzs[i]));
    total_energy += energy;
  }

  cout << total_energy << endl;
}
