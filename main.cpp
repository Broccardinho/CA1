#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

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
        try{

            getline(ss, result.race, ',');
            getline(ss, result.driver, ',');
            getline(ss, result.team, ',');

            getline(ss, temp, ',');
            result.position = stoi(temp);

            getline(ss, temp, ',');
            result.fastestLap = stod(temp);

            results.push_back(result);
        }
        catch (const invalid_argument& e) {
            cout << "Error parsing line: " << line << endl;
        }
        catch (const out_of_range& e) {
            cout << "Error parsing line: " << line << endl;
        }
    }
    ifile.close();
}

void displayResults(vector<F1Results> &results) {
    cout << left << setw(25) << "Race"
    << setw(20) << "Driver"
    << setw(10) << "Team"
    << setw(10) << "Position"
    << setw(15) << "Fastest Lap"
    << endl;

    cout << string(25 + 20 + 10 + 10 + 15, '-') << endl;

    for (const auto &result : results) {
        cout << left << setw(25) << result.race
        << setw(20) << result.driver
        << setw(10) << result.team
        << setw(10) << result.position
        << setw(15) << result.fastestLap
        << endl;
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
    return -1;
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
void filterByTeam(const vector<F1Results> &results, const string &team) {
    bool found = false;
    for (const auto &result: results) {
        if (result.team == team) {
            cout << result.race << " | ";
            cout << result.driver << " | ";
            cout << result.position << " | ";
            cout << result.fastestLap << " | \n";
            found = true;
        }
    }
    if (!found) {
        cout << "No races found for team " << team << ".\n";
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
    bool running = true;

    while (running) {
        showMenu();
        cin >> choice;
        cin.ignore(1000, '\n');

        switch (choice) {
            case 1:
                displayResults(results);
                break;
            case 2:
            break;
        }
    }
}