/**
 * @file graph.h
 * Definition of graph class.
 */ 

#pragma once

#include <string>
#include <map>

#include "heap.h"
#include "airport.h"
#include "route.h"

using namespace std;

/**
 * Directed, weighted graph containing airports as nodes and 
 * routes as edges.
 * @author Vaibhav Gupta
 */
class Graph {
    public:
        /**
         * Constructor to create empty graph.
         * @param output Print output to console or not.
         */
        Graph(bool output);

        // Destructor to delete graph.
        ~Graph();

        // Initializes graph by reading airports and routes.
        void initialize();

        /**
         * Helper function to read CSV of airport data and 
         * add airports to graph.
         * @param filename File name to read.
         * @returns Number of airports added.
         */
        int read_airports(const string filename);

         /**
         * Helper function to read CSV of route data and 
         * add routes to graph.
         * @param filename File name to read.
         * @returns Number of routes added.
         */
        int read_routes(const string filename);

        /**
         * Performs Shortest-Path Breadth-First Search on graph 
         * from one airport to the next in sequential order.
         * 
         * Shortest path is defined as the path with the least number
         * of airports/routes, which is not guaranteed to exist.
         * @param dests Reference to vector of airport IDs to visit.
         * @returns Vector of routes creating shortest-path to most
         * reachable destination.
         */ 
        vector<Route*> bfs(const vector<int> & dests);

        /**
         * Performs Shortest-Path A* Search on graph from one
         * airport to the next in sequential order by generating a
         * shortest-path tree.
         * 
         * Shortest path is defined as the path with the lowest cost,
         * which is not guaranteed to exist.
         * @param dests Reference to vector of airport IDs to visit.
         * @returns Vector of routes creating shortest-path to most
         * reachable destination.
         */ 
        vector<Route*> a_star_search(const vector<int> & dests);

        /**
         * Performs Prim's Algorithm on graph to generate the largest
         * minimum spanning tree from a given starting airport.
         * 
         * Equivalent to the largest connected component which contains the 
         * lowest cost routes to all airports that are reachable by the 
         * starting airport.
         * @param mst Minimum spanning tree to populate.
         * @param start_id ID of starting airport.
         */ 
        vector<Route*> prim_mst(Graph & mst, int src_id);

        /**
         * Helper heuristic function used by A* Search to estimate cost of
         * path from given airport to destination.
         * 
         * Estimates cost based on lowest cost route from airport in order 
         * for heuristic to be admissible.
         * @param id ID of airport to calculate heuristic.
         * @returns Estimated cost from airport to destination.
         */ 
        float heuristic(int id);

        /**
         * Returns all airports located in a given city.
         * @param city Name of city to get all airports in.
         * @returns Vector of pointers to all airports in given city.
         */
        vector<int> get_airports_in_city(string city) const;

        /**
         * Helper function to insert airport in graph.
         * @param airport Pointer to airport to insert in graph.
         */
        void insert_airport(Airport * airport);

        /**
         * Helper function to insert route in graph.
         * @param route Pointer to route to insert in graph.
         */
        void insert_route(Route * route);

        /**
         * Returns all airports in the graph, used for testing.
         * @returns Vector of pointers to all airports.
         */
        vector<Airport*> get_airports() const;

    private:
        vector<Airport*> airports;
        bool output;
};