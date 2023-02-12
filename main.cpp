#include <iostream>
#include <functional>
#include "array.hpp"
#include "logic.hpp"
#include "render.hpp"

typedef tarray<
  #include "reward.inc"
> rewards;

typedef tarray<
  #include "cmd.inc"
> cmds;

int main() {
  canvas<game_run<20, 8, cmds, rewards>>::draw();
  return game_run<20, 8, cmds, rewards>::is_over;
}
