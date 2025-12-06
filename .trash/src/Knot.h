#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

struct Knot {
    float x, y;
    Knot() : x(-1.0f), y(-1.0f) {}
    Knot(float _x, float _y) : x(_x), y(_y) {}

    bool operator==(const Knot& another) const;
    bool operator<(const Knot& another) const;
};

using Edge = std::pair<Knot, Knot>;
bool operator<(const Edge& a, const Edge& b);
bool operator==(const Edge& a, const Edge& b);

namespace std { 
    template<> struct hash<Knot> {
        size_t operator()(const Knot& k) const {
            return hash<float>()(k.x) ^ (hash<float>()(k.y) << 1);
        }
    };
}

void insertKnot(std::vector<std::vector<Knot>>& grid, const Knot& knot, float cellsize);

bool isKnotValid(const std::vector<std::vector<Knot>>& grid, int g_width, int g_height,
    const Knot& knot, float radius, float width, float height, float cellsize);

float distanceBetweenPoints(float x1, float y1, float x2, float y2);

std::vector<Knot> poissonDiskSampling(float width, float height, double radius, int limit);