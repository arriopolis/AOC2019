#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <limits>
#include <set>
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

  int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
  int x = 0, y = 0;

  map<pair<int,int>,int> area;
  area[{0,0}] = 1;
  set<pair<int,int>> dead_ends;

  // map<char,int> key_to_dir;
  // key_to_dir['w'] = 1;
  // key_to_dir['d'] = 4;
  // key_to_dir['s'] = 2;
  // key_to_dir['a'] = 3;

  pair<int,int> offsets[4] = {{0,-1},{0,1},{-1,0},{1,0}};
  int next_dir = 0;
  int last_dir = 0;
  // int ctr = 0;
  while (true) {
    // char c = '.';
    // while (key_to_dir.find(c) == key_to_dir.end()) {
    //   cout << "Please enter the direction in which you want to move." << endl;
    //   cin >> c;
    // }

    // Move
    // inputs.push(key_to_dir[c]);
    // cout << "Moving in direction " << next_dir << "." << endl;
    inputs.push(next_dir+1);
    step_intcode(tape, pc, relative_base, inputs, outputs);
    int res = outputs.front();
    outputs.pop();
    if (!outputs.empty()) {
      cout << "The droid returned more output than was expected." << endl;
      return 0;
    }

    // Interpret result
    // int dx = offsets[key_to_dir[c]-1].first, dy = offsets[key_to_dir[c]-1].second;
    int dx = offsets[next_dir].first, dy = offsets[next_dir].second;
    xmin = min(xmin,x+dx);
    xmax = max(xmax,x+dx);
    ymin = min(ymin,y+dy);
    ymax = max(ymax,y+dy);
    if (res == 0) {
      area[{x+dx,y+dy}] = 0;
    } else if (res == 1) {
      area[{x+dx,y+dy}] = 1;
      x += dx;
      y += dy;
      last_dir = next_dir;
    } else if (res == 2) {
      area[{x+dx,y+dy}] = 2;
      x += dx;
      y += dy;
      last_dir = next_dir;
      cout << "The droid has found the oxygen system at x = " << x << " and y = " << y << endl;
    }

    // Decide where to go next
    int num_not_good = 0;
    int poss_dir = -1;
    for (int new_dir = 0; new_dir < 4; new_dir++) {
      pair<int,int> o = offsets[new_dir%4];
      int dx = o.first, dy = o.second;
      if (area.find({x+dx,y+dy}) != area.end() && (area[{x+dx,y+dy}] == 0 || dead_ends.find({x+dx,y+dy}) != dead_ends.end())) {
        num_not_good++;
      } else if (new_dir != (last_dir ^ 1)) {
        poss_dir = new_dir;
      } else if (poss_dir == -1) {
        poss_dir = new_dir;
      }
    }
    next_dir = poss_dir;

    if (num_not_good == 4) {
      cout << "I'm stuck!" << endl;
      cout << "Current area:" << endl;
      for (int y2 = ymin; y2 <= ymax; y2++) {
        for (int x2 = xmin; x2 <= xmax; x2++) {
          if (x2 == x && y2 == y) {cout << 'O';}
          else if (area.find({x2,y2}) == area.end()) {cout << ' ';}
          else if (area[{x2,y2}] == 0) {cout << '#';}
          else if (area[{x2,y2}] == 1) {
            if (dead_ends.find({x2,y2}) == dead_ends.end()) {cout << '.';}
            else {cout << ',';}
          }
          else if (area[{x2,y2}] == 2) {cout << 'X';}
        }
        cout << endl;
      }
      break;
    }

    if (num_not_good == 3) {
      dead_ends.insert({x,y});
    }

    // if (ctr % 1000 == 0) {
    //   cout << "Current area:" << endl;
    //   for (int y2 = ymin; y2 <= ymax; y2++) {
    //     for (int x2 = xmin; x2 <= xmax; x2++) {
    //       if (x2 == x && y2 == y) {cout << 'O';}
    //       else if (area.find({x2,y2}) == area.end()) {cout << ' ';}
    //       else if (area[{x2,y2}] == 0) {cout << '#';}
    //       else if (area[{x2,y2}] == 1) {
    //         if (dead_ends.find({x2,y2}) == dead_ends.end()) {cout << '.';}
    //         else {cout << ',';}
    //       }
    //       else if (area[{x2,y2}] == 2) {cout << 'X';}
    //     }
    //     cout << endl;
    //   }
    //   string s;
    //   getline(cin,s);
    // }
    //
    //
    // ctr++;
  }

  map<pair<int,int>,int> dists;
  set<pair<int,int>> frontier;
  frontier.insert({0,0});
  dists[{0,0}] = 0;
  while (!frontier.empty()) {
    set<pair<int,int>> new_frontier;
    for (auto p : frontier) {
      int x = p.first, y = p.second;
      for (auto o : offsets) {
        int dx = o.first, dy = o.second;
        int newx = x+dx, newy = y+dy;
        if (dists.find({newx,newy}) != dists.end()) continue;
        if (area[{newx,newy}] == 0) continue;
        dists[{newx,newy}] = dists[{x,y}] + 1;
        new_frontier.insert({newx,newy});
      }
    }
    frontier = new_frontier;
  }

  cout << dists[{14,-20}] << endl;
}
