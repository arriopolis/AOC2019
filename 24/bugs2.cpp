#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

const pair<int,int> offsets[4] = {{-1,0},{0,-1},{0,1},{1,0}};
const int num_iterations = 200;

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

  // set up the grid
  char grid[401][5][5];
  for (int l = 0; l < 401; l++) {
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        grid[l][y][x] = '.';
      }
    }
  }

  // fill the middle level
  ifstream data_file(argv[1]);
  string s;
  int i = 0;
  while (getline(data_file, s)) {
    int j = 0;
    for (char c : s) {
      grid[200][i][j++] = c;
    }
    i++;
  }

  // display the grid
  cout << "Initial grid:" << endl;
  for (int l = 200 - num_iterations; l < 200 + num_iterations + 1; l++) {
    cout << "Level: " << l << endl;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        cout << grid[l][i][j];
      }
      cout << endl;
    }
  }

  // evolve
  for (int i = 0; i < num_iterations; i++) {

    // create new grid
    char new_grid[401][5][5];
    for (int l = 0; l < 401; l++) {
      for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
          if (y == 2 && x == 2) continue;
          int num_adjacent = 0;
          for (pair<int,int> o : offsets) {
            int dy = o.first, dx = o.second;
            int newy = y+dy, newx = x+dx;
            if (newy < 0 || newy >= 5 || newx < 0 || newx >= 5) {
              // Go one level up
              int newl = l-1;
              if (newl < 0) continue;
              if (newy < 0) {
                if (grid[newl][1][2] == '#') num_adjacent++;
              } else if (newx < 0) {
                if (grid[newl][2][1] == '#') num_adjacent++;
              } else if (newy >= 5) {
                if (grid[newl][3][2] == '#') num_adjacent++;
              } else {
                if (grid[newl][2][3] == '#') num_adjacent++;
              }
            } else if (newy == 2 && newx == 2) {
              // Go one level down
              int newl = l+1;
              if (newl >= 401) continue;
              if (dy < 0) {
                for (int newx = 0; newx < 5; newx++) {
                  if (grid[newl][4][newx] == '#') num_adjacent++;
                }
              } else if (dx < 0) {
                for (int newy = 0; newy < 5; newy++) {
                  if (grid[newl][newy][4] == '#') num_adjacent++;
                }
              } else if (dy > 0) {
                for (int newx = 0; newx < 5; newx++) {
                  if (grid[newl][0][newx] == '#') num_adjacent++;
                }
              } else {
                for (int newy = 0; newy < 5; newy++) {
                  if (grid[newl][newy][0] == '#') num_adjacent++;
                }
              }
            } else {
              // Stay on the same level
              if (grid[l][newy][newx] == '#') num_adjacent++;
            }
          }
          if (grid[l][y][x] == '#') {
            new_grid[l][y][x] = (num_adjacent == 1 ? '#' : '.');
          } else {
            new_grid[l][y][x] = ((num_adjacent == 1 || num_adjacent == 2) ? '#' : '.');
          }
        }
      }
    }

    // update the grid
    for (int l = 0; l < 401; l++) {
      for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
          grid[l][y][x] = new_grid[l][y][x];
        }
      }
    }

    // display the grid
    cout << "New grid:" << endl;
    for (int l = 200 - num_iterations; l < 200 + num_iterations + 1; l++) {
      cout << "Level: " << l << endl;
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          cout << grid[l][i][j];
        }
        cout << endl;
      }
    }
  }

  int num_bugs = 0;
  for (int l = 0; l < 401; l++) {
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        if (grid[l][y][x] == '#') num_bugs++;
      }
    }
  }

  cout << num_bugs << endl;
}
