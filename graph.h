/**
 * @file graph.h
 * Definition of graph class.
 */ 

#pragma once

#include <string>

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

        // Destructor to delete graph.
        ~Graph();

        // Initializes graph by reading airports and routes.
        void initialize();

        /**
         * Helper function to read CSV of airport data and 
         * add airports to graph.
         * @param file_path File path to read.
         * @returns Number of airports added.
         */
        int read_airports(const string &file_path);

         /**
         * Helper function to read CSV of route data and 
         * add routes to graph.
         * @param filename File name to read.
         * @returns Number of routes added.
         */
        int read_routes(const string &file_path);

        /**
         * Performs Dijkstra's Algorithm for Single-Source Shortest-Path on
         * graph from one airport to the next in sequential order by
         * generating a shortest-path tree.
         * 
         * Shortest path is defined as the path with the lowest cost,
         * which is not guaranteed to exist.
         * @param dest_ids Reference to vector of airport IDs to visit.
         * @returns Vector of routes creating shortest-path to most
         * reachable destination.
         */ 
        vector<Route*> dijkstra(const vector<int> &dest_ids) const;

        /**
         * Performs Prim's Algorithm on graph to generate the largest
         * minimum spanning tree from a given starting airport.
         * 
         * Equivalent to the largest connected component which contains the 
         * lowest cost routes to all airports that are reachable by the 
         * starting airport.
         * @param mst Minimum spanning tree to populate.
         * @param src_id ID of starting airport.
         */ 
        vector<Route*> prim_mst(Graph &mst, int src_id) const;

        /**
         * Performs Floyd-Warshall's algorithm to calculate the betweenness
         * centrality of every airport by finding the shortest path between
         * every pair of airports in the graph.
         *
         * Betweenness centrality is defined as the number of shortest paths
         * through a given airport divided by the total number of shortest paths
         * that do not include the airport as a source or destination.
         * @returns Vector representing the betweenness centrality of each airport.
         */
        vector<double> floyd_warshall() const;

        /**
         * Returns all airports located in a given city.
         * @param city Name of city to get all airports in.
         * @returns Vector of pointers to all airports in given city.
         */
        vector<int> get_airports_in_city(string &city) const;

        /**
         * Helper function to insert airport in graph.
         * @param airport Pointer to airport to insert in graph.
         */
        void insert_airport(Airport *airport);

        /**
         * Helper function to insert route in graph.
         * @param route Pointer to route to insert in graph.
         */
        void insert_route(Route *route);

        /**
         * Returns all airports in the graph, used for testing.
         * @returns Vector of pointers to all airports.
         */
        vector<Airport*> get_airports() const;

    private:
        vector<Airport*> airports;
};