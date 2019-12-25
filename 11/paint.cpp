#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <limits>
#include <string>
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

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    cout << "Please supply the filename of the data file as a command line argument." << endl;
    return 0;
  }

  ifstream data_file(argv[1]);
  vector<long long> tape;
  long long n;
  while (data_file >> n) {
    tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  int pc = 0;
  int relative_base = 0;
  queue<int> inputs;
  queue<int> outputs;
  set<pair<int,int>> white;
  set<pair<int,int>> touched;
  bool done = false;
  int x = 0, y = 0;
  int dx = 0, dy = -1;
  while (!done) {
    int input = (white.find({x,y}) == white.end() ? 0 : 1);
    inputs.push(input);

    // cout << "x = " << x << ", y = " << y << endl;
    // cout << "Input: " << input << endl;

    done = step_intcode(tape, pc, relative_base, inputs, outputs);

    touched.insert({x,y});
    int color = outputs.front();
    outputs.pop();
    if (color == 0) {
      white.erase({x,y});
    } else if (color == 1) {
      white.insert({x,y});
    } else {
      cout << "A wrong color code was obtained: " << color << endl;
      return 0;
    }

    // cout << "Color: " << color << endl;

    int turn = outputs.front();
    outputs.pop();
    int newdx,newdy;
    if (turn == 0) {
      newdx = dy;
      newdy = -dx;
    } else if (turn == 1) {
      newdx = -dy;
      newdy = dx;
    } else {
      cout << "A wrong turn code was obtained: " << turn << endl;
      return 0;
    }

    if (!outputs.empty()) {
      cout << "Not all outputs were read." << endl;
      return 0;
    }

    // cout << "Turn: " << turn << endl;
    // cout << "White:" << endl;
    // for (pair<int,int> p : white) {
    //   cout << p.first << ',' << p.second << endl;
    // }
    // string s;
    // getline(cin,s);

    dx = newdx;
    dy = newdy;
    x += dx;
    y += dy;
  }

  cout << touched.size() << endl;
}
