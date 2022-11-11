#pragma once 
#include <memory>
#include <vector>
#include "../Agents/Agent.hpp"


class Node {
 public:
  using NodePtr = Node*;
  NodePtr left, right;
  Agent agent;
  bool vertical;

  Node(Agent boid, bool vertical, NodePtr left, NodePtr right);

  Node();

  bool isLeaf();
};

class KDTree {
 private:
  float width, height;

  Node::NodePtr root;

  Node::NodePtr insert(Node::NodePtr node, Agent boid, bool vertical);

  void search(Agent query, double radius, Node::NodePtr node, std::vector<int>& results);

 public:
  KDTree(float width, float height);

  void insert(Agent boid);

  neighbors search(Agent query, const float radius);
};
