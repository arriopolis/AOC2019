#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the name of the data file as a command line argument." << endl;
    return 0;
  }

  // Read the maze
  ifstream data_file(argv[1]);
  vector<vector<char>> maze;
  string s;
  while (getline(data_file,s)) {
    vector<char> row;
    for (auto c : s) {
      row.push_back(c);
    }
    maze.push_back(row);
  }

  // Display the maze
  for (auto r : maze) {
    for (auto c : r) {
      cout << c;
    }
    cout << endl;
  }

  // Find the portals
  pair<int,int> offsets[4] = {{-1,0},{0,1},{1,0},{0,-1}};
  map<pair<char,char>,vector<pair<int,int>>> portals;
  for (int y = 0; y < maze.size(); y++) {
    for (int x = 0; x < maze[y].size(); x++) {
      if (maze[y][x] != '.') continue;
      for (pair<int,int> o : offsets) {
        int dy = o.first, dx = o.second;
        int newy = y+dy, newx = x+dx;
        if (newy < 0 || newy >= maze.size() || newx < 0 || newx >= maze[newy].size()) continue;
        if ('A' <= maze[newy][newx] && maze[newy][newx] <= 'Z') {
          char c1,c2;
          if (dy + dx < 0) {
            c1 = maze[y+2*dy][x+2*dx];
            c2 = maze[y+dy][x+dx];
          } else {
            c1 = maze[y+dy][x+dx];
            c2 = maze[y+2*dy][x+2*dx];
          }
          portals[{c1,c2}].push_back({y,x});
        }
      }
    }
  }

  // Display the portals
  for (auto p : portals) {
    pair<char,char> cs = p.first;
    vector<pair<int,int>> s = p.second;
    cout << "Portal " << cs.first << cs.second << ": ";
    for (auto sq : s) {
      cout << '(' << sq.first << ',' << sq.second << ") ";
    }
    cout << endl;
  }

  // Store the links
  map<pair<int,int>,pair<int,int>> links;
  pair<int,int> start;
  pair<int,int> end;
  for (auto p : portals) {
    pair<char,char> cs = p.first;
    vector<pair<int,int>> s = p.second;
    if (cs.first == 'A' && cs.second == 'A') {
      start = s.back();
    } else if (cs.first == 'Z' && cs.second == 'Z') {
      end = s.back();
    } else {
      pair<int,int> sq1 = s.back();
      s.pop_back();
      pair<int,int> sq2 = s.back();
      links[sq1] = sq2;
      links[sq2] = sq1;
    }
  }

  // Display the links
  cout << "Possible links:" << endl;
  for (auto p : links) {
    pair<int,int> sq1 = p.first;
    pair<int,int> sq2 = p.second;
    cout << '(' << sq1.first << ',' << sq1.second << ") --> (" << sq2.first << ',' << sq2.second << ')' << endl;
  }
  cout << "Start: (" << start.first << ',' << start.second << ')' << endl;
  cout << "End: (" << end.first << ',' << end.second << ')' << endl;

  // Run Dijkstra with BFS
  map<pair<int,int>,int> visited;
  visited[start] = 0;
  set<pair<int,int>> frontier = {start};
  int ctr = 0;
  while (!frontier.empty()) {
    ctr++;
    set<pair<int,int>> new_frontier;
    for (pair<int,int> sq : frontier) {
      int y = sq.first, x = sq.second;
      for (pair<int,int> o : offsets) {
        int dy = o.first, dx = o.second;
        int newy = y+dy, newx = x+dx;
        if (newy < 0 || newy >= maze.size() || newx < 0 || newx >= maze[newy].size()) continue;
        if (maze[newy][newx] != '.') continue;
        if (visited.find({newy,newx}) != visited.end()) continue;
        visited[{newy,newx}] = ctr;
        new_frontier.insert({newy,newx});
      }
      if (links.find(sq) != links.end()) {
        pair<int,int> newsq = links[sq];
        if (visited.find(newsq) == visited.end()) {
          visited[newsq] = ctr;
          new_frontier.insert(newsq);
        }
      }
    }
    frontier = new_frontier;
  }

  // Display the distances
  // for (auto p : visited) {
  //   pair<int,int> sq = p.first;
  //   int dist = p.second;
  //   cout << '(' << sq.first << ',' << sq.second << "): " << dist << endl;
  // }

  cout << "Distance: " << visited[end] << endl;
}
