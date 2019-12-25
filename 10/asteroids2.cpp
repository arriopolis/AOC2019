#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <tuple>
using namespace std;

#define PI 3.14159265

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
  map<pair<int,int>,vector<int>> best_dirs;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (asteroids[y][x] == 0) continue;
      map<pair<int,int>,vector<int>> dirs;
      for (int y2 = 0; y2 < h; y2++) {
        for (int x2 = 0; x2 < w; x2++) {
          if (asteroids[y2][x2] == 0) continue;
          if (y2 == y && x2 == x) continue;
          int dy = y2 - y;
          int dx = x2 - x;
          int d = __gcd(abs(dy),abs(dx));
          int ndy = dy / d;
          int ndx = dx / d;
          dirs[{ndy,ndx}].push_back(d);
        }
      }
      if (dirs.size() > most_visible) {
        best_place = {y,x};
        best_dirs = dirs;
        most_visible = dirs.size();
      }
    }
  }

  cout << "Best place: " << best_place.first << ',' << best_place.second << endl;
  // cout << "Asteroids: " << endl;
  // for (pair<pair<int,int>,vector<int>> x : best_dirs) {
  //   pair<int,int> dir = x.first;
  //   vector<int> ds = x.second;
  //   cout << dir.first << ',' << dir.second << ": ";
  //   for (int d : ds) {
  //     cout << d << ' ';
  //   }
  //   cout << endl;
  // }
  // return 0;

  vector<tuple<double,pair<int,int>,vector<int>>> rots;
  for (pair<pair<int,int>,vector<int>> p : best_dirs) {
    pair<int,int> dir = p.first;
    vector<int> ds = p.second;
    sort(ds.begin(), ds.end());
    reverse(ds.begin(), ds.end());
    double angle = atan2(dir.second, dir.first);
    // if (angle >= PI - 1e-8) angle -= 2 * PI;
    // if (angle < 0) angle += 2 * PI;
    rots.push_back({angle, dir, ds});
  }

  sort(rots.begin(), rots.end());
  reverse(rots.begin(), rots.end());

  int ctr = 0;
  int y = best_place.first, x = best_place.second;
  asteroids[y][x] = 2;

  // for (auto l : asteroids) {
  //   for (auto c : l) {
  //     cout << (c == 0 ? '.' : (c == 1 ? '#' : 'o'));
  //   }
  //   cout << endl;
  // }
  // getline(cin,s);

  while (rots.size() > 0) {
    vector<tuple<double,pair<int,int>,vector<int>>> new_rots;
    for (tuple<double,pair<int,int>,vector<int>> x : rots) {
      double angle = get<0>(x);
      pair<int,int> dir = get<1>(x);
      vector<int> ds = get<2>(x);
      int d = ds.back();
      ds.pop_back();
      if (ds.size() > 0) {
        new_rots.push_back({angle,dir,ds});
      }
      ctr++;
      int y2 = best_place.first + d * dir.first, x2 = best_place.second + d * dir.second;
      cout << ctr << ": " << "Shooting down asteroid at offset " << d * dir.first << "," << d * dir.second << ", at location " << y2 << ',' << x2 << endl;
      // asteroids[y2][x2] = 0;
      // for (auto l : asteroids) {
      //   for (auto c : l) {
      //     cout << (c == 0 ? '.' : (c == 1 ? '#' : 'o'));
      //   }
      //   cout << endl;
      // }
      // getline(cin,s);
    }
    rots = new_rots;
  }
}
