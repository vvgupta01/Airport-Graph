/**
 * @file heap.cpp
 * Implementation of heap class.
 */

#include "heap.h"
#include "route.h"

Heap::Heap(int num_airports, int start_id) {
    for (int i = 0; i < num_airports; i++) {
        ids.push_back(i);
    }
    std::swap(ids[0], ids[start_id]);

    costs.resize(num_airports, INT_MAX);
    costs[start_id] = 0;
}

int Heap::pop() {
    int id = ids[0];
    ids[0] = ids.back();
    ids.pop_back();

    heapify_down(0);
    return id;
}

void Heap::update(int id, float cost) {
    costs[id] = cost;
    for (unsigned long i = 0; i < ids.size(); i++) {
        if (ids[i] == id) {
            heapify_up(i);
        }
    }
}

void Heap::heapify_up(int idx) {
    if (idx > 0) {
        int parent_idx = get_parent(idx);
        if (costs[ids[idx]] < costs[ids[parent_idx]]) {
            std::swap(ids[idx], ids[parent_idx]);
            heapify_up(parent_idx);
        }
    }
}

void Heap::heapify_down(int idx) {
    if (has_child(idx)) {
        int child_idx = get_min_child(idx);
        if (costs[ids[idx]] > costs[ids[child_idx]]) {
            std::swap(ids[idx], ids[child_idx]);
            heapify_down(child_idx);
        }
    }
}

double Heap::get_cost(int id) const {
    return costs[id];
}

int Heap::get_min_child(int idx) const {
    unsigned long lc_idx = get_lchild(idx);
    unsigned long rc_idx = get_rchild(idx);

    if (lc_idx >= ids.size()) {
        return rc_idx;
    } else if (rc_idx >= ids.size()) {
        return lc_idx;
    }

    if (costs[ids[lc_idx]] < costs[ids[rc_idx]]) {
        return lc_idx;
    }
    return rc_idx;
}

bool Heap::has_child(int idx) const {
    return get_lchild(idx) < ids.size();
}

unsigned long Heap::get_lchild(int idx) const {
    return 2 * idx + 1;
}

unsigned long Heap::get_rchild(int idx) const {
    return 2 * idx + 2;
}

unsigned long Heap::get_parent(int idx) const {
    return (idx - 1) / 2;
}
