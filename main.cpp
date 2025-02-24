#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct F1Results {
    string race;
    string driver;
    string team;
    int position;
    double fastestLap;
};

void loadData(vector<F1Results> &results, const string &filename) {
    ifstream ifile(filename);
    if (!ifile.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }
    string line;
    getline(ifile, line);

    while (getline(ifile, line)) {
        istringstream ss(line);
        F1Results result;
        string temp;

        getline(ss, result.race, ',');
        getline(ss, result.driver, ',');
        getline(ss, result.team, ',');

        getline(ss, temp, ',');
        result.position = stoi(temp);

        getline(ss, temp, ',');
        result.fastestLap = stod(temp);

        results.push_back(result);
    }
    ifile.close();
}

void showMenu() {
    cout << "\n1. Display all results\n";
    cout << "2. Search for a driver\n";
    cout << "3. Count teams by wins\n";
    cout << "4. Filter by team\n";
    cout << "5. Exit\n";
}

int main() {
    vector<F1Results> results;
    string filename = "F1_Data.csv";
    loadData(results, filename);
}