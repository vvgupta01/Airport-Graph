/**
 * @file route.h
 * Definition of route class.
 */ 

#pragma once

#include <string>
#include <map>

using namespace std;

class Airport;

/**
 * Route between two airports represented as a directed, 
 * weighted graph edge.
 * @author Vaibhav Gupta
 */
class Route {
    public:
        /** 
        * Constructor to create route based on flight.
        * @param src Pointer to source airport flight departs from.
        * @param dest Pointer to destination airport flight arrives to.
        * @param label Name of flight airline.
        * @param weight Cost of traveling from source airport to destination 
        * airport through airline.
        */
        Route(Airport * src, Airport * dest, string label, float weight);

        /**
         * Returns pointer to source airport.
         * @returns Pointer to source airport.
         */
        Airport * get_src() const;

        /**
         * Returns pointer to destination airport.
         * @returns Pointer to destination airport.
         */
        Airport * get_dest() const;

        /**
         * Returns label of route.
         * @returns Label of route.
         */
        string get_label() const;

        /**
         * Returns cost of route.
         * @returns Cost of route.
         */
        float get_weight() const;
    
    private:
        Airport * src, * dest;
        string label;
        float weight;
};