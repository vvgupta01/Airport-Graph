/**
 * @file airport.cpp
 * Implementation of airport class.
 */

#include "airport.h"

Airport::Airport(int id, string name, string city) {
    this -> id = id;
    this -> name = name;
    this -> city = city;
}

Airport::~Airport() {
    for (Route * route : routes) {
        delete route;
    }
}

void Airport::add_route(Route * route) {
    routes.push_back(route);
}

vector<Route*> Airport::get_routes() const {
    return routes;
}

int Airport::get_id() const {
    return id;
}

string Airport::get_name() const {
    return name;
}

string Airport::get_city() const {
    return city;
}

std::ostream & operator<<(std::ostream & os, const Airport * airport) {
    return os << airport -> get_name() << " (" << airport -> get_id() << ")";
}