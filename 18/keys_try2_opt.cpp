#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

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
          set<char> new_doors = doors;
          int dy = q.first, dx = q.second;
          int newy = y+dy, newx = x+dx;
          if (visited.find({newy,newx}) != visited.end()) continue;
          if (area[newy][newx] == '#') continue;
          if ('A' <= area[newy][newx] && area[newy][newx] <= 'Z') {
            new_doors.insert(area[newy][newx]);
          }
          if ('a' <= area[newy][newx] && area[newy][newx] <= 'z') {
            // cout << "Found key " << area[newy][newx] << " after " << num_steps << " steps. Doors: ";
            // for (char c : doors) {
            //   cout << c << ' ';
            // }
            // cout << endl;
            moves.push_back({num_steps, area[newy][newx], new_doors});
          }
          new_frontier.insert({{newy,newx}, new_doors});
          visited[{newy,newx}] = num_steps;
        }
      }
      frontier = new_frontier;
    }

    all_moves[c] = moves;
  }

  // Display all moves
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

  map<char,set<char>> required_keys;
  for (auto p : all_moves['@']) {
    char c = get<1>(p);
    set<char> doors = get<2>(p);
    for (char d : doors) {
      required_keys[c].insert(tolower(d));
    }
  }

  bool change = true;
  while (change) {
    change = false;
    for (auto p : required_keys) {
      char c = p.first;
      set<char> req = p.second;
      for (char q : req) {
        set<char> req2 = required_keys[q];
        for (char r : req2) {
          if (req.find(r) == req.end()) {
            required_keys[c].insert(r);
            change = true;
          }
        }
      }
    }
  }

  cout << "Required keys:" << endl;
  for (auto p : required_keys) {
    char c = p.first;
    set<char> req = p.second;
    cout << c << ": ";
    for (char d : req) {
      cout << d << ' ';
    }
    cout << endl;
  }

  int num_keys = all_moves.size() - 1;
  cout << all_moves.size() << endl;

  int best_score;
  int cur_inv_score = 0;

  map<pair<set<char>,char>,int> costs;
  priority_queue<tuple<int,set<char>,char>, vector<tuple<int,set<char>,char>>, greater<tuple<int,set<char>,char>>> pq;
  pq.push({0,{},'@'});
  while (!pq.empty()) {
    tuple<int,set<char>,char> p = pq.top();
    pq.pop();
    int cur_score = get<0>(p);
    set<char> s = get<1>(p);
    char c = get<2>(p);

    if (cur_score > cur_inv_score) {
      cur_inv_score = cur_score;
      cout << "Current score under investigation: " << cur_inv_score << endl;
    }

    if (s.size() == num_keys) {
      best_score = cur_score;
      cout << cur_score << endl;
      break;
    }

    if (costs.find({s,c}) != costs.end()) continue;
    costs[{s,c}] = cur_score;

    // for (auto x : s) {
    //   cout << x;
    // }
    // cout << ',' << c << ',' << cur_score << endl;

    // string tmp;
    // getline(cin,tmp);

    for (auto a : all_moves[c]) {
      int dist = get<0>(a);
      char d = get<1>(a);
      set<char> doors = get<2>(a);
      bool success = true;

      // Check if you can move there
      for (auto x : doors) {
        if (s.find(tolower(x)) == s.end()) {
          success = false;
          break;
        }
      }
      if (!success) continue;

      // Check if you needed d to get somewhere already
      for (char x : s) {
        if (required_keys[x].find(d) != required_keys[x].end()) {
          success = false;
          break;
        }
      }
      if (!success) continue;

      set<char> news = s;
      news.insert(d);
      if (costs.find({news,d}) != costs.end() && costs[{news,d}] <= cur_score + dist) continue;
      pq.push({cur_score + dist, news, d});
    }
  }

  // for (auto p : costs) {
  //   set<char> s = p.first.first;
  //   char c = p.first.second;
  //   int cost = p.second;
  //   for (char d : s) cout << d;
  //   cout << ',' << c << ':' <<  cost << endl;
  // }

  cout << "Best score is: " << best_score << endl;
}
