#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../graph.h"

void test_path(vector<int> dests, vector<int> bfs_sol, vector<int> a_sol, vector<string> labels) {
    Graph graph(false);
    graph.initialize();

    vector<Route*> bfs_path = graph.bfs(dests);
    vector<Route*> a_path = graph.a_star_search(dests);

    if (bfs_sol.empty()) {
        REQUIRE(bfs_path.empty());
        REQUIRE(a_path.empty());
    } else {
        for (unsigned int i = 0; i < bfs_path.size(); i++) {
            REQUIRE(bfs_path[i] -> get_src() -> get_id() == bfs_sol[i]);
            REQUIRE(bfs_path[i] -> get_dest() -> get_id() == bfs_sol[i + 1]);
        }

        for (unsigned int i = 0; i < a_path.size(); i++) {
            REQUIRE(a_path[i] -> get_src() -> get_id() == a_sol[i]);
            REQUIRE(a_path[i] -> get_dest() -> get_id() == a_sol[i + 1]);
            REQUIRE(a_path[i] -> get_label() == labels[i]);
        }
    }
}

void test_mst(int src_id, vector<int> solution, int threshold) {
    Graph graph(false);
    graph.initialize();

    Graph mst(false);
    vector<Route*> routes = graph.prim_mst(mst, src_id);

    float cost = 0;
    for (Route * route : routes) {
        cost += route -> get_weight();
    }
    REQUIRE(cost <= threshold);

    if (solution.size() == 0) {
        REQUIRE(routes.size() == 0);
    } else {
        vector<int> actual;
        for (Airport * airport : mst.get_airports()) {
            if (airport != NULL) {
                actual.push_back(airport -> get_id());   
            }
        }

        for (unsigned int i = 0; i < actual.size(); i++) {
            REQUIRE(actual[i] == solution[i]);
        }
    }
}

TEST_CASE("Test direct flight to single destination", "[weight=1]") {
    vector<int> dests = {200, 31};
    vector<string> labels = {"American Airlines"};
    test_path(dests, dests, dests, labels);
}

TEST_CASE("Test short route to single destination", "[weight=1]") {
    vector<int> dests = {70, 536};
    vector<int> solution = {70, 229, 411, 536};
    vector<string> labels = {"Delta Air Lines", "Allegiant Air", "Allegiant Air"};
    test_path(dests, solution, solution, labels);
}

TEST_CASE("Test long route to single destination", "[weight=1]") {
    vector<int> dests = {518, 315};
    vector<int> solution = {518, 81, 176, 200, 144, 315};
    vector<string> labels = {"Era Alaska", "Alaska Airlines", "United Airlines", 
        "United Airlines", "United Airlines"};
    test_path(dests, solution, solution, labels);
}

TEST_CASE("Test direct flights to multiple destinations", "[weight=1]") {
    vector<int> dests = {185, 75, 124, 54, 28};
    vector<string> labels = {"American Airlines", "Delta Air Lines", 
        "Delta Air Lines", "Delta Air Lines"};
    test_path(dests, dests, dests, labels);
}

TEST_CASE("Test short route to multiple destinations", "[weight=1]") {
    vector<int> dests = {72, 322, 309};
    vector<int> solution = {72, 124, 322, 124, 54, 309};
    vector<string> labels = {"Delta Air Lines", "Silver Airways (3M)", "Silver Airways (3M)", 
        "Delta Air Lines", "Delta Air Lines"};
    test_path(dests, solution, solution, labels);
}

TEST_CASE("Test long route to multiple destinations", "[weight=1]") {
    vector<int> dests = {254, 471, 531};
    vector<int> bfs_sol = {254, 229, 176, 403, 471, 403, 176, 200, 252, 531};
    vector<int> a_sol = {254, 168, 176, 403, 471, 403, 176, 200, 252, 531};
    vector<string> labels = {"United Airlines", "United Airlines", "Alaska Airlines", 
        "Era Alaska", "Era Alaska", "Alaska Airlines", "United Airlines", "American Airlines", 
        "Air Sureste"};
    test_path(dests, bfs_sol, a_sol, labels);
}

TEST_CASE("Test short round trip", "[weight=1]") {
    vector<int> dests = {165, 532, 165};
    vector<int> bfs_sol = {165, 122, 532, 122, 165};
    vector<int> a_sol = {165, 124, 532, 124, 165};
    vector<string> labels = {"Delta Air Lines", "Delta Air Lines", "Delta Air Lines", 
        "Delta Air Lines"};
    test_path(dests, bfs_sol, a_sol, labels);
}

TEST_CASE("Test long round trip", "[weight=1]") {
    vector<int> dests = {44, 533, 44};
    vector<int> solution = {44, 31, 176, 81, 533, 534, 420, 81, 176, 31, 44};
    vector<string> labels = {"American Airlines", "American Airlines", "Alaska Airlines", 
        "Era Alaska", "Era Alaska", "Era Alaska", "Era Alaska", "Alaska Airlines", 
        "American Airlines", "American Airlines"};
    test_path(dests, solution, solution, labels);
}

TEST_CASE("Test fully nonexistent route", "[weight=1]") {
    vector<int> dests = {523, 1};
    test_path(dests, vector<int>(), vector<int>(), vector<string>());
}

TEST_CASE("Test partially nonexistent route", "[weight=1]") {
    vector<int> dests = {343, 203, 9};
    vector<int> solution = {343, 32, 200, 203};
    vector<string> labels = {"United Airlines", "American Airlines", "American Airlines"};
    test_path(dests, solution, solution, labels);
}

TEST_CASE("Test minimum spanning trees", "[weight=3]") {
    vector<int> missing = {1, 9, 152, 218, 352, 408, 429, 431, 467, 468, 488, 493, 515, 516, 528};

    vector<int> solution;
    for (unsigned int i = 0; i < 539; i++) {
        if (std::find(missing.begin(), missing.end(), i) == missing.end()) {
            solution.push_back(i);
        }
    }

    test_mst(314, solution, 5200);
    test_mst(445, solution, 5200);
    test_mst(105, solution, 5200);
}

TEST_CASE("Test small minimum spanning tree", "[weight=1]") {
    vector<int> solution = {218, 408, 515, 516};
    test_mst(218, solution, 76);
}

TEST_CASE("Test nonexistent minimum spanning tree", "[weight=1]") {
    test_mst(494, vector<int>(), 0);
}