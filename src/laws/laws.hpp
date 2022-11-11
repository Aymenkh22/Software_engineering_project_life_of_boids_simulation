#pragma once
#include "../Agents/Agent.hpp"

Dvec seperation(int agentIndex, group birds, neighbors neighbor);

Dvec prediators_separation(int agentIndex, group birds, neighbors neighbor);

Dvec alignment(int agentIndex, group birds, neighbors neighbor);

Dvec cohesion(int agentIndex, group birds, neighbors neighbor);

Dvec escape_prediator(int agentIndex, group birds, neighbors neighbor);

Dvec hunt(int agentIndex, group& birds, neighbors neighbor);

void move(int index, group& birds, neighbors neighbor);

void eat(group& boids);

void magic(std::vector<Vertex>& vertex_data, group& boids, float w, float h);
