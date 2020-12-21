/**
 * @file airport.h
 * Definition of airport class.
 */ 

#pragma once

#include <string>
#include <vector>

#include "route.h"

using namespace std;

/**
 * Airport connected by routes represented as a graph node.
 * @author Vaibhav Gupta
 */ 
class Airport {
    public:
        /**
         * Constructor to create airport.
         * @param id Unique numerical identifier for airport.
         * @param name Name of airport.
         * @param city City in which airport is located in.
         */
        Airport(int id, string name, string city);

        // Destructor to delete airport.
        ~Airport();

        /**
         * Adds route to airport.
         * @param route Pointer to route which has this airport as 
         * its source airport.
         */ 
        void add_route(Route * route);

        /**
         * Returns all airport routes.
         * @returns Vector of pointers to routes of the airport.
         */ 
        vector<Route*> get_routes() const;

        /**
         * Returns ID of airport.
         * @returns ID of airport.
         */ 
        int get_id() const;

        /**
         * Returns name of airport.
         * @returns Name of airport.
         */
        string get_name() const;

        /**
         * Returns city of airport.
         * @returns City of airport.
         */ 
        string get_city() const;
        
    private:
        int id;
        string name, city;

        vector<Route*> routes;
};