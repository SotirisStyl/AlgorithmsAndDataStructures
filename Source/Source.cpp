#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <ratio>
#include <chrono>
#include "Stack.h"

using namespace std;
using namespace std::chrono;


#define SIZE 1000
#define MIN 0
#define MAX 10000
#define KEYS_S 1000
#define MAX_L 100

// Box data structure
struct Box {
    // Assign width, length, name of the box
    unsigned w;
    unsigned l;
    char n;
};

// PackerProblem data structure
struct PackerProblem {
    // Assign container width, length, number of boxes a a box array
    unsigned c_w;
    unsigned c_l;
    unsigned num;
    Box* box;
};

PackerProblem* loadPackerProblem(string f) {
    // Open file
    ifstream in(f);

    //Check if the problem is found or not
    if (!in) {
        cout << "Error: Could not open file " << f << endl;
        return 0;
    }

    PackerProblem* prob = new PackerProblem;

    // Read the container width and length
    in >> prob->c_w >> prob->c_l;

    // Read the number of boxes
    in >> prob->num;

    // Allocate array for boxes
    prob->box = new Box[prob->num];

    // Read the box width, length and name
    for (int p = 0; p < prob->num; p++) {
        Box& box = prob->box[p];
        in >> box.w >> box.l >> box.n;
    }

    // Print container configuration
    cout << "Container Configuration (" << prob->c_w << "x" << prob->c_l << ")" << endl;
    cout << "+";
    for (unsigned q = 0; q < prob->c_w; q++) {
        cout << "-";
    }
    cout << "+" << endl;
    for (unsigned q = 0; q < prob->c_l; q++) {
        cout << "|";
        for (unsigned i = 0; i < prob->c_w; i++) {
            cout << "X";
        }
        cout << "|" << endl;
    }
    cout << "+";
    for (unsigned p = 0; p < prob->c_w; p++) {
        cout << "-";
    }
    cout << "+" << endl;

    // Print box configurations
    cout << "Boxes to be placed (" << prob->num << "):" << endl << endl;
    for (unsigned q = 0; q < prob->num; q++) {
        Box& box = prob->box[q];
        cout << box.n << " (" << box.w << "x" << box.l << ")" << endl;
        for (unsigned k = 0; k < box.l; k++) {
            for (unsigned p = 0; p < box.w; p++) {
                cout << box.n;
            }
            cout << endl;
        }
        cout << endl;
    }

    return prob;
}



struct PackerSolver {
    PackerProblem* problem;
    string container[100][100]; // assume max size of 100x100
    stack<pair<unsigned, unsigned>> positionStack;

    // Function to place a box at a given position
    void placeBox(int x, int y, const Box& box) {
        // Mark cells as occupied by the box
        for (unsigned j = x; j < x + box.w; j++) {
            for (unsigned k = y; k < y + box.l; k++) {
                container[j][k] = box.n;
            }
        }

        // Push position onto stack
        positionStack.push(make_pair(x, y));
    }
};


PackerSolver* solveProblem(PackerProblem* prob) {
    // Initialize container with zeros
    PackerSolver* solution = new PackerSolver();
    solution->problem = prob;
    memset(solution->container, 0, sizeof(solution->container));

    // Initialize position stack
    pair<unsigned, unsigned>* stackPositon = new pair<unsigned, unsigned>[prob->num + 1];
    unsigned Top = 0;
    stackPositon[Top] = make_pair(0, 0);

    // Loop until all boxes have been placed
    unsigned index = 0;
    while (index < solution->problem->num) {
        // Get the next box to be placed
        Box box = solution->problem->box[index];

        // Check if the box can be placed in the current position
        pair<unsigned, unsigned> position = stackPositon[Top];
        if (position.first + box.w <= solution->problem->c_w && position.second + box.l <= solution->problem->c_l) {
            // Place the box in the container
            for (unsigned k = position.first; k < position.first + box.w; k++) {
                for (unsigned q = position.second; q < position.second + box.l; q++) {
                    solution->container[k][q] = index + 1;
                }
            }

            // Push the next position to the stack
            Top++;
            stackPositon[Top] = make_pair(position.first, position.second + box.l);

            // Move to the next box
            index++;
        }
        else {
            // The box cannot be placed in the current position, so backtrack
            Top--;

            // Check if there are no more positions in the stack (i.e., the box cannot be placed anywhere)
            if (Top == 0) {
                return solution;
            }
        }
    }

    return solution;
}

//Function to print the container
void printContainer(int container[][MAX_L], int c_w, int c_l) {
    cout << "Container Configuration (" << c_w << "x" << c_l << ")" << endl;
    cout << "+";
    for (int q = 0; q < c_w; q++) {
        cout << "-";
    }
    cout << "+" << endl;
    for (int d = 0; d < c_l; d++) {
        cout << "|";
        for (int i = 0; i < c_w; i++) {
            if (container[i][d] == 0) {
                cout << " ";
            }
            else {
                cout << container[i][d];
            }
        }
        cout << "|" << endl;
    }
    cout << "+";
    for (int h = 0; h < c_w; h++) {
        cout << "-";
    }
    cout << "+" << endl;
}

int main() {

    high_resolution_clock::time_point start, finish;
    duration<double> duration;

    // Load PackerProblem from file
    PackerProblem* problem = loadPackerProblem("input.txt");

    PackerSolver* solver = solveProblem(problem);

    start = high_resolution_clock::now();
    /*keypos = linearSearch(array, 0, ARRAY_SIZE - 1, keys[key Index]);*/
    finish = high_resolution_clock::now();
    duration = finish - start;
    cout << "Time: " << duration.count() << " s\n";

    // Deallocate dynamically allocated memory
    delete[] problem->box;
    delete problem;

    return 0;
}