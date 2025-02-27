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

void displayResults(vector<F1Results> &results) {
    cout << "|Race       |Driver      |Team     |Position    |Fastest Lap    |\n";
    cout << "---------------------------------------------------------------------";
    for (const auto &result: results) {
        cout << result.race << " | ";
        cout << result.driver << " | ";
        cout << result.team << " | ";
        cout << result.position << " | ";
        cout << result.fastestLap << " | \n";
    }
}

int searchDriver(const vector<F1Results> &results, const string &driverName) {
    bool found = false;

    cout << "Results for " << driverName << ":\n";
    cout << "--------------------------------------------\n";
    cout << "|  Race  |  Team  | Position | Fastest Lap |\n";
    cout << "--------------------------------------------\n";

    for (const auto &result : results) {
        if (result.driver == driverName) {
            cout << "| " << result.race << " | "
            << result.team << " | "
            << result.position << " | "
            << result.fastestLap << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No races found for " << driverName << ".\n";
    }
}
void countTeamWins(const vector<F1Results> &results) {
    map<string, int> teamWins;
    for (const auto &result : results) {
        if (result.position == 1) {
            teamWins[result.team]++;
        }
    }
    cout << "Team wins: \n";
    for (const auto &teamWin : teamWins) {
        cout << teamWin.first << " " << teamWin.second << "\n";
    }
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

    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            displayResults(results);
        }else if (choice == 2) {
            string driver;
            cout << "Enter driver name: ";
            getline(cin, driver);

            int index = searchDriver(results, driver);
            if (index != -1) {
                cout << results[index].driver << ":\n"
                << results[index].race << " | "
                << results[index].team << " | "
                << results[index].position << " | "
                << results[index].fastestLap << "|\n";
            } else {
                cout << "Driver not found.\n";
            }
        }else if (choice == 3) {
            countTeamWins(results);
        }else if (choice == 4) {

        }else if (choice == 5) {
            break;
        }
    }
    return 0;
}