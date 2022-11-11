#include "laws.hpp"
#include "omp.h"
#include "../kd_tree/kdTree.h"


Dvec seperation(int agentIndex, group birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  if (birds[agentIndex].alive && (!birds[agentIndex].prediator)) {
    for (int i = 0; i < neighbor.size(); i++) {
      if (birds[neighbor[i]].alive) {
        Dvec aux = birds[agentIndex].center - birds[neighbor[i]].center;
        float d = aux.magnitude();
        if (d > 0 && d < SeperationRadius) {
          Dvec aux = birds[neighbor[i]].center - birds[agentIndex].center;
          aux.normalize();
          aux = aux / d;
          vec = vec - aux;
          count++;
        }
      }
    }
    if (count != 0) {
      vec.normalize();
    }
  }
  return vec;
}

Dvec prediators_separation(int agentIndex, group birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  if (birds[agentIndex].prediator) {
    for (int i = 0; i < neighbor.size(); i++) {
      if (birds[neighbor[i]].prediator) {
        Dvec aux = birds[agentIndex].center - birds[neighbor[i]].center;
        float d = aux.magnitude();
        if (d > 0 && d <  SeperationRadius) {
          Dvec aux = birds[neighbor[i]].center - birds[agentIndex].center;
          aux.normalize();
          aux = aux / d;
          vec = vec - aux;
          count++;
        }
      }
    }
    if (count != 0) {
      vec.normalize();
    }
  }
  return vec;
}

Dvec alignment(int agentIndex, const group birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  if (birds[agentIndex].alive && (!birds[agentIndex].prediator)) {
    for (int i = 0; i < neighbor.size(); i++) {
      if (!birds[neighbor[i]].prediator && birds[neighbor[i]].alive
          && birds[agentIndex].team == birds[neighbor[i]].team) {
        Dvec aux = birds[agentIndex].center - birds[neighbor[i]].center;
        vec = vec + birds[neighbor[i]].speed;
        count++;
      }
    }
    if (count != 0)
      vec.normalize();
  }
  return vec;
}

Dvec cohesion(int agentIndex, group birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  if (birds[agentIndex].alive && (!birds[agentIndex].prediator)) {
    for (int i = 0; i < neighbor.size(); i++) {
      if (!birds[neighbor[i]].prediator && birds[neighbor[i]].alive
          && birds[agentIndex].team == birds[neighbor[i]].team) {
        Dvec aux = birds[agentIndex].center - birds[neighbor[i]].center;
        vec = vec + birds[neighbor[i]].center;
        count++;
      }
    }
    if (count != 0) {
      vec = vec / count - birds[agentIndex].center;
      vec.normalize();
    }
  }
  return vec;
}

Dvec escape_prediator(int agentIndex, group birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  if (!birds[agentIndex].prediator && birds[agentIndex].alive) {
    for (int i = 0; i < neighbor.size(); i++) {
      if (birds[neighbor[i]].prediator) {
        Dvec aux = birds[agentIndex].center - birds[neighbor[i]].center;
        float d = aux.magnitude();
        if (d > 0 && d < SeperationRadius) {
          Dvec aux = birds[neighbor[i]].center - birds[agentIndex].center;
          aux.normalize();
          aux = aux / d;
          vec = vec - aux;
          count++;
        }
      }
    }
  }
  if (count != 0) {
    vec.normalize();
  }
  return vec;
}

