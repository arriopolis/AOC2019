#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

const long long deck_size = 10007;
// const long long deck_size = 10;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the name of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  string s;
  vector<string> shuffles;
  while (getline(data_file,s)) {
    shuffles.push_back(s);
  }

  long long a = 1;
  long long b = 0;

  for (string s : shuffles) {
    stringstream ss;
    ss << s;
    string w;
    ss >> w;
    if (w == "cut") {
      long long n;
      ss >> n;
      b = (b + deck_size - n) % deck_size;
    } else {
      // deal
      string w2;
      ss >> w2;
      if (w2 == "into") {
        // deal into new stack
        a = (deck_size - a) % deck_size;
        b = (deck_size - b - 1) % deck_size;
      } else {
        // deal with increment
        ss >> w2;
        long long n;
        ss >> n;
        a = (a * n) % deck_size;
        b = (b * n) % deck_size;
      }
    }
  }

  cout << "Permutation after 100 moves:" << endl;
  cout << a << "x + " << b << endl;

  long long x = 2019;
  cout << "The card 2019 ends up in position: " << (a * x + b) % deck_size << endl;
}
