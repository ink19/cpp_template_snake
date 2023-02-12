#ifndef MAIN_RENDER
#define MAIN_RENDER

#include <iostream>
#include "array.hpp"
#include "logic.hpp"

#define VIDEO_PIXEL_BLANK 0
#define VIDEO_PIXEL_SNAKE 1
#define VIDEO_PIXEL_WALL 2
#define VIDEO_PIXEL_GOODS 3

template <int pixel_type>
class video_pixel;

template <>
class video_pixel<VIDEO_PIXEL_BLANK> {
public:
  const static char value = ' ';
};

template <>
class video_pixel<VIDEO_PIXEL_WALL> {
public:
  const static char value = '*';
};

template <>
class video_pixel<VIDEO_PIXEL_SNAKE> {
public:
  const static char value = '#';
};

template <>
class video_pixel<VIDEO_PIXEL_GOODS> {
public:
  const static char value = '$';
};

template <bool is_wall, bool is_agent, bool is_reward>
class pixel_value;

template <bool is_agent, bool is_reward>
class pixel_value<true, is_agent, is_reward>: public video_pixel<VIDEO_PIXEL_WALL>{};

template <bool is_reward>
class pixel_value<false, true, is_reward>: public video_pixel<VIDEO_PIXEL_SNAKE>{};

template <>
class pixel_value<false, false, true>: public video_pixel<VIDEO_PIXEL_GOODS>{};

template <>
class pixel_value<false, false, false>: public video_pixel<VIDEO_PIXEL_BLANK>{};

template <typename pgame_run_type, bool is_over>
class canvas_impl;

template <typename pgame_run_type, int pixel_desc_index, bool isNext>
class canvas_render;

template <typename pgame_run_type>
class canvas: public canvas_impl<pgame_run_type, pgame_run_type::is_over> {};

template <typename pgame_run_type>
class canvas_impl<pgame_run_type, false> {
public:
  typedef pgame_run_type game_run_type;
  const static int width = game_run_type::width;
  const static int height = game_run_type::height;
  const static int pixel_count = width * height;
  typedef canvas_impl<game_run_type, false> type;
  typedef canvas_render<type, pixel_count, false> m_canvas_render;
  static void draw() {
    m_canvas_render::draw();
  }
};

template <typename pgame_run_type>
class canvas_impl<pgame_run_type, true> {
public:
  static void draw() {
    std::cout << "Over." << std::endl;
    std::cout << "Score:" << pgame_run_type::reward_index << std::endl;
  }
};

template <typename pcanvas_type, int pixel_desc_index>
class canvas_render<pcanvas_type, pixel_desc_index, false> {
public:
  const static int pixel_index = pcanvas_type::pixel_count - pixel_desc_index;
  const static int width = pixel_index % pcanvas_type::width;
  const static int height = pixel_index / pcanvas_type::width;
  typedef canvas_render<pcanvas_type, pixel_desc_index, false> type;
  typedef pcanvas_type convas_type;
  static void draw() {
    std::cout << pixel_type::value;
    canvas_render<pcanvas_type, pixel_desc_index - 1, width + 1 == pcanvas_type::width>::draw();
  }
private:
  const static int dwidth = pcanvas_type::width - width - 1;
  const static int dheight = pcanvas_type::height - height - 1;
  const static bool is_wall = (dwidth == 0 || dheight == 0 || width == 0 || height == 0);
  const static bool is_agent = tarray_is_contain<typename pcanvas_type::game_run_type::agent_tracks, pixel_index>::value;
  const static bool is_reward = pcanvas_type::game_run_type::reward_pixel_index == pixel_index;
  typedef pixel_value<is_wall, is_agent, is_reward> pixel_type;
};

template <typename pcanvas_type, int pixel_desc_index>
class canvas_render<pcanvas_type, pixel_desc_index, true> {
public:
  const static int pixel_index = pcanvas_type::pixel_count - pixel_desc_index;
  const static int width = pixel_index % pcanvas_type::width;
  const static int height = pixel_index / pcanvas_type::width;
  typedef canvas_render<pcanvas_type, pixel_desc_index, true> type;
  typedef pcanvas_type convas_type;
  static void draw() {
    std::cout << std::endl;
    canvas_render<pcanvas_type, pixel_desc_index, false>::draw();
  }
};

template <typename pcanvas_type>
class canvas_render<pcanvas_type, 1, false> {
public:
  const static int pixel_index = pcanvas_type::pixel_count;
  const static int width = pixel_index % pcanvas_type::width;
  const static int height = pixel_index / pcanvas_type::width;
  typedef canvas_render<pcanvas_type, 1, false> type;
  typedef pcanvas_type convas_type;
  static void draw() {
    std::cout << video_pixel<VIDEO_PIXEL_WALL>::value << std::endl;
  }
};

#endif