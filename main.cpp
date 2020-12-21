#include <iostream>
#include <sstream>

#include "graph.h"

using namespace std;

string get_input(const string prompt) {
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
        cout << "[" << i + 1 << "] " << airports[city_airports[i]] -> get_name() 
            << " (" << airports[city_airports[i]] -> get_id() << ")" << endl;
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
        cout << "Incomplete Path Found" << endl;
    } else if (path.back() -> get_dest() -> get_id() != dest_id) {
        cout << "Incomplete Path Found:" << endl;
    } else {
        float cost = 0;
        for (Route * route : path) {
            cost += route -> get_weight();
        }
        cout << "Complete Path Found in " << path.size() 
            << " Route(s) at " << cost << " cost:" << endl;
    }

    for (Route * route : path) {
        Airport * src = route -> get_src();
        Airport * dest = route -> get_dest();
        cout << src -> get_name() << " (" << src -> get_id() << ")" << " --- " 
            << route -> get_label() << " [" << route -> get_weight() << "]" << " --> " 
            << dest -> get_name() << " (" << dest -> get_id() << ")" << endl;
    }
}

void print_mst(const Graph & mst, const vector<Route*> & tree) {
    float cost = 0;

    string missing = "";
    vector<Airport*> airports = mst.get_airports();
    for (unsigned int i = 0; i < airports.size(); i++) {
        if (airports[i] != NULL) {
             for (Route * route : airports[i] -> get_routes()) {
                cost += route -> get_weight();
            }
        } else {
            missing += std::to_string(i) + " ";
        }
    }

    float coverage = 100.0 * (tree.size() + 1) / mst.get_airports().size();
    cout << "Contains " << tree.size() + 1 << " airports (" << coverage 
            << "% coverage)" << " at " << cost << " cost:" << endl;
    
    
    for (Route * route : tree) {
        Airport * src = route -> get_src();
        Airport * dest = route -> get_dest();
        cout << src -> get_name() << " (" << src -> get_id() << ")" << " --- " 
            << route -> get_label() << " [" << route -> get_weight() << "]" << " --> " 
            << dest -> get_name() << " (" << dest -> get_id() << ")" << endl;
    }
    cout << endl << "Missing airports: " << missing << endl;
}

int main() {
    Graph graph(true);
    graph.initialize();

    bool query = true;
    while (query) {
        cout << endl;
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
            vector<Route*> bfs_path = graph.bfs(airports);
            print_path(bfs_path, airports.back());

            vector<Route*> a_path = graph.a_star_search(airports);
            print_path(a_path, airports.back());
        } else if (algorithm == 2) {
            Graph mst(false);
            vector<Route*> tree = graph.prim_mst(mst, airports[0]);
            print_mst(mst, tree);
        }

        cout << endl;
        query = (get_input("Requery? [Y/N]") == "y");
    }
}