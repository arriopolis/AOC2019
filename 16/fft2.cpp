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

  int offset = 1000000 * nums[0] + 100000 * nums[1] + 10000 * nums[2] + 1000 * nums[3] + 100 * nums[4] + 10 * nums[5] + nums[6];

  vector<int> new_nums;
  for (int i = 0; i < 10000; i++) {
    for (int n : nums) {
      new_nums.push_back(n);
    }
  }
  nums = new_nums;

  int base[4] = {0,1,0,-1};
  for (int i = 0; i < 100; i++) {
    cout << "FFT iteration: " << i+1 << endl;

    vector<int> cumsums;
    int t = 0;
    cumsums.push_back(t);
    for (int n : nums) {
      t += n;
      cumsums.push_back(t);
    }

    vector<int> new_nums;
    for (int j = 1; j <= nums.size(); j++) {
      int t = 0;
      for (int k = 1; k <= nums.size() + 1; k += 2) {
        if (k*j > nums.size()) break;
        int start = k*j;
        int end = min((k+1)*j, (int)(nums.size()+1));
        int m = base[k%4];
        int a = m * (cumsums[end-1] - cumsums[start-1]);
        t += a;
      }
      new_nums.push_back(abs(t)%10);
    }
    nums = new_nums;
  }

  for (int i = offset; i < offset+8; i++) {
    cout << nums[i];
  }
  cout << endl;
}
