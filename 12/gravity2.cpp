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

  int start_xs[4];
  int start_ys[4];
  int start_zs[4];
  int start_vxs[4];
  int start_vys[4];
  int start_vzs[4];
  for (int i = 0; i < 4; i++) {
    start_xs[i] = xs[i];
    start_ys[i] = ys[i];
    start_zs[i] = zs[i];
    start_vxs[i] = vxs[i];
    start_vys[i] = vys[i];
    start_vzs[i] = vzs[i];
  }

  long long t = 0;
  while (true) {
    if (t % 100000 == 0) cout << t << '\r';
    if (t > 0) {
      bool success = true;
      for (int i = 0; i < 4; i++) {
        if (xs[i] != start_xs[i]) {success = false; break;}
        if (ys[i] != start_ys[i]) {success = false; break;}
        if (zs[i] != start_zs[i]) {success = false; break;}
        if (vxs[i] != start_vxs[i]) {success = false; break;}
        if (vys[i] != start_vys[i]) {success = false; break;}
        if (vzs[i] != start_vzs[i]) {success = false; break;}
      }
      if (success) {
        cout << t << endl;
        return 0;
      }
    }

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

    t++;
  }
}
