#include <iostream>
#include <sstream>
#include <fstream>

#include "graph.h"

using namespace std;

string get_input(const string &prompt) {
    string input;
    cout << prompt << ": ";
    std::getline(cin, input);
    return input;
}

int get_airport(const Graph &graph, const string &prompt) {
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

void run_dijkstra(const Graph &graph, const vector<int> &dest_ids) {
    const string file_path = "data/path-routes.txt";

    vector<Route*> path = graph.dijkstra(dest_ids);
    if (path.empty()) {
        cout << "No Path Found" << endl;
    } else if (path.back() -> get_dest() -> get_id() != dest_ids.back()) {
        cout << "Partially Complete Path Found:" << endl;
    } else {
        double cost = 0;
        for (Route *route : path) {
            cost += route -> get_weight();
        }
        cout << "Complete Path Found in " << path.size() 
            << " Route(s) at " << cost << " Cost:" << endl;
    }

    ofstream file(file_path);
    for (Route *route : path) {
        cout << route << endl;
        file << route << endl;
    }
    file.close();
    cout << endl << "Saved path routes to " << file_path << endl;
}

void run_prim_mst(const Graph &graph, int start_id) {
    const string file_path = "data/mst-routes.txt";

    Graph mst;
    vector<Route*> tree = graph.prim_mst(mst, start_id);

    double cost = 0;
    stringstream missing;
    vector<Airport*> airports = mst.get_airports();
    for (unsigned int i = 0; i < airports.size(); i++) {
        if (airports[i] != NULL) {
            for (Route *route : airports[i] -> get_routes()) {
                cost += route -> get_weight();
            }
        } else {
            missing << i << " ";
        }
    }

    float coverage = 100.0 * (tree.size() + 1) / airports.size();
    cout << "Contains " << tree.size() + 1 << " Airports (" << coverage 
            << "% Coverage)" << " at " << cost << " Cost" << endl;
    cout << "Missing Airports: " << missing.str() << endl;

    ofstream file(file_path);
    for (Route *route : tree) {
        file << route << endl;
    }
    file.close();
    cout << endl << "Saved MST routes to " << file_path << endl;
}

void run_floyd_warshall(const Graph &graph) {
    const string file_path = "data/airport-centralities.txt";
    vector<double> scores = graph.floyd_warshall();
    vector<Airport*> airports = graph.get_airports();

    int min = -1, max = -1;
    int zero_centrality = 0;
    double total_centrality = 0;
    for (unsigned long i = 0; i < airports.size(); i++) {
        total_centrality += scores[i];
        if (scores[i] == 0) {
            zero_centrality++;
        } else if (min == -1 || scores[i] < scores[min]) {
            min = i;
        } else if (max == -1 || scores[i] > scores[max]) {
            max = i;
        }
    }
    double avg_centrality = total_centrality / airports.size();

    cout << endl;
    cout << "Minimum Centrality: " << airports[min] << " = " << scores[min] << endl;
    cout << "Maximum Centrality: " << airports[max] << " = " << scores[max] << endl;
    cout << "Average Centrality: " << avg_centrality << endl;
    cout << "Number of Zero Centrality Airports: " << zero_centrality << endl;

    ofstream file(file_path);
    for (unsigned long i = 0; i < airports.size(); i++) {
        file << airports[i] << " = " << scores[i] << endl;
    }
    file.close();
    cout << endl << "Saved airport centralities to " << file_path << endl;
}

int main(int argc, char** argv) {
    Graph graph;
    graph.initialize();

    if (argc == 1) {
        bool query = true;
        while (query) {
            cout << endl << "[1] Dijkstra's Algorithm (Shortest-Path)" << endl 
                << "[2] Prim's Algorithm (Largest MST)" << endl 
                << "[3] Floyd-Warshall Algorithm (Betweenness Centrality)" << endl;
            int algorithm = std::stoi(get_input("Select Algorithm"));
            while (algorithm < 1 || algorithm > 3) {
                cout << "Invalid Algorithm" << endl;
                algorithm = std::stoi(get_input("Select Algorithm"));
            }

            if (algorithm == 1) {
                vector<int> airports;
                airports.push_back(get_airport(graph, "Enter Starting City"));

                int dests = std::stoi(get_input("Enter Number of Destinations"));
                for (int dest = 1; dest <= dests; dest++) {
                    stringstream prompt;
                    prompt << "Enter Destination City (" << dest << "/" << dests << ")";
                    airports.push_back(get_airport(graph, prompt.str()));
                }
                run_dijkstra(graph, airports);
            } else if (algorithm == 2) {
                int start_id = get_airport(graph, "Enter Starting City");
                run_prim_mst(graph, start_id);
            } else {
                run_floyd_warshall(graph);
            }

            cout << endl;
            query = (get_input("Requery? [Y/N]") == "y");
        }
    } else if (argc == 2) {
        run_prim_mst(graph, std::stoi(argv[1]));
    } else {
        vector<int> airports;
        for (int i = 1; i < argc; i++) {
            airports.push_back(std::stoi(argv[i]));
        }
        run_dijkstra(graph, airports);
    }
}