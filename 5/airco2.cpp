#include <iostream>
using namespace std;

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

  int pc = 0;
  while (true) {
    int opcode = tape[pc];
    int instruction = opcode%100; // add
    if (instruction == 1) {
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      tape[tape[pc+3]] = input1 + input2;
      pc += 4;
    } else if (instruction == 2) { // multiply
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      tape[tape[pc+3]] = input1 * input2;
      pc += 4;
    } else if (instruction == 3) { // input
      cin >> n;
      tape[tape[pc+1]] = n;
      pc += 2;
    } else if (instruction == 4) { // output
      cout << tape[tape[pc+1]] << endl;
      pc += 2;
    } else if (instruction == 5) { // jump if true
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      if (input1 != 0) {
        pc = input2;
      } else {
        pc += 3;
      }
    } else if (instruction == 6) { // jump if false
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      if (input1 == 0) {
        pc = input2;
      } else {
        pc += 3;
      }
    } else if (instruction == 7) { // less than
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      tape[tape[pc+3]] = (input1 < input2 ? 1 : 0);
      pc += 4;
    } else if (instruction == 8) { // equals than
      int mode1 = (opcode % 1000) / 100;
      int input1 = (mode1 == 1 ? tape[pc+1] : tape[tape[pc+1]]);
      int mode2 = (opcode % 10000) / 1000;
      int input2 = (mode2 == 1 ? tape[pc+2] : tape[tape[pc+2]]);
      tape[tape[pc+3]] = (input1 == input2 ? 1 : 0);
      pc += 4;
    } else if (instruction == 99) { // halt
      break;
    } else {
      cout << "Unknown opcode encountered." << endl;
      break;
    }
  }

  return 0;
}
