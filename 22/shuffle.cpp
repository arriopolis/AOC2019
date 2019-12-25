#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

const int deck_size = 10007;

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

  vector<int> deck;
  for (int i = 0; i < deck_size; i++) {
    deck.push_back(i);
  }

  for (string s : shuffles) {
    stringstream ss;
    ss << s;
    string w;
    ss >> w;
    if (w == "cut") {
      int n;
      ss >> n;
      vector<int> new_deck;
      if (n < 0) n += deck_size;
      for (int i = n; i < deck_size + n; i++) {
        new_deck.push_back(deck[i%deck_size]);
      }
      deck = new_deck;
    } else {
      // deal
      string w2;
      ss >> w2;
      if (w2 == "into") {
        // deal into new stack
        reverse(deck.begin(), deck.end());
      } else {
        // deal with increment
        ss >> w2;
        int n;
        ss >> n;
        vector<int> new_deck;
        for (int i = 0; i < deck_size; i++) {
          new_deck.push_back(0);
        }
        for (int i = 0; i < deck_size; i++) {
          new_deck[(n*i)%deck_size] = deck[i];
        }
        deck = new_deck;
      }
    }
  }

  for (int n : deck) {
    cout << n << ' ';
  }
  cout << endl;

  for (int i = 0; i < deck_size; i++) {
    if (deck[i] == 2019) {
      cout << i << endl;
    }
  }
}
