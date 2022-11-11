#include "kdTree.h"
#include <cmath>

Node::Node(Agent boid, bool vertica, Node::NodePtr l, Node::NodePtr r) {
  agent = boid;
  vertical = vertica;
  left = l;
  right = r;
}

Node::Node()= default;

bool Node::isLeaf() {
  return this->left == NULL && this->right == NULL;
}

KDTree::KDTree(float width, float height) {
  this->root = NULL;
  this->width = width;
  this->height = height;
}

Node::NodePtr KDTree::insert(Node::NodePtr node, Agent boid, bool vertical) {
  if (node == NULL) {
    return new Node(boid, vertical, NULL, NULL);
    ;
  }

  if ((node->vertical && boid.center.x < node->agent.center.x)
      || (!node->vertical && boid.center.y < node->agent.center.y)) {
    node->left = insert(node->left, boid, !node->vertical);
  } else {
    node->right = insert(node->right, boid, !node->vertical);
  }
  return node;
}

void KDTree::insert(Agent boid) {
  this->root = insert(this->root, boid, true);
}
 
void KDTree::search(Agent query, double radius, Node::NodePtr node, std::vector<int>& results) {
  double w = (query.center - node->agent.center).Dsquare();

  if (node->isLeaf()) {
    if (w < radius * radius && visible(query.speed, query.center, node->agent.center)) {
      results.push_back(node->agent.index);
    }
    return;
  }

  if (w < radius * radius)
    results.push_back(node->agent.index);

  double qx = query.center.x;
  double nx = node->agent.center.x;
  double qy = query.center.y;
  double ny = node->agent.center.y;

  if (node->vertical) {
    if (qx < nx) {
      if (qx - radius <= nx && node->left != NULL) {
        search(query, radius, node->left, results);
      }
      if (qx + radius > nx && node->right != NULL) {
        search(query, radius, node->right, results);
        return;
      }
    } else {
      if (qx + radius > nx && node->right != NULL) {
        search(query, radius, node->right, results);
      }
      if (qx - radius <= nx && node->left != NULL) {
        search(query, radius, node->left, results);
        return;
      }
    }
  } else {
    if (qy < ny) {
      if (qy - radius <= ny && node->left != NULL) {
        search(query, radius, node->left, results);
      }
      if (qy + radius > ny && node->right != NULL) {
        search(query, radius, node->right, results);
        return;
      }
    } else {
      if (qy + radius > ny && node->right != NULL) {
        search(query, radius, node->right, results);
      }
      if (qy - radius <= ny && node->left != NULL) {
        search(query, radius, node->left, results);
        return;
      }
    }
  }
}

neighbors KDTree::search(Agent query, const float radius) {
  std::vector<int> results;
  search(query, radius, root, results);
  return results;
}