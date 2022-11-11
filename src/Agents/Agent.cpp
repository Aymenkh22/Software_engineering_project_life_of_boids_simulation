#include "Agent.hpp"
#include "omp.h"
#include <random>


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 2 * height);
std::uniform_int_distribution<> dis1(1, 2 * width);
std::uniform_int_distribution<> dis2(0, PI * 2);

Agent::Agent() {
  v1.pos[0] = 0;
  v2.pos[0] = width / 80;
  v3.pos[0] = 0;
  v1.pos[1] = height / 160;
  v2.pos[1] = height / 80;
  v3.pos[1] = 3 * height / 160;
  v1.col = {1.0, 0.0, 0.0};
  v2.col = {1.0, 0.0, 0.0};
  v3.col = {1.0, 0.0, 0.0};
  center = Dvec(width / 120, height / 40);
  speed = Dvec(1, 0);
  angle = 0;
  index = 0;
}

Agent::Agent(vec2 Pos, float Angle, int id, vec3 col, int t) {
  auto rand_x = Pos[0];
  auto rand_y = Pos[1];
  float s = std::sin(angle);
  float c = std::cos(angle);
  v1.pos[0] = ((-20 * c) + 600 / 80 * s + rand_x + 800 / 40) / 2;
  v2.pos[0] = (rand_x + 800 / 40) / 2;
  v3.pos[0] = ((-20 * c) - 600 / 80 * s + rand_x + 800 / 40) / 2;
  v1.pos[1] = ((-20 * s) - 600 / 80 * c + rand_y + 600 / 40) / 2;
  v2.pos[1] = (rand_y + 600 / 40) / 2;
  v3.pos[1] = ((-800 / 40 * s) + 600 / 80 * c + rand_y + 600 / 40) / 2;
  float x4 = v2.pos[0] - v1.pos[0] / 2 - v3.pos[0] / 2;
  float y4 = v2.pos[1] - v1.pos[1] / 2 - v3.pos[1] / 2;
  float norm = (float)sqrt(pow(x4, 2) + pow(y4, 2));
  center = Dvec((v1.pos[0] + v2.pos[0] + v3.pos[0]) / 3, (v1.pos[1] + v2.pos[1] + v3.pos[1]) / 3);
  speed = Dvec(x4 / norm, y4 / norm);
  angle = Angle;
  index = id;
  v1.col = col;
  v2.col = col;
  v3.col = col;
  team = t;
}

void Agent::rotate(float ang) {
  float s = std::sin(ang);
  float c = std::cos(ang);
  auto x5 = center.x;
  auto y5 = center.y;
  float x1 = (v1.pos[0] - x5) * c - (v1.pos[1] - y5) * s + x5;
  float x2 = (v2.pos[0] - x5) * c - (v2.pos[1] - y5) * s + x5;
  float x3 = (v3.pos[0] - x5) * c - (v3.pos[1] - y5) * s + x5;
  float y1 = (v1.pos[0] - x5) * s + (v1.pos[1] - y5) * c + y5;
  float y2 = (v2.pos[0] - x5) * s + (v2.pos[1] - y5) * c + y5;
  float y3 = (v3.pos[0] - x5) * s + (v3.pos[1] - y5) * c + y5;
  v1.pos[0] = x1;
  v2.pos[0] = x2;
  v3.pos[0] = x3;
  v1.pos[1] = y1;
  v2.pos[1] = y2;
  v3.pos[1] = y3;
  center = Dvec((v1.pos[0] + v2.pos[0] + v3.pos[0]) / 3, (v1.pos[1] + v2.pos[1] + v3.pos[1]) / 3);
  speed.x = v2.pos[0] - center.x;
  speed.y = v2.pos[1] - center.y;
  speed.normalize();
  angle = fmod(angle + ang, 2 * PI);
}

void Agent::advance() {
  if (prediator) {
    v1.pos[0] += speed.x * step * 1.5;
    v2.pos[0] += speed.x * step * 1.5;
    v3.pos[0] += speed.x * step * 1.5;
    v1.pos[1] += speed.y * step * 1.5;
    v2.pos[1] += speed.y * step * 1.5;
    v3.pos[1] += speed.y * step * 1.5;
  } else {
    v1.pos[0] += speed.x * step;
    v2.pos[0] += speed.x * step;
    v3.pos[0] += speed.x * step;
    v1.pos[1] += speed.y * step;
    v2.pos[1] += speed.y * step;
    v3.pos[1] += speed.y * step;
  }
  center = Dvec((v1.pos[0] + v2.pos[0] + v3.pos[0]) / 3, (v1.pos[1] + v2.pos[1] + v3.pos[1]) / 3);
}

