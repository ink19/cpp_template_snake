#ifndef MAIN_LOGIC
#define MAIN_LOGIC

#include "array.hpp"

template <int width, int height, int reward_index>
class reward_index_trans {
public:
  const static int game_width = width - 1;
  const static int game_height = height - 1;
  const static int game_pixel_count = game_width * game_height;

  const static int reward_game_pixel_index = reward_index % game_pixel_count;
  const static int value = reward_game_pixel_index % game_width + 1 + ((reward_game_pixel_index / game_width) + 1) * width;
};

template <int width, int height, int action_index>
class game_action_trans {
public:
  const static int value = 0;
  const static int width_value = 0;
  const static int height_value = 0;
};

template <int width, int height>
class game_action_trans<width, height, 1> {
public:
  const static int value = -width;
  const static int width_value = 0;
  const static int height_value = -1;
};

template <int width, int height>
class game_action_trans<width, height, 2> {
public:
  const static int value = width;
  const static int width_value = 0;
  const static int height_value = 1;
};

template <int width, int height>
class game_action_trans<width, height, 3> {
public:
  const static int value = -1;
  const static int width_value = -1;
  const static int height_value = 0;
};

template <int width, int height>
class game_action_trans<width, height, 4> {
public:
  const static int value = 1;
  const static int width_value = 1;
  const static int height_value = 0;
};

template<typename T, bool is_eat>
class agent_tracks_pop;

template<typename T>
class agent_tracks_pop<T, true> {
public:
  typedef T type;
};

template<typename T>
class agent_tracks_pop<T, false> {
public:
  typedef typename T::pop_front_type type;
};

template <int width, int height, int agent_width, int agent_height>
class is_game_beat_wall {
public:
  const static bool n_beat = agent_height < 1;
  const static bool s_beat = agent_height > height - 2;
  const static bool w_beat = agent_width < 1;
  const static bool e_beat = agent_width > width - 2;
  const static bool value = n_beat || s_beat || w_beat || e_beat;
};

template <int width, int height, typename cmd, typename rewards, int step>
class game_run_impl;

template <int width, int height, typename cmd, typename rewards>
class game_run: public game_run_impl<width, height, cmd, rewards, cmd::size>{};

template <int pwidth, int pheight, typename cmd, typename rewards, int step>
class game_run_impl{
public:
  const static int width = pwidth;
  const static int height = pheight;
  typedef game_run_impl<width, height, cmd, rewards, step> type;
  typedef game_run_impl<width, height, cmd, rewards, step - 1> last_type;
  const static int action = tarray_get<cmd, step - 1>::value;
  typedef game_action_trans<width, height, action> action_type;

  const static int agent_pixel_index = last_type::agent_pixel_index + action_type::value;
  const static int agent_width = last_type::agent_width + action_type::width_value;
  const static int agent_height = last_type::agent_height + action_type::height_value;
  const static bool is_eat = agent_pixel_index == last_type::reward_pixel_index;

  const static int reward_index = last_type::reward_index + is_eat;
  const static int reward_pixel_index = reward_index_trans<width, height, tarray_get<rewards, reward_index>::value>::value;

  
  typedef typename agent_tracks_pop<typename last_type::agent_tracks, is_eat>::type last_agent_tracks;
  const static bool agent_beat_tail = tarray_is_contain<last_agent_tracks, agent_pixel_index>::value;
  const static bool is_over = is_game_beat_wall<width, height, agent_width, agent_height>::value || agent_beat_tail || last_type::is_over;

  typedef typename tarray_push_back<last_agent_tracks, agent_pixel_index>::type agent_tracks;
};

template <int pwidth, int pheight, typename cmd, typename rewards>
class game_run_impl<pwidth, pheight, cmd, rewards, 0>{
public:
  const static int width = pwidth;
  const static int height = pheight;
  typedef game_run_impl<width, height, cmd, rewards, 0> type;
  const static int reward_index = 0;
  const static int reward_pixel_index = reward_index_trans<width, height, tarray_get<rewards, reward_index>::value>::value;
  const static bool is_eat = false;
  const static int agent_pixel_index = width * 2 + 2;
  const static int agent_width = 2;
  const static int agent_height = 2;
  const static bool is_over = false;

  typedef tarray<width * 2 + 2> agent_tracks;
};

#endif
