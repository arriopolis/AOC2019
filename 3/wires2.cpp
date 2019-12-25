#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);

  vector<vector<pair<int,int>>> wires;
  string line;
  while (getline(data_file, line)) {
    vector<pair<int,int>> wire;
    istringstream iline(line);
    int x = 0, y = 0;
    // cout << "New wire:" << endl;
    string tmp;
    while (getline(iline, tmp, ',')) {
      char direction = tmp[0];
      istringstream num(tmp.substr(1));
      int n;
      num >> n;
      int dx,dy;
      if (direction == 'U') dx = 0, dy = 1;
      else if (direction == 'L') dx = -1, dy = 0;
      else if (direction == 'R') dx = 1, dy = 0;
      else if (direction == 'D') dx = 0, dy = -1;
      else {
        cout << "Unknown direction encountered: " << direction << "." << endl;
        return 0;
      }

      // cout << direction << n << endl;

      x += dx * n;
      y += dy * n;
      wire.push_back({x,y});
    }

    cout << "Wire:" << endl;
    for (auto p : wire) {
      cout << p.first << " " << p.second << endl;
    }
    cout << "-----" << endl;

    wires.push_back(wire);
  }

  vector<tuple<int,int,int>> intersections;
  int prevx1 = 0, prevy1 = 0, steps1 = 0;
  for (auto p1 : wires[0]) {
    int newx1 = p1.first, newy1 = p1.second;
    int dx1 = newx1 - prevx1, dy1 = newy1 - prevy1;
    int prevx2 = 0, prevy2 = 0, steps2 = 0;
    for (auto p2 : wires[1]) {
      int newx2 = p2.first, newy2 = p2.second;

      int dx2 = newx2 - prevx2, dy2 = newy2 - prevy2;

      cout << prevx1 << ' ' << prevy1 << ' ' << newx1 << ' ' << newy1 << endl;
      cout << prevx2 << ' ' << prevy2 << ' ' << newx2 << ' ' << newy2 << endl;

      if (dx1 == 0 && dx2 == 0) {
        // Both lines vertical
        if (prevx1 == prevx2)
        {
          for (int y = max(min(prevy1,newy1),min(prevy2,newy2)); y <= min(max(prevy1,newy1),max(prevy2,newy2)); y++) {
            intersections.push_back({steps1+steps2+abs(y-prevy1)+abs(y-prevy2),prevx1,y});
            cout << prevx1 << ' ' << y << endl;
          }
        }
      } else if (dy1 == 0 && dy2 == 0) {
        // Both lines horizontal
        if (prevy1 == prevy2)
        {
          for (int x = max(min(prevx1,newx1),min(prevx2,newx2)); x <= min(max(prevx1,newx1),max(prevx2,newx2)); x++) {
            intersections.push_back({steps1+steps2+abs(x-prevx1)+abs(x-prevx2),x,prevy1});
            cout << x << ' ' << prevy1 << endl;
          }
        }
      } else if (dx1 == 0) {
        int y = prevy2, x = prevx1;
        if (y <= max(prevy1, newy1) && y >= min(prevy1, newy1) && x <= max(prevx2, newx2) && x >= min(prevx2, newx2)) {
          intersections.push_back({steps1+steps2+abs(x-prevx1)+abs(y-prevy1)+abs(x-prevx2)+abs(y-prevy2),x,y});
          cout << x << ' ' << y << endl;
        }
      } else if (dx2 == 0) {
        int y = prevy1, x = prevx2;
        if (y <= max(prevy2, newy2) && y >= min(prevy2, newy2) && x <= max(prevx1, newx1) && x >= min(prevx1, newx1)) {
          intersections.push_back({steps1+steps2+abs(x-prevx1)+abs(y-prevy1)+abs(x-prevx2)+abs(y-prevy2),x,y});
          cout << x << ' ' << y << endl;
        }
      }

      steps2 += abs(dx2) + abs(dy2);
      prevx2 = newx2, prevy2 = newy2;
    }
    steps1 += abs(dx1) + abs(dy1);
    prevx1 = newx1, prevy1 = newy1;
  }

  sort(intersections.begin(), intersections.end());

  cout << "Intersections:" << endl;
  for (auto p : intersections) {
    cout << get<0>(p) << ' ' << get<1>(p) << ' ' << get<2>(p) << endl;
  }
}
