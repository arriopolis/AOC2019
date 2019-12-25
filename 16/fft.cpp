#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  vector<int> nums;
  ifstream data_file(argv[1]);
  char c;
  while (data_file >> c) {
    nums.push_back((int)c - (int)'0');
  }

  int base[4] = {0,1,0,-1};
  for (int i = 0; i < 100; i++) {
    vector<int> new_nums;
    for (int j = 1; j <= nums.size(); j++) {
      int t = 0;
      for (int k = 1; k <= nums.size(); k++) {
        int n = nums[k-1];
        t += base[(k/j)%4] * n;
      }
      new_nums.push_back(abs(t)%10);
    }
    nums = new_nums;
  }

  for (int n : nums) {
    cout << n;
  }
  cout << endl;
}
