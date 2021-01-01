/**
 * @file route.cpp
 * Implementation of route class.
 */

#include <iostream>

#include "route.h"

Route::Route(Airport *src, Airport *dest, string label, double weight) {
    this -> src = src;
    this -> dest = dest;
    this -> label = label;
    this -> weight = weight;
}

Airport* Route::get_src() const {
    return src;
}

Airport* Route::get_dest() const {
    return dest;
}

string Route::get_label() const {
    return label;
}

double Route::get_weight() const {
    return weight;
}

std::ostream &operator<<(std::ostream &os, const Route *route) {
    return os << route -> get_src() << " --- " << route -> get_label() << " [" 
        << route -> get_weight() << "]" << " --> " << route -> get_dest(); 
}