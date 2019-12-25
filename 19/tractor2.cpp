#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <limits>
using namespace std;

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

bool step_intcode(vector<long long>& tape, int& pc, int& relative_base, queue<int>& inputs, queue<int>& outputs) {
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

int get_traction(vector<long long> tape, int x, int y) {
  int pc = 0;
  int relative_base = 0;
  queue<int> inputs;
  queue<int> outputs;
  inputs.push(x);
  inputs.push(y);

  bool done = step_intcode(tape, pc, relative_base, inputs, outputs);
  if (!done) {
    cout << "The intcode program did not halt." << endl;
    return 0;
  }

  if (outputs.empty()) {
    cout << "The intcode program did not return any output." << endl;
    return 0;
  }

  int res = outputs.front();
  outputs.pop();

  return res;
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

  int square_size = 100;

  vector<int> xmins = {0,0,0,2,3,0,4};
  vector<int> xmaxs = {1,0,0,3,4,0,5};
  int y = 7;
  while (true) {

    // for (int x = 0; x < 50; x++) {
    //   int res = get_traction(reference_tape, x, y);
    //   cout << (res == 1 ? '#' : '.');
    // }
    // cout << endl;

    int x = xmins[y-1];
    while (get_traction(reference_tape, x, y) == 0) {
      x++;
    }
    xmins.push_back(x);

    x = xmaxs[y-1];
    while (get_traction(reference_tape, x, y) == 1) {
      x++;
    }
    xmaxs.push_back(x);

    cout << "y = " << y << ", xmin = " << xmins[y] << ", xmax = " << xmaxs[y] << endl;

    if (y > square_size && xmaxs[y-square_size+1] - xmins[y] >= square_size) {
      cout << xmins[y] << ',' << y-square_size+1 << endl;
      return 0;
    }
    y++;
  }
}
