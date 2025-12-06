#pragma once
#include "Triangulation.h"
#include "Knot.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <random>
#include <unordered_set>

struct EdgeWithWeight {
    Knot start;
    Knot end;
    float weight;

    EdgeWithWeight(Knot s, Knot e, float w) : start(s), end(e), weight(w) {}
};

class DisjointSetUnion {
private:
    std::unordered_map<Knot, Knot> parent;
    std::unordered_map<Knot, int> rank;

public:
    void makeSet(const Knot& knot);
    Knot find(const Knot& knot);
    void unite(const Knot& knot_1, const Knot& knot_2);
};

std::vector<EdgeWithWeight> calculateMST(const std::vector<Edge>& edges);
std::vector<Edge> addRandomEdges(
    const std::vector<Edge>& mst_edges,
    const std::vector<Edge>& all_edges,
    float probability
);
