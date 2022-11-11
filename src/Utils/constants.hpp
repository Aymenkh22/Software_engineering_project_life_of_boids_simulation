#pragma once

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat3x3 = std::array<vec3, 3>;
using mat4x4 = std::array<vec4, 4>;

const int SIZE =1000;
const float max_l = 460;
const int num_prediators = 5;
const float UNIT_SPEED = (float)0.3;
const float PI = (float)3.14159265;
const float NeighboRadius = 150;
const float SeperationRadius = 150;
const float step = 4;
const float width=1600;
const float height = 1200;
const float r = 10;
const float vision_angle =2* PI / 3;
using neighbors = std::vector<int>; 
const float sWeight = 3;
const float aWeight = 2;
const float cWeight = 2;
