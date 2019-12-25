#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <limits>
#include <string>
#include <set>
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
  vector<long long> tape;
  long long n;
  while (data_file >> n) {
    tape.push_back(n);
    data_file.ignore(numeric_limits<streamsize>::max(), ',');
  }

  // Initialize intcode computer
  int pc = 0;
  int relative_base = 0;
  queue<long long> inputs;
  queue<long long> outputs;

  // Initialize information containers
  map<pair<int,int>,char> poss;
  int poss_xmin = 0, poss_xmax = 0;
  int poss_ymin = 0, poss_ymax = 0;
  int x = 0, y = 0;
  poss[{x,y}] = '.';

  // Initialize command shorthands
  map<string,string> shorthands = {
    {"w", "north"},
    {"a", "west"},
    {"s", "south"},
    {"d", "east"}
  };

  set<string> directions;
  set<string> items;
  string area;
  string info;
  string s;
  set<string> inventory;
  while (true) {
    // Step the intcode program
    bool done = step_intcode(tape, pc, relative_base, inputs, outputs);

    // Get the output
    string msg;
    while (!outputs.empty()) {
      char c = outputs.front();
      outputs.pop();
      msg += c;
    }

    // Parse the output
    if (s == "north" || s == "west" || s == "east" || s == "south") {
      int room_pos = 0;
      while (msg.find("==", room_pos) != -1) {
        int area_pos_start = msg.find("==", room_pos) + 3;
        int area_pos_end = msg.find("==", area_pos_start) - 1;
        area = msg.substr(area_pos_start, area_pos_end - area_pos_start);
        room_pos = area_pos_end + 3;

        int info_pos_start = msg.find('\n', area_pos_end) + 1;
        int info_pos_end = msg.find('\n', info_pos_start);
        info = msg.substr(info_pos_start, info_pos_end - info_pos_start);

        directions.clear();
        int doors_pos = msg.find("Doors here lead:", room_pos);
        doors_pos = msg.find('\n', doors_pos) + 1;
        while (msg.substr(doors_pos,2) == "- ") {
          int end_line = msg.find('\n', doors_pos);
          directions.insert(msg.substr(doors_pos + 2, end_line - doors_pos - 2));
          doors_pos = end_line + 1;
        }

        items.clear();
        int items_pos = msg.find("Items here:", room_pos);
        if (items_pos != 0) {
          items_pos = msg.find('\n', items_pos) + 1;
          while (msg.substr(items_pos,2) == "- ") {
            int end_line = msg.find('\n', items_pos);
            items.insert(msg.substr(items_pos + 2, end_line - items_pos - 2));
            items_pos = end_line + 1;
          }
        }

        // Update data structures
        if (s == "south") y += (info != "Analyzing..." ? 2 : -2);
        else if (s == "west") x -= (info != "Analyzing..." ? 2 : -2);
        else if (s == "east") x += (info != "Analyzing..." ? 2 : -2);
        else if (s == "north") y -= (info != "Analyzing..." ? 2 : -2);

        poss_xmin = min(poss_xmin,x-1);
        poss_xmax = max(poss_xmax,x+1);
        poss_ymin = min(poss_ymin,y-1);
        poss_ymax = max(poss_ymax,y+1);

        poss[{x,y}] = '.';
        poss[{x,y+1}] = (directions.find("south") != directions.end() ? '.' : '#');
        poss[{x,y-1}] = (directions.find("north") != directions.end() ? '.' : '#');
        poss[{x+1,y}] = (directions.find("east") != directions.end() ? '.' : '#');
        poss[{x-1,y}] = (directions.find("west") != directions.end() ? '.' : '#');
        poss[{x-1,y-1}] = '#';
        poss[{x-1,y+1}] = '#';
        poss[{x+1,y-1}] = '#';
        poss[{x+1,y+1}] = '#';
      }
    } else if (s.substr(0,4) == "take") {
      if (msg.substr(5,4) == "take") {
        inventory.insert(s.substr(5));
        items.erase(s.substr(5));
      }
    } else if (s.substr(0,4) == "drop") {
      if (msg.substr(5,4) == "drop") {
        inventory.erase(s.substr(5));
        items.insert(s.substr(5));
      }
    }

    // Draw the control panel
    cout << "----------------" << endl;
    cout << "Message:" << endl;
    cout << msg << endl << endl;

    cout << "Map" << endl;
    for (int tmpy = poss_ymin; tmpy <= poss_ymax; tmpy++) {
      for (int tmpx = poss_xmin; tmpx <= poss_xmax; tmpx++) {
        if (tmpx == x && tmpy == y) {
          cout << 'X';
        } else if (poss.find({tmpx,tmpy}) == poss.end()) {
          cout << ' ';
        } else {
          cout << poss[{tmpx,tmpy}];
        }
      }
      cout << endl;
    }
    cout << "Current area: " << area << endl;
    cout << "Info: " << info << endl;
    cout << "Possible directions: ";
    for (string dir : directions) {
      cout << dir << ", ";
    }
    cout << endl;
    cout << "Items: ";
    for (string item : items) {
      cout << item << ", ";
    }
    cout << endl;
    cout << "Inventory: ";
    for (string item : inventory) {
      cout << item << ", ";
    }
    cout << endl;
    cout << "----------------" << endl;

    // Stop if you are done
    if (done) break;

    // Get user input
    getline(cin,s);

    // Parse user input
    if (shorthands.find(s) != shorthands.end()) {
      s = shorthands[s];
    }

    // Pass the input to the intcode program
    for (auto c : s) {
      inputs.push(c);
    }
    inputs.push('\n');
  }
}