Dvec hunt(int agentIndex, group& birds, neighbors neighbor) {
  Dvec vec = Dvec(0.0, 0.0);
  int count = 0;
  float dis = NeighboRadius * 2;
  if (birds[agentIndex].prediator) {
    if (birds[agentIndex].prey == -1) {
      for (int i = 0; i < neighbor.size(); i++) {
        if ((!birds[neighbor[i]].prediator) && birds[neighbor[i]].alive) {
          Dvec mouth = Dvec(birds[agentIndex].v2.pos[0], (birds[agentIndex].v2.pos[1]));
          Dvec aux = birds[neighbor[i]].center - mouth;
          float d = aux.magnitude();
          if (d != 0 && d < dis) {
            dis = d;
            vec = aux;
            count = 1;
            birds[agentIndex].prey = neighbor[i];
          }
        }
      }
    } else {
      int i = birds[agentIndex].prey;
      Dvec mouth = Dvec(birds[agentIndex].v2.pos[0], (birds[agentIndex].v2.pos[1]));
      Dvec aux = birds[i].center - mouth;
      float d = aux.magnitude();
      if (d < NeighboRadius) {
        vec = aux;
        count = 1;
      } else
        birds[agentIndex].prey = -1;
    }
  }
  if (count != 0) {
    vec.normalize();
  }
  return vec;
}

void move(int index, group& birds, neighbors neighbor) {
  Dvec vec = (seperation(index, birds, neighbor) * sWeight) + (alignment(index, birds, neighbor) * aWeight)
             + (cohesion(index, birds, neighbor) * cWeight) + escape_prediator(index, birds, neighbor) * 10
             + prediators_separation(index, birds, neighbor) + hunt(index, birds, neighbor);
  if (vec.magnitude() != 0) {
    vec.normalize();
    float angle = atan2(vec.x * birds[index].speed.y - vec.y * birds[index].speed.x,
                        vec.x * birds[index].speed.x + vec.y * birds[index].speed.y);

    angle = -fmod(angle, PI / 5);

    birds[index].rotate(angle);
  }
  birds[index].advance();
}

void eat(group& boids) {
  for (int i = num_prediators; i < SIZE; i++) {
    bool size_check = false;
    auto d1 = Dvec(boids[i].v1.pos[0] - boids[i].v2.pos[0], boids[i].v1.pos[1] - boids[i].v2.pos[1]).Dsquare();
    auto d2 = Dvec(boids[i].v1.pos[0] - boids[i].v3.pos[0], boids[i].v1.pos[1] - boids[i].v3.pos[1]).Dsquare();
    auto d3 = Dvec(boids[i].v3.pos[0] - boids[i].v2.pos[0], boids[i].v3.pos[1] - boids[i].v2.pos[1]).Dsquare();
    if (d1 > max_l || d2 > max_l || d3 > max_l) {
      size_check = true;
    }
    #pragma omp parallel for
    for (int j = 0; j < num_prediators; j++) {
      Dvec vec = Dvec(boids[j].v2.pos[0], (boids[j].v2.pos[1]));
      Dvec aux = vec - boids[i].center;
      float d = aux.Dsquare();
      if (d < r * r || size_check) {
        boids[i].alive = false;
        boids[i].v1.pos = {0.0, 0.0};
        boids[i].v2.pos = {0.0, 0.0};
        boids[i].v3.pos = {0.0, 0.0};
        if (!size_check && boids[j].prey==i) {
          boids[j].prey = -1;
        }
      }
    }
  }
}

void magic(std::vector<Vertex>& vertex_data, group& boids, float w, float h) {
  auto tree = KDTree(width, height);
  for (Agent b : boids)
    tree.insert(b); /**/
  neighbors neighberhood[SIZE];
#pragma omp parallel for
  for (int i = 0; i < SIZE; i++) {
    neighberhood[i] = tree.search(boids[i], NeighboRadius);  // neighberhood update using kdTree
    // neighberhood[i] = updateNeighbors(i, boids);//Neighberhood update using basic method
  }
#pragma omp parallel for
  for (int i = 0; i < SIZE; i++) {
    move(i, boids, neighberhood[i]);
    boids[i].keep_in(w, h);
  }
  eat(boids);
#pragma omp parallel for
  for (int i = 0; i < SIZE; i++) {
    vertex_data[3 * i] = boids[i].v1;
    vertex_data[3 * i + 1] = boids[i].v2;
    vertex_data[3 * i + 2] = boids[i].v3;
  }
}