#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <bits/ranges_algo.h>

using namespace std;

struct F1Results {
    string race;
    string driver;
    string team;
    int position;
    double fastestLap;
};

struct LapStats {
    F1Results minLap;
    F1Results maxLap;
    double averageLap;
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
        try {
            getline(ss, result.race, ',');
            getline(ss, result.driver, ',');
            getline(ss, result.team, ',');

            getline(ss, temp, ',');
            result.position = stoi(temp);

            getline(ss, temp, ',');
            result.fastestLap = stod(temp);

            results.push_back(result);
        } catch (const invalid_argument &e) {
            cout << "Error parsing line: " << line << endl;
        }
        catch (const out_of_range &e) {
            cout << "Error parsing line: " << line << endl;
        }
    }
    ifile.close();
}

void displayResults(const vector<F1Results> &results) {
    cout << left << setw(25) << "Race"
            << setw(20) << "Driver"
            << setw(10) << "Team"
            << setw(10) << "Position"
            << setw(15) << "Fastest Lap"
            << endl;

    cout << string(25 + 20 + 10 + 10 + 15, '-') << endl;

    for (const auto &result: results) {
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

map<string, int> countTeamWins(const vector<F1Results> &results) {
    map<string, int> teamWins;
    for (const auto &result: results) {
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

    for (const auto &result: results) {
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

LapStats calculateLapStats(const vector<F1Results> &results, const string &race) {
    LapStats stats;
    bool raceFound = false;
    double totalLapTime = 0;
    int count = 0;

    for (const auto &result: results) {
        if (result.race == race) {
            stats.minLap = result;
            stats.maxLap = result;
            raceFound = true;
            break;
        }
    }
    if (!raceFound) {
        cout << "No results found for race: " << race << endl;
    }

    for (const auto &result: results) {
        if (result.race == race) {
            if (result.fastestLap < result.position) {
                stats.minLap = result;
            }
            if (result.fastestLap > result.position) {
                stats.maxLap = result;
            }
            totalLapTime += result.fastestLap;
            count++;
        }
    }
    stats.averageLap = totalLapTime / count;
    return stats;
}

string toLowercase(const string &str) {
    string lower = str;
    for (char &c: lower) {
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
    }
    return lower;
}

vector<F1Results> searchPartialMatch(const vector<F1Results> &results, const string &partialMatch,
                                     const string &searchTerm) {
    vector<F1Results> partialResults;
    string termLower = toLowercase(searchTerm);

    for (auto it = results.begin(); it != results.end(); it++) {
        string raceLower = toLowercase(it->race);
        string driverLower = toLowercase(it->driver);
        string teamLower = toLowercase(it->team);

        if (raceLower.find(termLower) != string::npos ||
            driverLower.find(termLower) != string::npos ||
            teamLower.find(termLower) != string::npos) {
            partialResults.push_back(*it);
        }
    }
    return partialResults;
}

void sortByFastestLap(vector<F1Results> &results) {
    sort(results.begin(), results.end(), [](const F1Results &r1, const F1Results &r2) {
        return r1.fastestLap > r2.fastestLap;
    });
}

void showMenu() {
    cout << "\n1. Display all results\n";
    cout << "2. Search for a driver\n";
    cout << "3. Count teams by wins\n";
    cout << "4. Filter by team\n";
    cout << "5. Calculate fastest Lap time for a track\n";
    cout << "6. Search for Partial Matches\n";
    cout << "7. Sort by track and fastest time (desc)\n";
    cout << "8. Exit\n";
}

int main() {
    vector<F1Results> results;
    string filename = "F1_Data.csv";
    loadData(results, filename);

    int choice;
    bool running = true;

    while (running) {
        showMenu();

        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore();
        }
        cin.ignore(1000, '\n');

        switch (choice) {
            case 1:
                cout << "All results\n";
                cout << "-------------------------------------------------------------------------------------\n"
                        << "| Race              | Driver           | Team              | Position | Fastest Lap |\n"
                        << "-------------------------------------------------------------------------------------\n";
                for (const auto &result: results) {
                    cout << "| " << left << setw(18) << result.race
                            << "| " << setw(17) << result.driver
                            << "| " << setw(18) << result.team
                            << "| " << setw(9) << result.position
                            << "| " << fixed << setprecision(2) << setw(11) << result.fastestLap
                            << " |\n";
                }
                cout << "-------------------------------------------------------------------------------------\n";
                break;
            case 2: {
                string driver;
                cout << "Enter driver name: ";
                getline(cin, driver);
                int index = searchDriver(results, driver);
                if (index != -1) {
                    const F1Results &result = results[index];
                    cout << "--------------------------------------------\n"
                            << "| Race            | Position | Fastest Lap |\n"
                            << "---------------------------------------------\n"
                            << "| " << left << setw(16) << result.race
                            << "| " << setw(9) << result.position
                            << "| " << fixed << setprecision(2) << setw(11) << result.fastestLap
                            << " |\n"
                            << "--------------------------------------------\n";
                } else {
                    cout << "Driver not found.\n";
                }
                break;
            }
            case 3: {
                map<string, int> teamWins = countTeamWins(results);
                cout << "Team Wins:\n";
                cout << "----------------------------\n"
                        << "| Team           | Wins    |\n"
                        << "----------------------------\n";
                for (const auto &team: teamWins) {
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
            case 5: {
                string race;
                cout << "Enter race name: ";
                getline(cin, race);

                try {
                    LapStats stats = calculateLapStats(results, race);

                    cout << "Fastest Lap Statistics for " << race << ":\n";
                    cout << "------------------------------------------------------------\n"
                            << "| Statistic | Driver          | Position | Fastest Lap (s) |\n"
                            << "------------------------------------------------------------\n"
                            << "| Min       | " << left << setw(16) << stats.minLap.driver
                            << "| " << setw(9) << stats.minLap.position
                            << "| " << fixed << setprecision(2) << setw(14) << stats.minLap.fastestLap
                            << " |\n"
                            << "| Max       | " << setw(16) << stats.maxLap.driver
                            << "| " << setw(9) << stats.maxLap.position
                            << "| " << setw(14) << stats.maxLap.fastestLap
                            << " |\n"
                            << "| Average   | " << setw(16) << "N/A"
                            << "| " << setw(9) << "N/A"
                            << "| " << setw(15) << stats.averageLap
                            << " |\n"
                            << "------------------------------------------------------------\n";
                } catch (const invalid_argument &e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 6: {
                string searchTerm;
                cout << "Enter partial match: ";
                getline(cin, searchTerm);

                vector<F1Results> partialResults = searchPartialMatch(results, searchTerm, searchTerm);
                if (partialResults.empty()) {
                    cout << "No results found for: " << searchTerm << "\n";
                } else {
                    cout << "Partial results for: " << searchTerm << "\n";
                    cout << "-----------------------------------------------------------------------------\n"
                            << "| Race              | Driver          | Team       | Position | Fastest Lap |\n"
                            << "-----------------------------------------------------------------------------\n";
                    for (const auto &result: partialResults) {
                        cout << "| " << left << setw(18) << result.race
                                << "| " << setw(17) << result.driver
                                << "| " << setw(10) << result.team
                                << "| " << setw(9) << result.position
                                << "| " << fixed << setprecision(2) << setw(11) << result.fastestLap
                                << " |\n";
                    }
                    cout << "-----------------------------------------------------------------------------\n";
                }
                break;
            }
            case 7: {
                string searchTerm;
                cout << "Enter Track to sort by: ";
                getline(cin, searchTerm);

                vector<F1Results> filteredResults;
                for (const auto &result: results) {
                    if (searchTerm == result.race) {
                        filteredResults.push_back(result);
                    }
                }
                if (filteredResults.empty()) {
                    cout << "No results found for: " << searchTerm << "\n";
                } else {
                    sortByFastestLap(filteredResults);


                    cout << "Results for " << searchTerm << " sorted by fastest lap time (descending):\n";
                    cout << "-------------------------------------------------------------------------------------\n"
                            << "| Race              | Driver          | Team               | Position | Fastest Lap |\n"
                            << "-------------------------------------------------------------------------------------\n";
                    for (const auto &result: filteredResults) {
                        cout << "| " << left << setw(18) << result.race
                                << "| " << setw(17) << result.driver
                                << "| " << setw(18) << result.team
                                << "| " << setw(9) << result.position
                                << "| " << fixed << setprecision(2) << setw(11) << result.fastestLap
                                << " |\n";
                    }
                    cout << "-------------------------------------------------------------------------------------\n";
                }
                break;
            }
            case 8:
                running = false;
                cout << "Exiting the system\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
    return 0;
}
