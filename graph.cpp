/**
 * @file graph.cpp
 * Implementation of graph class.
 */

#include "graph.h"

#include <queue>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>

Graph::Graph(bool output) {
    this -> output = output;
}

Graph::~Graph() {
    for (Airport * airport : airports) {
        delete airport;
    }
}

void Graph::initialize() {
    auto start = chrono::steady_clock::now();
    int num_airports = read_airports("data/airports.csv");
    int num_routes = read_routes("data/routes.csv");
    auto end = chrono::steady_clock::now();

    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (output) {
        cout << "Generated graph in " << time << "ms" << endl;
        cout << "Loaded " << num_airports << " airports, " << num_routes 
            << " routes" << endl;
    }
}

int Graph::read_airports(const string filename) {
    string name, city;
    ifstream file(filename);

    int airport_id = 0;
    while (std::getline(file, name, ',')) {
        name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
        std::getline(file, city, ',');

        insert_airport(new Airport(airport_id, name, city));
        airport_id++;
    }
    return airport_id;
}

int Graph::read_routes(const string filename) {
    string src_id, dest_id, label, weight;
    ifstream file(filename);

    int routes = 0;
    while (std::getline(file, src_id, ',')) {
        std::getline(file, dest_id, ',');
        std::getline(file, label, ',');
        std::getline(file, weight, ',');

        Airport * src = airports[std::stoi(src_id)];
        Airport * dest = airports[std::stoi(dest_id)];
        insert_route(new Route(src, dest, label, std::stof(weight)));
        routes++;
    }
    return routes;
}

vector<Route*> Graph::bfs(const vector<int> & dests) {
    auto start = chrono::steady_clock::now();

    vector<Route*> path;
    for (unsigned long i = 0; i < dests.size() - 1; i++) {
        Airport * src = airports[dests[i]], *dest = airports[dests[i + 1]];

        queue<Airport*> bfs_queue;
        vector<Route*> predecessor(airports.size(), NULL);

        bfs_queue.push(src);
        Airport * current = src;
        while (current != dest && !bfs_queue.empty()) {
            current = bfs_queue.front();
            bfs_queue.pop();

            for (Route * route : current -> get_routes()) {
                Airport * dest = route -> get_dest();
                if (predecessor[dest -> get_id()] == NULL) {
                    bfs_queue.push(dest);
                    predecessor[dest -> get_id()] = route;
                }
            }
        }

        if (current != dest) {
            break;
        }

        vector<Route*> subpath;
        while (current != src) {
            subpath.push_back(predecessor[current -> get_id()]);
            current = predecessor[current -> get_id()] -> get_src();
        }
        std::reverse(subpath.begin(), subpath.end());
        path.insert(path.end(), subpath.begin(), subpath.end());
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (output) {
        cout << endl << "Completed Shortest-Path BFS in " << time << "ms" << endl;
    }
    return path;
}

vector<Route*> Graph::a_star_search(const vector<int> & dests) {
    auto start = chrono::steady_clock::now();

    vector<Route*> path;
    for (unsigned long i = 0; i < dests.size() - 1; i++) {
        Airport * src = airports[dests[i]], *dest = airports[dests[i + 1]];

        Heap heap(airports.size(), src -> get_id());
        vector<Route*> predecessor(airports.size(), NULL);

        Graph spt(false);
        spt.airports.resize(airports.size(), NULL);
        
        int current = -1;
        for (unsigned long i = 0; i < airports.size() && current != dest -> get_id(); i++) {
            current = heap.pop();
            if (heap.get_cost(current) == 1000) {
                break;
            }

            spt.airports[current] = new Airport(current, airports[current] -> get_name(), 
                airports[current] -> get_city());
            
            Route * pred = predecessor[current];
            if (pred != NULL) {
                Route * route_copy = new Route(spt.airports[pred -> get_src() -> get_id()], 
                    spt.airports[current], pred -> get_label(), pred -> get_weight());
                spt.insert_route(route_copy);
            }

            for (Route * route : airports[current] -> get_routes()) {
                Airport * dest = route -> get_dest();
                if (spt.airports[dest -> get_id()] == NULL) {
                    float curr_cost = heap.get_cost(current) + route -> get_weight() + 
                        heuristic(dest -> get_id());
                    if (curr_cost < heap.get_cost(dest -> get_id())) {
                        heap.update(dest -> get_id(), curr_cost);
                        predecessor[dest -> get_id()] = route;
                    }
                }
            }
        }

        if (current != dest -> get_id()) {
            break;
        }

        vector<Route*> subpath;
        while (current != src -> get_id()) {
            subpath.push_back(predecessor[current]);
            current = predecessor[current] -> get_src() -> get_id();
        }
        std::reverse(subpath.begin(), subpath.end());
        path.insert(path.end(), subpath.begin(), subpath.end());
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (output) {
        cout << endl << "Completed Shortest-Path A* Search in " << time << "ms" << endl;
    }
    return path;
}

vector<Route*> Graph::prim_mst(Graph & mst, int src_id) {
    auto start = chrono::steady_clock::now();

    Heap heap(airports.size(), src_id);
    vector<Route*> predecessor(airports.size(), NULL);
    vector<Route*> tree;

    mst.airports.resize(airports.size(), NULL);

    int current = -1;
    for (unsigned long i = 0; i < airports.size(); i++) {
        current = heap.pop();
        if (heap.get_cost(current) == 1000) {
            break;
        }

        mst.airports[current] = new Airport(current, airports[current] -> get_name(), 
            airports[current] -> get_city());
        
        Route * pred = predecessor[current];
        if (pred != NULL) {
            Route * route_copy = new Route(mst.airports[pred -> get_src() -> get_id()], 
                mst.airports[current], pred -> get_label(), pred -> get_weight());
            mst.insert_route(route_copy);
            tree.push_back(route_copy);
        }

        for (Route * route : airports[current] -> get_routes()) {
            Airport * dest = route -> get_dest();
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
    if (output) {
        cout << "Generated Largest Prim MST in " << time << "ms" << endl;
    }
    return tree;
}

float Graph::heuristic(int id) {
    float min_weight = 1000;
    for (Route * route : airports[id] -> get_routes()) {
        if (route -> get_weight() < min_weight) {
            min_weight = route -> get_weight();
        }
    }
    return min_weight;
}

vector<int> Graph::get_airports_in_city(string city) const {
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

void Graph::insert_airport(Airport * airport) {
    airports.push_back(airport);
}

void Graph::insert_route(Route * route) {
    route -> get_src() -> add_route(route);
}

vector<Airport*> Graph::get_airports() const {
    return airports;
}