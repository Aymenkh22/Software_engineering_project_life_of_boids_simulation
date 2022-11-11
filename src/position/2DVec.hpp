#pragma once
#include <iostream>
#include <cmath>
#include "../Utils/constants.hpp"
 
using vec2 = std::array<float, 2>;
class Dvec {
 public:
  float x, y;
  // constructors
  Dvec(void) {
    x = 0;
    y = 0;
  }
  Dvec(float _x, float _y) {
    x = _x;
    y = _y;
  }
  ~Dvec(void) {
    x = 0;
    y = 0;
  }
  // operators
  friend Dvec operator+(Dvec v1, Dvec v2) {
    Dvec v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    return v3;
  }
  friend Dvec operator-(Dvec v1, Dvec v2) {
    Dvec v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    return v3;
  }
  friend Dvec operator*(Dvec v1, float num) {
    Dvec v2;
    v2.x = v1.x * num;
    v2.y = v1.y * num;
    return v2;
  }
  friend Dvec operator/(Dvec v1, float num) {
    Dvec v2;
    v2.x = v1.x / num;
    v2.y = v1.y / num;
    return v2;
  }
  friend bool operator==(Dvec v1, Dvec v2) {
    if (v2.x == v1.x && v2.y == v1.y)
      return true;
    return false;
  }
  friend bool operator!=(Dvec v1, Dvec v2) {
    if (v2.x != v1.x || v2.y != v1.y)
      return true;
    return false;
  }

  float magnitude() { return (float)(sqrt(x * x + y * y)); }
  float Dsquare() { return (float)(x * x + y * y); }

  void normalize() {
    float norm = magnitude();
    if (norm != 0) {
      x = x / norm;
      y = y / norm;
    }
  }
};

