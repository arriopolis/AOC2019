#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please provide the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);

  map<string,vector<string>> orbits;
  map<string,string> circles;

  string s;
  while (getline(data_file,s)) {
    string c,o;
    stringstream ss;
    ss << s;
    getline(ss,c,')');
    getline(ss,o);
    orbits[c].push_back(o);
    circles[o] = c;
  }

  map<string,int> dists;
  priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> q;
  q.push({0,"YOU"});
  while (!q.empty()) {
    auto p = q.top();
    q.pop();

    int cur_dist = p.first;
    string c = p.second;
    cout << cur_dist << ' ' << c << endl;

    if (!(dists.find(c) == dists.end())) continue;
    dists[c] = cur_dist;

    // Propagate back
    if (c.compare("COM")) {
      string pc = circles[c];
      if (dists.find(pc) == dists.end()) {
        q.push({cur_dist+1,pc});
      }
    }

    // Propagate forward
    for (string newo : orbits[c]) {
      if (dists.find(newo) == dists.end()) {
        q.push({cur_dist+1,newo});
      }
    }
  }

  cout << dists["SAN"] - 2 << endl;
}
