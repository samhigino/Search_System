#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream in("tests/input_1.txt", fstream::in);
    ofstream out("output/output.txt", fstream::out);
    ofstream debug("output/debug.txt", fstream::out);

    if(!debug.is_open()) {
        cout << "Error while opening debug.\n";
        return 1;
    }
    if(!in.is_open() || !out.is_open()) {
        debug << "Error while opening in or out.\n";
        return 2;
    }

    return 0;
}