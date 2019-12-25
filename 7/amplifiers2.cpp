#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
using namespace std;

bool run_intcode(vector<int>& tape, queue<int>& input, queue<int>& output, int& pc) {
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
      // cin >> n;
      if (input.empty()) return false;
      int n = input.front();
      input.pop();
      tape[tape[pc+1]] = n;
      pc += 2;
    } else if (instruction == 4) { // output
      // cout << tape[tape[pc+1]] << endl;
      output.push(tape[tape[pc+1]]);
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

  return true;
}

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data as command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  vector<int> reference_tape;
  int n;
  while (data_file >> n) {
    reference_tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  int best_thrust = 0;
  int phases[5] = {5,6,7,8,9};
  do {
    vector<int> tapes[5] = {reference_tape, reference_tape, reference_tape, reference_tape, reference_tape};
    int pcs[5] = {0,0,0,0,0};
    queue<int> channels[5];
    for (int i = 0; i < 5; i++) {
      channels[i].push(phases[i]);
    }
    channels[0].push(0);

    while (true) {
      bool done = true;
      for (int i = 0; i < 5; i++) {
        // cout << "Amplifier " << i << ":" << endl;
        // queue<int> tmp_input = channels[i];
        // cout << "Input:";
        // if (tmp_input.empty()) cout << " << No input. >>";
        // while (!tmp_input.empty()) {
        //   cout << ' ' << tmp_input.front();
        //   tmp_input.pop();
        // }
        // cout << endl;

        done &= run_intcode(tapes[i], channels[i], channels[(i+1)%5], pcs[i]);

        // queue<int> tmp_output = channels[(i+1)%5];
        // cout << "Output:";
        // if (tmp_output.empty()) cout << " << No output. >>";
        // while (!tmp_output.empty()) {
        //   cout << ' ' << tmp_output.front();
        //   tmp_output.pop();
        // }
        // cout << endl;
        //
        // if (done) cout << "I'm done." << endl;
      }

      if (done) {
        best_thrust = max(best_thrust, channels[0].front());
        break;
      }
    }
  } while (next_permutation(phases, phases+5));

  cout << best_thrust << endl;

  return 0;
}
