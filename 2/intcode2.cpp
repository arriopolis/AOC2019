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
  vector<int> input_tape;
  int n;
  while (data_file >> n) {
    input_tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      vector<int> tape;
      for (int k = 0; k < input_tape.size(); k++) {
        tape.push_back(input_tape[k]);
      }

      tape[1] = i;
      tape[2] = j;

      int pc = 0;
      while (true) {
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

      if (tape[0] == 19690720) {
        cout << 100 * i + j << endl;
        return 0;
      }
    }
  }

  cout << "The correct configuration was not found." << endl;
  return 0;
}
