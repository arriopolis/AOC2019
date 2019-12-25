#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please provide the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);

  map<string,vector<string>> orbits;

  string s;
  while (getline(data_file,s)) {
    string c,o;
    stringstream ss;
    ss << s;
    getline(ss,c,')');
    getline(ss,o);
    orbits[c].push_back(o);
  }

  int total = 0;
  vector<pair<string,int>> q = {{"COM",0}};
  while (q.size()) {
    auto p = q.back();
    q.pop_back();
    string c = p.first;
    int dist = p.second;
    for (string o : orbits[c]) {
      q.push_back({o,dist+1});
      total += dist + 1;
    }
  }

  cout << total << endl;
}
