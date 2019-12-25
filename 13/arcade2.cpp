#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <limits>
#include <thread>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
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

  tape[0] = 2;

  int pc = 0;
  int relative_base = 0;
  queue<int> inputs;
  queue<int> outputs;

  map<pair<int,int>,int> screen;
  int score = 0;
  int xmin = 0, xmax = 0, ymin = 0, ymax = 0;

  struct termios oldSettings, newSettings;

  tcgetattr( fileno( stdin ), &oldSettings );
  newSettings = oldSettings;
  newSettings.c_lflag &= (~ICANON & ~ECHO);
  tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

  while (true)
  {
    fd_set set;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    FD_ZERO( &set );
    FD_SET( fileno( stdin ), &set );

    int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

    char c = '.';
    if( res > 0 )
    {
      // printf( "Input available\n" );
      read( fileno( stdin ), &c, 1 );
      cout << c << endl;
    }
    else if( res < 0 )
    {
      perror( "select error" );
      break;
    }
    // else
    // {
    //   printf( "Select timeout\n" );
    // }

    bool done = step_intcode(tape, pc, relative_base, inputs, outputs);
    if (done) return 0;

    while (!outputs.empty()) {
      int x = outputs.front();
      outputs.pop();
      int y = outputs.front();
      outputs.pop();
      int obj = outputs.front();
      outputs.pop();
      if (x == -1 && y == 0) {score = obj;}
      else {
        xmin = min(xmin,x);
        xmax = max(xmax,x);
        ymin = min(ymin,y);
        ymax = max(ymax,y);
        screen[{x,y}] = obj;
      }
    }

    for (auto p : screen) {
      cout << p.first.first << ' ' << p.first.second << ' ' << p.second << endl;
    }

    for (int y = ymin; y <= ymax; y++) {
      for (int x = xmin; x <= xmax; x++) {
        if (screen.find({x,y}) != screen.end()) {
          int obj = screen[{x,y}];
          if (obj == 1) {cout << '#';}
          else if (obj == 2) {cout << 'X';}
          else if (obj == 3) {cout << '-';}
          else if (obj == 4) {cout << 'O';}
          else {cout << ' ';}
        }
      }
      cout << endl;
    }
    cout << "Score: " << score << endl;

    long long input = 0;
    if (c == 'a') {input = -1;}
    if (c == 'd') {input = 1;}

    inputs.push(input);
  }

  tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
  return 0;
}
