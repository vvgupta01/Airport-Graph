/**
 * @file route.cpp
 * Implementation of route class.
 */

#include "route.h"

Route::Route(Airport * src, Airport * dest, string label, float weight) {
    this -> src = src;
    this -> dest = dest;
    this -> label = label;
    this -> weight = weight;
}

Airport * Route::get_src() const {
    return src;
}

Airport * Route::get_dest() const {
    return dest;
}

string Route::get_label() const {
    return label;
}

float Route::get_weight() const {
    return weight;
}