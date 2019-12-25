#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  string s;
  vector<vector<int>> asteroids;
  while (getline(data_file, s)) {
    vector<int> line;
    for (char c : s) {
      line.push_back(c == '.' ? 0 : 1);
    }
    asteroids.push_back(line);
  }

  int h = asteroids.size();
  int w = asteroids[0].size();

  int most_visible = 0;
  pair<int,int> best_place;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (asteroids[y][x] == 0) continue;
      set<pair<int,int>> dirs;
      for (int y2 = 0; y2 < h; y2++) {
        for (int x2 = 0; x2 < w; x2++) {
          if (asteroids[y2][x2] == 0) continue;
          if (y2 == y && x2 == x) continue;
          int dy = y2-y;
          int dx = x2-x;
          int d = __gcd(abs(dy),abs(dx));
          dy /= d;
          dx /= d;
          dirs.insert({dy,dx});
        }
      }
      if (dirs.size() > most_visible) {
        best_place = {y,x};
        most_visible = dirs.size();
      }
    }
  }

  cout << best_place.first << ',' << best_place.second << ':' << most_visible << endl;

  // for (auto l : asteroids) {
  //   for (auto c : l) {
  //     cout << (c == 1 ? '#' : '.');
  //   }
  //   cout << endl;
  // }
}
