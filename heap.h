/**
 * @file heap.h
 * Definition of heap class.
 */ 

#pragma once

#include <vector>
#include "airport.h"

using namespace std;

/**
 * Minimum priority queue to select the airport ID with the 
 * lowest cost that has not been visited yet.
 * @author Vaibhav Gupta
 */ 
class Heap {
    public:
        /**
         * Constructor to create a heap of airport IDs.
         * @param num_airports Number of airports/IDs.
         * @param start_id Starting airport ID, initialized to 0 cost.
         */ 
        Heap(int num_airports, int start_id);

        /**
         * Removes and returns airport ID with the lowest cost.
         * @returns Airport ID with the lowest cost.
         */
        int pop();

        /**
         * Updates cost of airport and corrects heap.
         * @param id Airport ID to be updated.
         * @param cost New cost of airport.
         */
        void update(int id, float cost);

        /**
         * Helper function to correct heap by moving a
         * low cost node up the tree.
         * @param idx Node index in airport IDs.
         */ 
        void heapify_up(int idx);

         /**
         * Helper function to correct heap by moving a
         * high cost node down the tree.
         * @param idx Node index in airport IDs.
         */ 
        void heapify_down(int idx);

        /**
         * Returns the cost of a given airport.
         * @param id Airport ID to return cost of.
         * @returns Cost of airport.
         */
        float get_cost(int id) const;

        /**
         * Helper function to return the index of the child 
         * with the lowest cost of a given node.
         * @param idx Node index in airport IDs.
         * @returns Child index with the lowest cost.
         */
        int get_min_child(int idx) const;

        /**
         * Helper function to determine if a given node has a
         * child or not.
         * @param idx Node index in airport IDs.
         * @returns Boolean representing if node has a child or not.
         */
        bool has_child(int idx) const;

        /**
         * Helper function to return the index of the left 
         * child of a given node.
         * @param idx Node index in airport IDs.
         * @returns Node index of left child in airport IDs.
         */
        int get_lchild(int idx) const;

        /**
         * Helper function to return the index of the right 
         * child of a given node.
         * @param idx Node index in airport IDs.
         * @returns Node index of right child in airport IDs.
         */
        int get_rchild(int idx) const;

        /**
         * Helper function to return the parent of a given node.
         * @param idx Node index in airport IDs.
         * @returns Node index of parent in airport IDs.
         */
        int get_parent(int idx) const;

    private:
        vector<int> ids;
        vector<float> costs; 
};