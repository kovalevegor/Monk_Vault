#pragma once

#include <vector>
#include <iostream>
#include <set>
#include "Knot.h"

struct Triangle {
    Knot knot_1, knot_2, knot_3;

    bool operator == (const Triangle& another) const;
};

struct EdgeHash {
    size_t operator()(const Edge& e) const {
        auto hash1 = std::hash<float>()(e.first.x) ^ std::hash<float>()(e.first.y);
        auto hash2 = std::hash<float>()(e.second.x) ^ std::hash<float>()(e.second.y);
        return hash1 ^ (hash2 << 1);
    }
};

double determinant(const Knot& a, const Knot& b, const Knot& c);

bool isKnotInCircumcircle(const Triangle& triangle, const Knot& knot);

std::vector<Edge> getEdgesFromTriangles(const std::vector<Triangle>& triangles);

std::vector<Triangle> delaunayTriangulation(const std::vector<Knot>& knots);

std::vector<Edge> getUniqueEdges(const std::vector<Triangle>& triangles);