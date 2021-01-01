/**
 * @file graph.cpp
 * Implementation of graph class.
 */

#include "graph.h"

#include <map>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>

Graph::~Graph() {
    for (Airport *airport : airports) {
        delete airport;
    }
}

void Graph::initialize() {
    auto start = chrono::steady_clock::now();
    int num_airports = read_airports("data/airports.csv");
    int num_routes = read_routes("data/routes.csv");
    auto end = chrono::steady_clock::now();

    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Generated graph in " << time << "ms" << endl;
    cout << "Loaded " << num_airports << " nodes, " << num_routes 
        << " edges" << endl << flush;
}

int Graph::read_airports(const string &file_path) {
    string name, city;
    ifstream file(file_path);

    int airport_id = 0;
    while (std::getline(file, name, ',')) {
        name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
        std::getline(file, city, ',');

        insert_airport(new Airport(airport_id, name, city));
        airport_id++;
    }
    file.close();
    return airport_id;
}

int Graph::read_routes(const string &file_path) {
    string src_id, dest_id, label, weight;
    ifstream file(file_path);

    int routes = 0;
    while (std::getline(file, src_id, ',')) {
        std::getline(file, dest_id, ',');
        std::getline(file, label, ',');
        std::getline(file, weight, ',');

        Airport * src = airports[std::stoi(src_id)];
        Airport * dest = airports[std::stoi(dest_id)];
        insert_route(new Route(src, dest, label, std::stod(weight)));
        routes++;
    }
    file.close();
    return routes;
}

