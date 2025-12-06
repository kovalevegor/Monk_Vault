#include "Triangulation.h"
#include <algorithm>
#include <map>

//Bowyer-Watson Algorithm for Delaunay Triangulation https://www.gorillasun.de/blog/bowyer-watson-algorithm-for-delaunay-triangulation/#the-super-triangle

bool Triangle::operator == (const Triangle& another) const {
    std::set<Knot> this_knots = { knot_1, knot_2, knot_3 };
    std::set<Knot> another_knots = { another.knot_1, another.knot_2, another.knot_3 };
    return this_knots == another_knots;
}

bool operator<(const Edge& a, const Edge& b) {
    if (a.first == b.first) {
        return a.second < b.second;
    }
    return a.first < b.first;
}

bool operator==(const Edge& a, const Edge& b) {
    return (a.first == b.first && a.second == b.second) ||
        (a.first == b.second && a.second == b.first);
}

void printTriangle(Triangle& triangle) {
    std::cout << "\n(" << triangle.knot_1.x << "; " << triangle.knot_1.y << ") " <<
        "(" << triangle.knot_2.x << "; " << triangle.knot_2.y << ") " <<
        "(" << triangle.knot_3.x << "; " << triangle.knot_3.y << ")";
}

double determinant(const Knot& a, const Knot& b, const Knot& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool isKnotInCircumcircle(const Triangle& triangle, const Knot& knot) {
    Knot a = triangle.knot_1;
    Knot b = triangle.knot_2;
    Knot c = triangle.knot_3;

    float d = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    if (d == 0) return false;

    float ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
        ((b.x * b.x + b.y * b.y) * (c.y - a.y)) +
        ((c.x * c.x + c.y * c.y) * (a.y - b.y)));
    float uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
        ((b.x * b.x + b.y * b.y) * (a.x - c.x)) +
        ((c.x * c.x + c.y * c.y) * (b.x - a.x)));
    Knot center(ux / (2 * d), uy / (2 * d));

    float radius_sq = (center.x - a.x) * (center.x - a.x) +
        (center.y - a.y) * (center.y - a.y);
    float dist_sq = (knot.x - center.x) * (knot.x - center.x) +
        (knot.y - center.y) * (knot.y - center.y);

    return dist_sq <= radius_sq;
}

std::vector<Edge> getEdgesFromTriangles(const std::vector<Triangle>& triangles) {
    std::map<std::pair<Knot, Knot>, bool> edge_map;
    for (const auto& triangle : triangles) {
        Edge edges[3] = {
            {std::min(triangle.knot_1, triangle.knot_2), std::max(triangle.knot_1, triangle.knot_2)},
            {std::min(triangle.knot_1, triangle.knot_3), std::max(triangle.knot_1, triangle.knot_3)},
            {std::min(triangle.knot_2, triangle.knot_3), std::max(triangle.knot_2, triangle.knot_3)}
        };
        for (const auto& edge : edges) {
            auto key = std::make_pair(edge.first, edge.second);
            edge_map[key] = !edge_map[key]; 
        }
    }

    std::vector<Edge> edges;
    for (const auto& pair : edge_map) {
        if (pair.second) { 
            edges.emplace_back(pair.first.first, pair.first.second);
        }
    }
    return edges;
}

std::vector<Triangle> delaunayTriangulation(const std::vector<Knot>& knots) {
    std::vector<Triangle> triangles;

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& knot : knots) {
        if (knot.x < minX) minX = knot.x;
        if (knot.y < minY) minY = knot.y;
        if (knot.x > maxX) maxX = knot.x;
        if (knot.y > maxY) maxY = knot.y;
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midX = (minX + maxX) / 2.0f;
    float midY = (minY + maxY) / 2.0f;

    Triangle superTriangle = {
        {midX - 20.0f * deltaMax, midY - 10.0f * deltaMax},
        {midX, midY + 20.0f * deltaMax},
        {midX + 20.0f * deltaMax, midY - 10.0f * deltaMax}
    };
    triangles.push_back(superTriangle);

    for (const auto& knot : knots) {

        std::vector<Triangle> badTriangles;
        for (const auto& triangle : triangles) {
            if (isKnotInCircumcircle(triangle, knot)) {
                badTriangles.push_back(triangle);
            }
        }

        std::vector<Edge> polygon;

        for (const auto& triangle : badTriangles) {
            for (const auto& edge : getEdgesFromTriangles({ triangle })) {
                bool shared = false;
                for (const auto& otherTriangle : badTriangles) {
                    if (triangle == otherTriangle) continue;
                    for (const auto& otherEdge : getEdgesFromTriangles({ otherTriangle })) {
                        if (edge == otherEdge) {
                            shared = true;
                            break;
                        }
                    }
                    if (shared) break;
                }
                if (!shared) {
                    polygon.push_back(edge);
                }
            }
        }

        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
            [&badTriangles](const Triangle& t) {
                return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
            }), triangles.end());

        for (const auto& edge : polygon) {
            triangles.push_back({ edge.first, edge.second, knot });
        }
    }

    triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
        [&superTriangle](const Triangle& t) {
            return t.knot_1 == superTriangle.knot_1 || t.knot_1 == superTriangle.knot_2 || t.knot_1 == superTriangle.knot_3 ||
                t.knot_2 == superTriangle.knot_1 || t.knot_2 == superTriangle.knot_2 || t.knot_2 == superTriangle.knot_3 ||
                t.knot_3 == superTriangle.knot_1 || t.knot_3 == superTriangle.knot_2 || t.knot_3 == superTriangle.knot_3;
        }), triangles.end());

    return triangles;
}

std::vector<Edge> getUniqueEdges(const std::vector<Triangle>& triangles) {
    std::set<Edge> unique_edges;

    for (const auto& triangle : triangles) {
        Edge edges[3] = {
            {std::min(triangle.knot_1, triangle.knot_2),
             std::max(triangle.knot_1, triangle.knot_2)},
            {std::min(triangle.knot_1, triangle.knot_3),
             std::max(triangle.knot_1, triangle.knot_3)},
            {std::min(triangle.knot_2, triangle.knot_3),
             std::max(triangle.knot_2, triangle.knot_3)}
        };

        for (const auto& edge : edges) {
            unique_edges.insert(edge);
        }
    }

    return std::vector<Edge>(unique_edges.begin(), unique_edges.end());
}