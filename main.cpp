#include <iostream>
#include <sstream>
#include <fstream>

#include "graph.h"

using namespace std;

string get_input(string prompt) {
    string input;
    cout << prompt << ": ";
    std::getline(cin, input);
    return input;
}

int get_airport(const Graph & graph, const string prompt) {
    string city = get_input(prompt);
    vector<int> city_airports = graph.get_airports_in_city(city);

    while (city_airports.empty()) {
        cout << "No Airports Found" << endl;
        city = get_input(prompt);
        city_airports = graph.get_airports_in_city(city);
    }

    vector<Airport*> airports = graph.get_airports();
    for (unsigned int i = 0; i < city_airports.size(); i++) {
        cout << "[" << i + 1 << "] " << airports[city_airports[i]] << endl;
    }

    stringstream ap_prompt;
    ap_prompt << "Select Airport [1";
    if (city_airports.size() == 1) {
        ap_prompt << "]";
    } else {
        ap_prompt << "-" << city_airports.size() << "]";
    }

    unsigned long airport = std::stoi(get_input(ap_prompt.str()));
    while (airport < 1 || airport > city_airports.size()) {
        cout << "Invalid Airport" << endl;
        airport = std::stoi(get_input(ap_prompt.str()));
    }
    return city_airports[airport - 1];
}

void print_path(const vector<Route*> & path, int dest_id) {
    if (path.empty()) {
        cout << "No Path Found" << endl;
    } else if (path.back() -> get_dest() -> get_id() != dest_id) {
        cout << "Partially Complete Path Found:" << endl;
    } else {
        float cost = 0;
        for (Route * route : path) {
            cost += route -> get_weight();
        }
        cout << "Complete Path Found in " << path.size()
             << " Route(s) at " << cost << " cost:" << endl;
    }

    for (Route * route : path) {
        cout << route << endl;
    }
}

void path_search(Graph & graph, vector<int> & airports) {
    print_path(graph.bfs(airports), airports.back());
    print_path(graph.a_star_search(airports), airports.back());
}

void prim_mst(Graph & graph, int start_id, string filename) {
    Graph mst(false);
    vector<Route*> tree = graph.prim_mst(mst, start_id);

    float cost = 0;
    stringstream missing;
    vector<Airport*> airports = mst.get_airports();
    for (unsigned int i = 0; i < airports.size(); i++) {
        if (airports[i] != NULL) {
            for (Route * route : airports[i] -> get_routes()) {
                cost += route -> get_weight();
            }
        } else {
            missing << i << " ";
        }
    }

    float coverage = 100.0 * (tree.size() + 1) / airports.size();
    cout << "Contains " << tree.size() + 1 << " airports (" << coverage
         << "% coverage)" << " at " << cost << " cost:" << endl;
    cout << "Missing airports: " << missing.str() << endl;

    ofstream file(filename);
    for (Route * route : tree) {
        file << route << endl;
    }
    cout << "Saved MST routes to " << filename << endl;
    file.close();
}

int main(int argc, char** argv) {
    Graph graph(true);
    graph.initialize();

    if (argc == 1) {
        bool query = true;
        while (query) {
            vector<int> airports;
            airports.push_back(get_airport(graph, "Enter Starting City"));

            cout << endl << "[1] Shortest-Path BFS, A* Search" << endl
                 << "[2] Largest Prim MST" << endl;
            int algorithm = std::stoi(get_input("Select Algorithm"));
            while (algorithm != 1 && algorithm != 2) {
                cout << "Invalid Algorithm";
                algorithm = std::stoi(get_input("Select Algorithm"));
            }
            cout << endl;

            if (algorithm == 1) {
                int dests = std::stoi(get_input("Enter Number of Destinations"));
                for (int dest = 1; dest <= dests; dest++) {
                    stringstream prompt;
                    prompt << "Enter Destination (" << dest << "/" << dests << ")";
                    airports.push_back(get_airport(graph, prompt.str()));
                }
                path_search(graph, airports);
            } else {
                prim_mst(graph, airports[0], "data/prim-mst-routes.txt");
            }

            cout << endl;
            query = (get_input("Requery? [Y/N]") == "y");
        }
    } else {
        if (argc == 2) {
            prim_mst(graph, std::stoi(argv[1]), "data/prim-mst-routes.txt");
        } else {
            vector<int> airports;
            for (int i = 1; i < argc; i++) {
                airports.push_back(std::stoi(argv[i]));
            }
            path_search(graph, airports);
        }
    }
}