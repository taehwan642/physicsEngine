#pragma once

namespace Engine {
class Time {
 private:
  double prevTime = 0;
  double currentTime = 0;
  double timeDiff = 0;
  unsigned int counter = 0;
  // input
  double maxFPS = 30;
  // output
  double FPS = 0;
  double ms = 0;

 public:
  void Update();
  double GetFPS();
};
}  // namespace Engine