#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <limits>
#include <string>
using namespace std;

const int num_devices = 50;

int mypow(int a, int b) {
  if (b == 0) return 1;
  if (b == 1) return a;
  int tmp = mypow(a,b/2);
  if (b%2 == 0) return tmp * tmp;
  else return a * tmp * tmp;
}

void write_to_tape(vector<long long>& tape, int pos, long long val) {
  while (tape.size() < pos+1) tape.push_back(0);
  tape[pos] = val;
}

long long read_from_tape(vector<long long>& tape, int pos) {
  while (tape.size() < pos+1) tape.push_back(0);
  return tape[pos];
}

long long get_position(vector<long long>& tape, int pc, long long opcode, int num, int relative_base) {
  long long mode = (opcode % mypow(10, num+2)) / mypow(10,num+1);
  long long pos = (mode == 0 ? read_from_tape(tape, pc+num) :
                  (mode == 1 ? pc + num :
                               read_from_tape(tape, pc+num) + relative_base));
  return pos;
}

long long get_parameter(vector<long long>& tape, int pc, long long opcode, int num, int relative_base) {
  long long pos = get_position(tape, pc, opcode, num, relative_base);
  return read_from_tape(tape, pos);
}

bool step_intcode(vector<long long>& tape, int& pc, int& relative_base, queue<long long>& inputs, queue<long long>& outputs) {
  while (true) {
    // cout << "Current state:" << endl;
    // for (long long i : tape) {
    //   cout << i << ' ';
    // }
    // cout << endl;
    // cout << "PC: " << pc << endl;
    // cout << "RB: " << relative_base << endl;

    long long opcode = read_from_tape(tape, pc);
    int instruction = opcode%100; // add
    if (instruction == 1) {
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      long long output = get_position(tape, pc, opcode, 3, relative_base);
      write_to_tape(tape, output, input1 + input2);
      pc += 4;
    } else if (instruction == 2) { // multiply
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      long long output = get_position(tape, pc, opcode, 3, relative_base);
      write_to_tape(tape, output, input1 * input2);
      pc += 4;
    } else if (instruction == 3) { // input
      long long n;
      if (inputs.empty()) return false;
      n = inputs.front();
      inputs.pop();
      long long output = get_position(tape, pc, opcode, 1, relative_base);
      write_to_tape(tape, output, n);
      pc += 2;
    } else if (instruction == 4) { // output
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      outputs.push(input1);
      pc += 2;
    } else if (instruction == 5) { // jump if true
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      if (input1 != 0) {
        pc = input2;
      } else {
        pc += 3;
      }
    } else if (instruction == 6) { // jump if false
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      if (input1 == 0) {
        pc = input2;
      } else {
        pc += 3;
      }
    } else if (instruction == 7) { // less than
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      long long output = get_position(tape, pc, opcode, 3, relative_base);
      write_to_tape(tape, output, (input1 < input2 ? 1 : 0));
      pc += 4;
    } else if (instruction == 8) { // equals than
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      long long input2 = get_parameter(tape, pc, opcode, 2, relative_base);
      long long output = get_position(tape, pc, opcode, 3, relative_base);
      write_to_tape(tape, output, (input1 == input2 ? 1 : 0));
      pc += 4;
    } else if (instruction == 9) {
      long long input1 = get_parameter(tape, pc, opcode, 1, relative_base);
      relative_base += input1;
      pc += 2;
    } else if (instruction == 99) { // halt
      return true;
    } else {
      cout << "Unknown opcode encountered." << endl;
      return true;
    }
  }
}

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  vector<long long> reference_tape;
  long long n;
  while (data_file >> n) {
    reference_tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  vector<long long> tapes[num_devices];
  bool dones[num_devices];
  int pcs[num_devices];
  int relative_bases[num_devices];
  queue<long long> inputs[num_devices];
  queue<long long> outputs[num_devices];
  for (int i = 0; i < num_devices; i++) {
    tapes[i] = reference_tape;
    dones[i] = false;
    pcs[i] = 0;
    relative_bases[i] = 0;
    inputs[i].push(i);
  }

  long long nat_x, nat_y;
  long long last_x = -1, last_y = -1;

  while (true) {
    bool progress = false;
    for (int i = 0; i < num_devices; i++) {
      if (dones[i]) {
        cout << "Skipping device " << i << " because it is already done." << endl;
        continue;
      }
      if (inputs[i].empty()) {
        inputs[i].push(-1);
      } else {
        progress = true;
      }
      dones[i] = step_intcode(tapes[i], pcs[i], relative_bases[i], inputs[i], outputs[i]);
      while (!outputs[i].empty()) {
        int device = outputs[i].front();
        outputs[i].pop();
        long long x = outputs[i].front();
        outputs[i].pop();
        long long y = outputs[i].front();
        outputs[i].pop();
        cout << "The following message is sent to address " << device << ": " << x << ',' << y << endl;
        if (device == 255) {
          nat_x = x;
          nat_y = y;
        } else {
          inputs[device].push(x);
          inputs[device].push(y);
        }
      }
    }

    if (!progress) {
      if (nat_y == last_y) {
        cout << "The y-value " << nat_y << " was delivered twice in a row." << endl;
        return 0;
      }
      inputs[0].push(nat_x);
      inputs[0].push(nat_y);
      last_x = nat_x;
      last_y = nat_y;
    }
  }
}
