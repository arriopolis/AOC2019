#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  string s;
  map<string,pair<int,map<string,int>>> reactions;
  while (getline(data_file,s)) {
    map<string,int> reaction;
    stringstream ss;
    ss << s;
    bool last = false;
    while (!last) {
      int amount;
      string material;
      ss >> amount;
      ss >> material;
      if (material.back() == ',') {
        material.pop_back();
      } else {
        last = true;
      }
      reaction[material] = amount;
    }

    string dummy;
    ss >> dummy;  // the => part

    int amount;
    string material;
    ss >> amount;
    ss >> material;
    reactions[material] = {amount, reaction};
  }

  // for (auto p : reactions) {
  //   string material = p.first;
  //   int amount = p.second.first;
  //   map<string,int> reaction = p.second.second;
  //   cout << material << ": " << amount << " with ";
  //   for (auto q : reaction) {
  //     string mat = q.first;
  //     int amo = q.second;
  //     cout << amo << " " << mat << " ";
  //   }
  //   cout << endl;
  // }

  vector<string> order;
  set<string> todo;
  for (auto p : reactions) {
    if (p.first != "ORE") {
      todo.insert(p.first);
    }
  }

  while (!todo.empty()) {

    // cout << "-----------" << endl;
    // for (string s : order) {
    //   cout << s << ", ";
    // }
    // cout << endl;
    // for (string s : todo) {
    //   cout << s << endl;
    // }

    for (auto p : reactions) {
      string mat = p.first;
      if (todo.find(mat) == todo.end()) continue;
      bool leaf = true;
      for (auto q : p.second.second) {
        string mat2 = q.first;
        if (todo.find(mat2) != todo.end()) leaf = false;
      }
      if (leaf) {
        order.push_back(mat);
        todo.erase(mat);
      }
    }

    // string s;
    // getline(cin,s);
  }

  // for (string s : order) {
  //   cout << s << endl;
  // }
  // cout << "--------" << endl;

  reverse(order.begin(), order.end());

  // for (string s : order) {
  //   cout << s << endl;
  // }
  // cout << "--------" << endl;

  long long xmin = 0;
  long long xmax = 10000000;
  while (xmax - xmin > 1) {
    long long xmiddle = (xmin + xmax) / 2;

    cout << "Amount of fuel: " << xmiddle << endl;

    map<string,long long> required_materials;
    required_materials["FUEL"] = xmiddle;

    for (string mat : order) {
      long long req = required_materials[mat];
      int amo = reactions[mat].first;
      long long num_reactions = (req + amo - 1) / amo;
      for (auto p : reactions[mat].second) {
        string mat2 = p.first;
        int amo2 = p.second;
        if (required_materials.find(mat2) == required_materials.end()) {
          required_materials[mat2] = amo2 * num_reactions;
        } else {
          required_materials[mat2] += amo2 * num_reactions;
        }
      }
    }

    cout << "Required ore: " << required_materials["ORE"] << endl;

    if (required_materials["ORE"] <= 1000000000000) {
      xmin = xmiddle;
    } else {
      xmax = xmiddle;
    }
  }

  cout << xmin << endl;
}
