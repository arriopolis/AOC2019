#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

const pair<int,int> offsets[4] = {{-1,0},{0,-1},{0,1},{1,0}};

int get_hash(char grid[5][5]) {
  int res = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (grid[i][j] == '#') {
        res += (1 << (5 * i + j));
      }
    }
  }
  return res;
}

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  string s;
  char grid[5][5];
  int i = 0;
  while (getline(data_file, s)) {
    int j = 0;
    for (char c : s) {
      grid[i][j++] = c;
    }
    i++;
  }

  cout << "Initial grid:" << endl;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      cout << grid[i][j];
    }
    cout << endl;
  }

  set<int> hashes;
  hashes.insert(get_hash(grid));
  // int iteration = 0;
  while (true) {
    // if (iteration % 1000 == 0) cout << "Iteration: " << iteration++ << endl;
    char new_grid[5][5];
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        int num_adjacent = 0;
        for (pair<int,int> o : offsets) {
          int dy = o.first, dx = o.second;
          int newy = y+dy, newx = x+dx;
          // cout << y << ',' << x << " --> " << newy << ',' << newx << endl;
          if (newy < 0 || newy >= 5 || newx < 0 || newx >= 5) continue;
          if (grid[newy][newx] == '#') num_adjacent++;
        }
        // cout << y << ',' << x << ',' << num_adjacent << endl;
        if (grid[y][x] == '#') {
          new_grid[y][x] = (num_adjacent == 1 ? '#' : '.');
        } else {
          new_grid[y][x] = ((num_adjacent == 1 || num_adjacent == 2) ? '#' : '.');
        }
      }
    }
    cout << "New grid:" << endl;
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        grid[y][x] = new_grid[y][x];
        cout << grid[y][x];
      }
      cout << endl;
    }
    int hash = get_hash(grid);
    if (hashes.find(hash) != hashes.end()) {
      cout << hash << endl;
      return 0;
    }
    hashes.insert(hash);
  }
}
