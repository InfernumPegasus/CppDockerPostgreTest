#pragma once

#include <chrono>
#include <iostream>

class Timer {
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

 public:
  Timer() { m_start = Clock::now(); }

  ~Timer() {
    const TimePoint end = Clock::now();
    std::cout
        << "Time : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count()
        << std::endl;
  }

 private:
  TimePoint m_start;
};