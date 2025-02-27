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
    for (size_t i = 0; i < results.size(); i++) {
        if (results[i].driver == driverName) {
            return i;
        }
    }
    return -1;
}
map<string,int> countTeamWins(const vector<F1Results> &results) {
    map<string, int> teamWins;
    for (const auto &result : results) {
        if (result.position == 1) {
            teamWins[result.team]++;
        }
    }
    return teamWins;
}

void filterByTeam(vector<F1Results> &results, const string &team) {
bool found = false;

    cout << "-----------------------------------------------------------------\n"
         << "| Race              | Driver          | Position | Fastest Lap |\n"
         << "-----------------------------------------------------------------\n";

    for (const auto &result : results) {
        if (result.team == team) {
            cout << "| " << left << setw(18) << result.race
            << "| " << setw(17) << result.driver
            << "| " << setw(9) << result.position
            << "| " << setw(11) << result.fastestLap
            << " |\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No results found for team " << team << endl;
    }
    cout << "-----------------------------------------------------------------\n";
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
            case 2: {
                string driver;
                cout << "Enter driver name: ";
                getline(cin, driver);
                int index = searchDriver(results, driver);
                if (index != -1) {
                    const F1Results& result = results[index];
                    cout <<"------------------------------------------\n"
                    << "| Race            | Position | Fastest Lap |\n"
                    <<"-------------------------?-----------------\n"
                    << "| "<< left << setw(16) << result.race
                    << "| "<< setw(9) << result.position
                    << "| "<< setw(11) << result.fastestLap
                    << " |\n"
                    <<"------------------------------------------\n";
                } else {
                    cout << "Driver not found.\n";
                }
                break;
            }
            case 3:{
                map<string, int> teamWins = countTeamWins(results);
                cout << "Team Wins:\n";
                cout << "----------------------------\n"
                     << "| Team           | Wins    |\n"
                     << "----------------------------\n";
                for (const auto &team : teamWins) {
                    cout << "| " << setw(15) << team.first
                    << "| " << setw(7) << team.second << " |\n";
                }
                cout << "----------------------------\n";
                break;
            }
            case 4: {
                string team;
                cout << "Enter team name: ";
                getline(cin, team);
                filterByTeam(results, team);
                break;
            }
        }
    }
}