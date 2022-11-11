#pragma once
#include <array>
#include <utility>
#include <vector>
#include "../position/2DVec.hpp"

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
struct Vertex {
  vec2 pos;
  vec3 col;
};

class Agent {
 public:
  // Variables
  Vertex v1;
  Vertex v2;
  Vertex v3;
  Dvec center;
  Dvec speed;
  float angle;
  int index;
  bool prediator = false;
  bool alive = true;
  int prey = -1;
  int team = 0;

  // Constructors
  Agent();
  Agent(vec2 Pos, float angle, int id, vec3 col, int t);

  void rotate(float angle);
  void advance();
  void keep_in(float width, float height);
};

using group = std::vector<Agent>;

bool visible(Dvec speed, Dvec pos1, Dvec pos2);
 
group random_initializer(std::vector<Vertex>& vertex_data);

void revive(group& boids, float x, float y);

neighbors updateNeighbors(int agentIndex, group birds);