void Agent::keep_in(float width, float height) {
  auto bord = width / 50;
  bool outside = (((abs(fmod(v1.pos[0], width)) < bord) || (abs(fmod(v1.pos[1], height)) < bord))
                  || (((abs(fmod(v2.pos[0], width)) < bord) || (abs(fmod(v2.pos[1], height)) < bord))
                      || ((abs(fmod(v3.pos[0], width)) < bord) || (abs(fmod(v3.pos[1], height)) < bord))));

  v1.pos[0] = outside ? v1.pos[0] : fmod(fmod(v1.pos[0], width) + width, width);
  v2.pos[0] = outside ? v2.pos[0] : fmod(fmod(v2.pos[0], width) + width, width);
  v3.pos[0] = outside ? v3.pos[0] : fmod(fmod(v3.pos[0], width) + width, width);
  v1.pos[1] = outside ? v1.pos[1] : fmod(fmod(v1.pos[1], height) + height, height);
  v2.pos[1] = outside ? v2.pos[1] : fmod(fmod(v2.pos[1], height) + height, height);
  v3.pos[1] = outside ? v3.pos[1] : fmod(fmod(v3.pos[1], height) + height, height);
  center = Dvec((v1.pos[0] + v2.pos[0] + v3.pos[0]) / 3, (v1.pos[1] + v2.pos[1] + v3.pos[1]) / 3);
}

bool visible(Dvec speed, Dvec pos1, Dvec pos2) {
  Dvec vec = Dvec(pos2.x - pos1.x, pos2.y - pos1.y);
  float angle = acos(((vec.x * speed.x + vec.y * speed.y) / (vec.magnitude() * speed.magnitude())));
  if (angle > vision_angle)
    return false;
  else
    return true;
}

group random_initializer(std::vector<Vertex>& vertex_data) {
  group boids(SIZE);
  float angle;
  float rand_x;
  float rand_y;
#pragma omp parallel for
  for (int i = 0; i < (SIZE + num_prediators) / 2; i++) {
    angle = dis2(gen);
    rand_x = dis1(gen);
    rand_y = dis(gen);
    boids[i] = Agent({rand_x, rand_y}, angle, i, {0.3, 0.5, 0.0}, 1);
    vertex_data[i * 3] = boids[i].v1;
    vertex_data[i * 3 + 1] = boids[i].v2;
    vertex_data[i * 3 + 2] = boids[i].v3;
  }
#pragma omp parallel for
  for (int i = (SIZE + num_prediators) / 2; i < SIZE; i++) {
    angle = dis2(gen);
    rand_x = dis1(gen);
    rand_y = dis(gen);
    boids[i] = Agent({rand_x, rand_y}, angle, i, {0.0, 0.5, 0.9}, 2);
    vertex_data[i * 3] = boids[i].v1;
    vertex_data[i * 3 + 1] = boids[i].v2;
    vertex_data[i * 3 + 2] = boids[i].v3;
  }
#pragma omp parallel for
  for (int i = 0; i < num_prediators; i++) {
    boids[i].prediator = true;
    boids[i].v1.col = {0.8, 0.5, 0.0};
    boids[i].v2.col = {1.0, 0.0, 0.0};
    boids[i].v3.col = {0.8, 0.5, 0.0};
    boids[i].v1.pos[0] = boids[i].v1.pos[0] * 3;
    boids[i].v1.pos[1] = boids[i].v1.pos[1] * 3;
    boids[i].v2.pos[0] = boids[i].v2.pos[0] * 3;
    boids[i].v2.pos[1] = boids[i].v2.pos[1] * 3;
    boids[i].v3.pos[0] = boids[i].v3.pos[0] * 3;
    boids[i].v3.pos[1] = boids[i].v3.pos[1] * 3;
  }
  return boids;
}


void revive(group& boids, float x, float y) {
#pragma omp parallel for
  for (int i = num_prediators; i < SIZE; i++) {
    float angle = dis2(gen);
    if (!boids[i].alive) {
      boids[i] = Agent({x, y}, angle, i, boids[i].v1.col, boids[i].team);
    }
  }
}

neighbors updateNeighbors(int agentIndex, group birds) {
  neighbors aux;
  int count = 0;
  for (int i = 0; i < agentIndex; i++) {
    auto vec = birds[agentIndex].center - birds[i].center;
    auto d = vec.magnitude();
    if (d < NeighboRadius && visible(birds[agentIndex].speed, birds[agentIndex].center, birds[i].center)) {
      aux.push_back(i);
    }
  }
  for (int i = agentIndex + 1; i < birds.size(); i++) {
    auto vec = birds[agentIndex].center - birds[i].center;
    auto d = vec.magnitude();
    if (d < NeighboRadius && visible(birds[agentIndex].speed, birds[agentIndex].center, birds[i].center)) {
      aux.push_back(i);
    }
  }
  return aux;
}
