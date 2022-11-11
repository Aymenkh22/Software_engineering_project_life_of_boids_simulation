#include <cmath>
#include <iostream>
#include <vector>
#include "laws/laws.hpp"
#include "Agents/Agent.hpp"
#include "gtest/gtest.h"

TEST(Laws, Eat) {
  std::vector<Vertex> vertex_data(SIZE * 3);
  group boids = random_initializer(vertex_data);

  boids[num_prediators] = Agent({20.0, 25.0}, 0.0, 2, {1.0, 1.0, 1.0}, 1);
  boids[num_prediators + 1].center = Dvec(boids[0].v2.pos[0], (boids[0].v2.pos[1]));

  eat(boids);

  ASSERT_FALSE(boids[num_prediators + 1].alive);
}

