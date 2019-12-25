#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
using namespace std;

void DFS(map<char,vector<tuple<int,char,set<char>>>>& all_moves, char cur_pos, int cur_score, set<char> visited, vector<char> cur_route, int& best_score) {
  vector<tuple<int,char,set<char>>> moves = all_moves[cur_pos];
  for (auto p : moves) {
    int dist = get<0>(p);
    if (cur_score + dist >= best_score) break;
    int d = get<1>(p);
    if (visited.find(d) != visited.end()) continue;
    set<char> doors = get<2>(p);
    bool success = true;
    for (char door : doors) {
      if (visited.find(tolower(door)) == visited.end()) {
        success = false;
        break;
      }
    }
    if (!success) continue;

    int new_score = cur_score + dist;
    set<char> new_visited = visited;
    new_visited.insert(d);
    vector<char> new_route = cur_route;
    new_route.push_back(d);

    if (new_visited.size() == all_moves.size()) {
      cout << "Found route of score: " << new_score << endl;
      cout << "Route: ";
      for (char x : new_route) {
        cout << x << ' ';
      }
      cout << endl;
      best_score = min(best_score, new_score);
    } else {
      DFS(all_moves, d, new_score, new_visited, new_route, best_score);
    }
  }
}

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  vector<vector<char>> area;

  ifstream data_file(argv[1]);
  string s;
  while (data_file >> s) {
    stringstream ss;
    ss << s;
    vector<char> row;
    for (char c : s) {
      row.push_back(c);
    }
    area.push_back(row);
  }

  int h = area.size();
  int w = area[0].size();

  map<char,pair<int,int>> keys;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (('a' <= area[y][x] && area[y][x] <= 'z') || area[y][x] == '@') {
        keys[area[y][x]] = {y,x};
      }
    }
  }

  cout << "The following keys are present:" << endl;
  for (auto p : keys) {
    char c = p.first;
    pair<int,int> q = p.second;
    cout << c << " : (" << q.first << ',' << q.second << ')' << endl;
  }

  map<char,vector<tuple<int,char,set<char>>>> all_moves;

  for (auto kp : keys) {
    char c = kp.first;
    int ystart = kp.second.first, xstart = kp.second.second;
    cout << "Looking from key " << c << endl;
    vector<tuple<int,char,set<char>>> moves;

    set<pair<pair<int,int>,set<char>>> frontier;
    map<pair<int,int>,int> visited;
    frontier.insert({{ystart,xstart},{}});
    visited[{ystart,xstart}] = 0;
    pair<int,int> offsets[4] = {{-1,0},{0,1},{1,0},{0,-1}};
    int num_steps = 0;
    while (!frontier.empty()) {
      num_steps++;
      set<pair<pair<int,int>,set<char>>> new_frontier;
      for (auto f : frontier) {
        auto p = f.first;
        int y = p.first, x = p.second;
        set<char> doors = f.second;
        for (auto q : offsets) {
          int dy = q.first, dx = q.second;
          int newy = y+dy, newx = x+dx;
          if (visited.find({newy,newx}) != visited.end()) continue;
          if (area[newy][newx] == '#') continue;
          if ('A' <= area[newy][newx] && area[newy][newx] <= 'Z') {
            doors.insert(area[newy][newx]);
          }
          if ('a' <= area[newy][newx] && area[newy][newx] <= 'z') {
            cout << "Found key " << area[newy][newx] << " after " << num_steps << " steps. Doors: ";
            for (char c : doors) {
              cout << c << ' ';
            }
            cout << endl;
            moves.push_back({num_steps, area[newy][newx], doors});
          }
          new_frontier.insert({{newy,newx}, doors});
          visited[{newy,newx}] = num_steps;
        }
      }
      frontier = new_frontier;
    }

    all_moves[c] = moves;
  }

  for (auto a : all_moves) {
    char c = a.first;
    auto moves = a.second;
    for (auto p : moves) {
      int dist = get<0>(p);
      char d = get<1>(p);
      set<char> doors = get<2>(p);
      cout << c << " --> " << d << ": " << dist << ", doors: ";
      for (auto door : doors) {
        cout << door << ", ";
      }
      cout << endl;
    }
  }

  int best_score = 100000000;
  set<char> visited;
  visited.insert('@');
  vector<char> cur_route = {'@'};
  DFS(all_moves, '@', 0, visited, cur_route, best_score);
  cout << best_score << endl;
}
