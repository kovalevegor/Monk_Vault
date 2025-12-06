#include "MinimumSpanningTree.h"
#include "Knot.h"
#include <cmath>

// DisjointSetUnion (DSU)
void DisjointSetUnion::makeSet(const Knot& k) {
    parent[k] = k;
    rank[k] = 0;
}

Knot DisjointSetUnion::find(const Knot& k) {
    if (parent[k] == k) return k;
    return parent[k] = find(parent[k]);
}

void DisjointSetUnion::unite(const Knot& a, const Knot& b) {
    Knot rootA = find(a);
    Knot rootB = find(b);
    if (rootA == rootB) return;

    if (rank[rootA] < rank[rootB]) {
        parent[rootA] = rootB;
    }
    else {
        parent[rootB] = rootA;
        if (rank[rootA] == rank[rootB]) rank[rootA]++;
    }
}

std::vector<EdgeWithWeight> calculateMST(const std::vector<Edge>& edges) {
    std::vector<EdgeWithWeight> weightedEdges;
    DisjointSetUnion dsu;

    for (const auto& edge : edges) { // 1.
        float dx = edge.second.x - edge.first.x;
        float dy = edge.second.y - edge.first.y;
        float weight = std::sqrt(dx * dx + dy * dy);
        weightedEdges.emplace_back(edge.first, edge.second, weight);
    }

    std::sort(weightedEdges.begin(), weightedEdges.end(), // 2.
        [](const EdgeWithWeight& a, const EdgeWithWeight& b) {
            return a.weight < b.weight;
        });

    for (const auto& edge : weightedEdges) {// 3.
        dsu.makeSet(edge.start);
        dsu.makeSet(edge.end);
    }

    std::vector<EdgeWithWeight> mst; // 4. 
    for (const auto& edge : weightedEdges) {
        if (dsu.find(edge.start) != dsu.find(edge.end)) {
            mst.push_back(edge);
            dsu.unite(edge.start, edge.end);
        }
    }

    return mst;
}

std::vector<Edge> addRandomEdges(
    const std::vector<Edge>& mst_edges,
    const std::vector<Edge>& all_edges,
    float probability
) {
    std::vector<Edge> result = mst_edges;

    if (all_edges.empty()) {
        return result;
    }

    float total_length = 0.0f;
    for (const auto& edge : all_edges) {
        float dx = edge.second.x - edge.first.x;
        float dy = edge.second.y - edge.first.y;
        total_length += std::sqrt(dx * dx + dy * dy);
    }
    float average_length = total_length / all_edges.size();
    float max_allowed_length = average_length * 1.20f; // +20%

    std::unordered_set<Edge, EdgeHash> mst_set(mst_edges.begin(), mst_edges.end());
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (const auto& edge : all_edges) {
        if (mst_set.find(edge) != mst_set.end()) {
            continue;
        }

        if (dist(gen) < probability) {
            float dx = edge.second.x - edge.first.x;
            float dy = edge.second.y - edge.first.y;
            float length = std::sqrt(dx * dx + dy * dy);

            if (length <= max_allowed_length) {
                result.push_back(edge);
            }
        }
    }

    return result;
}