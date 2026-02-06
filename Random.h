#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Random {
inline std::mt19937 &get_engine() {
  static std::random_device rd;
  static std::mt19937 engine(rd());
  return engine;
}

inline int get_int(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(get_engine());
}
} // namespace Random

#endif
