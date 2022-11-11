#include <cmath>
#include <iostream>
#include <vector>
#include "Agents/Agent.hpp"
#include "gtest/gtest.h"

TEST(Agents, Rotate) {
  float angle = 0;
  Agent agent = Agent({1.0, 1.0}, 0.0, 1, {1.0, 1.0, 1.0}, 1);

  float testAngle = (float)PI / 2;

  agent.rotate(testAngle);


  ASSERT_EQ(agent.angle, testAngle) << "Angle should be " << testAngle;
}

TEST(Agents, Advance) {
  Agent agent = Agent({1.0, 1.0}, 5, 6, {1.0, 1.0, 1.0}, 1);
  float testAngle = (float)PI / 4;
  agent.rotate(testAngle);
  auto old_x = agent.center.x;
  auto old_y = agent.center.y;
  agent.advance();
  ASSERT_NEAR(agent.center.x, old_x + agent.speed.x  *step, 1.0e-5);
  ASSERT_NEAR(agent.center.y, old_y + agent.speed.y * step, 1.0e-5);
}

TEST(Agents, Visibility) {
  Agent agent1 = Agent({2.0, 1.0}, 0.0, 1, {1.0, 1.0, 1.0}, 1);
  Agent agent2 = Agent({2.0, 2.0}, 0.0, 2, {1.0, 1.0, 1.0}, 2);
  // visible({1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0});

  ASSERT_EQ(visible({1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}), true) << "Should Return True ";
}

TEST(Agents, keep_in) {
  auto bord = width / 50;
  Agent agent = Agent({1.0, 1.0}, 0.0, 1, {1.0, 1.0, 1.0}, 1);
  agent.v1.pos[0] = width + 2 * bord;
  agent.v2.pos[0] = width + 2 * bord;
  agent.v3.pos[0] = width + 2 * bord;
  agent.v1.pos[1] = height + 2 * bord;
  agent.v2.pos[1] = height + 2 * bord;
  agent.v3.pos[1] = height + 2 * bord;
  agent.keep_in(width, height);

  ASSERT_EQ(agent.v1.pos[0], (float)2 * bord) << "Pos should be " << 2 * bord;
  ASSERT_EQ(agent.v2.pos[0], (float)2 * bord) << "Pos should be " << 2 * bord;
  ASSERT_EQ(agent.v3.pos[0], (float)2 * bord) << "Pos should be " << 2 * bord;
  ASSERT_EQ(agent.v1.pos[1], (float)2 * bord) << "Pos should be " << 2 * bord;
  ASSERT_EQ(agent.v2.pos[1], (float)2 * bord) << "Pos should be " << 2 * bord;
  ASSERT_EQ(agent.v3.pos[1], (float)2 * bord) << "Pos should be " << 2 * bord;
}

TEST(Agents, updateNeighbors) {
  Agent agent1 = Agent({1.0, 1.0}, 0.0, 1, {1.0, 1.0, 1.0}, 1);
  Agent agent2 = Agent({1.15, 1.15}, 0.0, 2, {1.0, 1.0, 1.0}, 1);
  Agent agent3 = Agent({1.2, 1.2}, 0.0, 3, {1.0, 1.0, 1.0}, 1);
  Agent agent4 = Agent({(float)5.0 + NeighboRadius, (float)5.5 + NeighboRadius}, 0.0, 4, {1.0, 1.0, 1.0}, 1);

  group birds;
  birds.push_back(agent1);
  birds.push_back(agent2);
  birds.push_back(agent3);
  birds.push_back(agent4);

  neighbors neighbor= updateNeighbors(1,  birds);
  bool find2 = false;
  bool find3 = false;
  bool find4 = false;

  for (int i = 0; i < neighbor.size(); i++) {
    if (neighbor[i] == 2)
      find2 = true;
    else if (neighbor[i] == 3)
      find3 = true;
    else
      find4 = true;
  }
  ASSERT_TRUE(find2);
  ASSERT_TRUE(find3);
  ASSERT_FALSE(find4);
}
