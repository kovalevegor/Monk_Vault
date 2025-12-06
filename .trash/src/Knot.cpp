#include "Knot.h"
#include <iostream>
#include <random>
#include <chrono>

bool Knot::operator==(const Knot& another) const {
    return x == another.x && y == another.y;
}

bool Knot::operator<(const Knot& another) const {
    if (x < another.x) return true;
    if (x > another.x) return false;
    return y < another.y;
}

void insertKnot(std::vector<std::vector<Knot>>& grid, const Knot& knot, float cellsize) {
    int xindex = static_cast<int>(std::floor(knot.x / cellsize));
    int yindex = static_cast<int>(std::floor(knot.y / cellsize));
    if (xindex >= 0 && xindex < grid.size() &&
        yindex >= 0 && yindex < grid[0].size()) {
        grid[xindex][yindex] = knot;
    }
}

bool isKnotValid(const std::vector<std::vector<Knot>>& grid, int g_width, int g_height,
    const Knot& knot, float radius, float width, float height, float cellsize) {
    if (knot.x < 0 || knot.x >= width || knot.y < 0 || knot.y >= height) { return false; }

    int xindex = static_cast<int>(std::floor(knot.x / cellsize));
    int yindex = static_cast<int>(std::floor(knot.y / cellsize));
    int i0 = std::max(xindex - 2, 0);
    int i1 = std::min(xindex + 2, g_width - 1);
    int j0 = std::max(yindex - 2, 0);
    int j1 = std::min(yindex + 2, g_height - 1);

    for (int i = i0; i <= i1; ++i) {
        for (int j = j0; j <= j1; ++j) {
            const Knot& other = grid[i][j];
            if (other.x != -1 && distanceBetweenPoints(other.x, other.y, knot.x, knot.y) < radius) {
                return false;
            }
        }
    }
    return true;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<Knot> poissonDiskSampling(float width, float height, double radius, int limit) {
    const int N = 2;
    std::vector<Knot> knots;
    std::vector<Knot> active;

    float cellsize = static_cast<float>(radius / std::sqrt(N));
    cellsize = std::max(cellsize, 1.0f);

    int ncells_width = static_cast<int>(std::ceil(width / cellsize)) + 1;
    int ncells_height = static_cast<int>(std::ceil(height / cellsize)) + 1;

    std::vector<std::vector<Knot>> grid(ncells_width, std::vector<Knot>(ncells_height));

    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist_x(0.0f, width);
    std::uniform_real_distribution<float> dist_y(0.0f, height);

    Knot k0(dist_x(gen), dist_y(gen));
    insertKnot(grid, k0, cellsize);
    knots.push_back(k0);
    active.push_back(k0);

    while (!active.empty()) {
        std::uniform_int_distribution<size_t> RKI_gen(0, active.size() - 1);
        size_t random_knot_index = RKI_gen(gen);
        Knot active_knot = active[random_knot_index];

        bool found = false;
        for (int tries = 0; tries < limit; ++tries) {
            std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * static_cast<float>(M_PI));
            float theta = angle_dist(gen);

            std::uniform_real_distribution<float> radius_dist(radius, 2.0f * radius);
            float random_radius = radius_dist(gen);

            Knot knew(
                active_knot.x + random_radius * std::cos(theta),
                active_knot.y + random_radius * std::sin(theta)
            );

            if (isKnotValid(grid, ncells_width, ncells_height, knew, radius, width, height, cellsize)) {
                knots.push_back(knew);
                insertKnot(grid, knew, cellsize);
                active.push_back(knew);
                found = true;
                break;
            }
        }

        if (!found) { active.erase(active.begin() + random_knot_index); }
    }

    return knots;
}