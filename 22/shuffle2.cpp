#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

const long long deck_size = 119315717514047;
const long long num_iterations = 101741582076661;
const long long pos = 2020;

// const long long deck_size = 10007;
// const long long num_iterations = 1;
// const long long pos = 8502;

// const long long deck_size = 10;

long long find_inverse(long long n) {
  long long preva = 1;
  long long prevb = 0;
  long long prevc = deck_size;
  long long a = 0;
  long long b = 1;
  long long c = n;
  while (c > 1) {
    long long m = prevc / c;
    long long newa = preva - m*a;
    long long newb = prevb - m*b;
    long long newc = prevc - m*c;
    // cout << newa << '*' << deck_size << '+' << newb << '*' << n << '=' << newc << endl;
    preva = a;
    prevb = b;
    prevc = c;
    a = newa;
    b = newb;
    c = newc;
  }
  return ((b % deck_size) + deck_size) % deck_size;
}

long long mult_mod(long long a, long long b, long long m) {
  long long res = 0;
  while (b > 0) {
    if (b % 2 == 1) res = (res + a) % m;
    a = (a << 1) % m;
    b >>= 1;
  }
  return res;
}

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

  reverse(shuffles.begin(), shuffles.end());

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
      b = (b + deck_size + n) % deck_size;
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
        long long inv_n = find_inverse(n);
        a = mult_mod(a, inv_n, deck_size);
        // a = (a * inv_n) % deck_size;
        b = mult_mod(b, inv_n, deck_size);
        // b = (b * inv_n) % deck_size;
      }
    }
    // cout << a << "x + " << b << endl;
  }

  cout << "Inverse permutation after 100 moves:" << endl;
  cout << a << "x + " << b << endl;

  long long x = pos;
  long long n = num_iterations;
  while (n > 0) {
    if (n % 2 == 1) {
      x = (mult_mod(a,x,deck_size) + b) % deck_size;
    }
    b = (mult_mod(a,b,deck_size) + b) % deck_size;
    // b = (b + a * b) % deck_size;
    a = mult_mod(a,a,deck_size);
    // a = (a * a) % deck_size;
    // cout << "Duplicated permutation: " << endl;
    // cout << a << "x + " << b << endl;
    n >>= 1;
  }
  cout << "The card at position " << pos << " has value: " << x << endl;
}