vector<Route*> Graph::dijkstra(const vector<int> &dest_ids) const {
    auto start = chrono::steady_clock::now();

    vector<Route*> path;
    for (unsigned long i = 0; i < dest_ids.size() - 1; i++) {
        int src_id = dest_ids[i], dest_id = dest_ids[i + 1];

        Heap heap(airports.size(), src_id);
        vector<Route*> predecessor(airports.size(), NULL);

        Graph spt;
        spt.airports.resize(airports.size(), NULL);
        
        int current = -1;
        while (current != dest_id) {
            current = heap.pop();
            if (heap.get_cost(current) == INT_MAX) {
                break;
            }

            spt.airports[current] = new Airport(current, airports[current] -> get_name(), 
                airports[current] -> get_city());
            
            Route *pred = predecessor[current];
            if (pred != NULL) {
                Route *route_copy = new Route(spt.airports[pred -> get_src() -> get_id()], 
                    spt.airports[current], pred -> get_label(), pred -> get_weight());
                spt.insert_route(route_copy);
            }

            for (Route *route : airports[current] -> get_routes()) {
                int dest_id = route -> get_dest() -> get_id();
                if (spt.airports[dest_id] == NULL) {
                    double curr_cost = heap.get_cost(current) + route -> get_weight();
                    if (curr_cost < heap.get_cost(dest_id)) {
                        heap.update(dest_id, curr_cost);
                        predecessor[dest_id] = route;
                    }
                }
            }
        }

        if (current != dest_id) {
            break;
        }

        vector<Route*> subpath;
        while (current != src_id) {
            subpath.push_back(predecessor[current]);
            current = predecessor[current] -> get_src() -> get_id();
        }
        std::reverse(subpath.begin(), subpath.end());
        path.insert(path.end(), subpath.begin(), subpath.end());
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Completed Dijkstra's Algorithm in " << time << "ms" << endl;
    return path;
}

vector<Route*> Graph::prim_mst(Graph &mst, int src_id) const {
    auto start = chrono::steady_clock::now();

    Heap heap(airports.size(), src_id);
    vector<Route*> predecessor(airports.size(), NULL);
    vector<Route*> tree;

    mst.airports.resize(airports.size(), NULL);

    for (unsigned long i = 0; i < airports.size(); i++) {
        int current = heap.pop();
        if (heap.get_cost(current) == INT_MAX) {
            break;
        }

        mst.airports[current] = new Airport(current, airports[current] -> get_name(), 
            airports[current] -> get_city());
        
        Route *pred = predecessor[current];
        if (pred != NULL) {
            Route *route_copy = new Route(mst.airports[pred -> get_src() -> get_id()], 
                mst.airports[current], pred -> get_label(), pred -> get_weight());
            mst.insert_route(route_copy);
            tree.push_back(route_copy);
        }

        for (Route *route : airports[current] -> get_routes()) {
            Airport *dest = route -> get_dest();
            if (mst.airports[dest -> get_id()] == NULL) {
                if (route -> get_weight() < heap.get_cost(dest -> get_id())) {
                    heap.update(dest -> get_id(), route -> get_weight());
                    predecessor[dest -> get_id()] = route;
                }
            }
        }
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Completed Prim's Algorithm in " << time << "ms" << endl;
    return tree;
}

vector<double> Graph::floyd_warshall() const {
    auto start = chrono::steady_clock::now();

    vector<vector<double>> adj_matrix(airports.size(), 
        vector<double>(airports.size(), INT_MAX));
    vector<vector<int>> successor(airports.size(), 
        vector<int>(airports.size(), -1));
    
    for (unsigned long i = 0; i < airports.size(); i++) {
        adj_matrix[i][i] = 0;
        successor[i][i] = i;
        for (Route *route : airports[i] -> get_routes()) {
            int dest_id = route -> get_dest() -> get_id();
            adj_matrix[i][dest_id] = route -> get_weight();
            successor[i][dest_id] = dest_id;
        }
    }

    for (unsigned long i = 0; i < airports.size(); i++) {
        for (unsigned long j = 0; j < airports.size(); j++) {
            if (i != j && adj_matrix[j][i] < INT_MAX) {
                for (unsigned long k = 0; k < airports.size(); k++) {
                    if (j != k && k != i && adj_matrix[i][k] < INT_MAX) {
                        double new_cost = adj_matrix[j][i] + adj_matrix[i][k];
                        if (new_cost < adj_matrix[j][k]) {
                            adj_matrix[j][k] = new_cost;
                            successor[j][k] = i;
                        }
                    }
                }
            } 
        }
    }

    int unique_paths = 0;
    for (unsigned long i = 0; i < airports.size(); i++) {
        for (unsigned long j = 0; j < airports.size(); j++) {
            if (i != j && adj_matrix[i][j] < INT_MAX) {
                unique_paths++;
            }
        }
    }

    int total_paths = 0;
    vector<double> centralities(airports.size());
    for (unsigned long i = 0; i < airports.size(); i++) {
        int paths = 0, possible_paths = 0;
        for (unsigned long j = 0; j < airports.size(); j++) {
            for (unsigned long k = 0; k < airports.size(); k++) {
                if (j != i && k != i && j != k && adj_matrix[j][k] < INT_MAX) {
                    possible_paths++;
                    unsigned long current = j;
                    while (current != k) {
                        current = successor[current][k];
                        if (current == i) {
                            paths++;
                            break;
                        }
                    }
                }
            }
        }
        total_paths += paths;
        centralities[i] = (double)paths / possible_paths;
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Completed Floyd-Warshall's Algorithm in " << time << "ms" << endl;
    cout << "Unique Shortest-Paths: " << unique_paths << endl;
    cout << "Average Shortest-Paths Per Airport: " << total_paths / airports.size() << endl;
    return centralities;
}

vector<int> Graph::get_airports_in_city(string &city) const {
    vector<int> city_airports;

    transform(city.begin(), city.end(), city.begin(), ::toupper);
    for (unsigned long i = 0; i < airports.size(); i++) {
        string ap_city = airports[i] -> get_city();
        transform(ap_city.begin(), ap_city.end(), ap_city.begin(), ::toupper);

        if (ap_city == city) {
            city_airports.push_back(i);
        }
    }
    return city_airports;
}

void Graph::insert_airport(Airport *airport) {
    airports.push_back(airport);
}

void Graph::insert_route(Route *route) {
    route -> get_src() -> add_route(route);
}

vector<Airport*> Graph::get_airports() const {
    return airports;
}