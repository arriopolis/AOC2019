#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data as command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  vector<int> tape;
  int n;
  while (data_file >> n) {
    tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  tape[1] = 12;
  tape[2] = 2;

  int pc = 0;
  while (true) {
    // for (int i : tape) {
    //   cout << i << ' ';
    // }
    // cout << endl;
    if (tape[pc] == 1) {
      tape[tape[pc+3]] = tape[tape[pc+2]] + tape[tape[pc+1]];
      pc += 4;
    } else if (tape[pc] == 2) {
      tape[tape[pc+3]] = tape[tape[pc+2]] * tape[tape[pc+1]];
      pc += 4;
    } else if (tape[pc] == 99) {
      break;
    } else {
      cout << "Unknown opcode encountered." << endl;
      return 0;
    }
  }

  cout << tape[0] << endl;
}
