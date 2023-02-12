#include <iostream>

#ifndef MAIN_ARRAY
#define MAIN_ARRAY

template <int ...data>
class tarray {};

template <typename T, int index>
class tarray_get;

template <>
class tarray<> {
public:
  typedef tarray<> type;
  const static int size = 0;
};

template <int data>
class tarray<data> {
public:
  typedef tarray<data> type;
  typedef tarray<> pop_front_type;
  const static int front_value = data;
  const static int size = 1;
  const static int back_value = data;
};

template <int data_1, int ...data>
class tarray<data_1, data...> {
public:
  typedef tarray<data_1, data...> type;
  typedef tarray<data...> pop_front_type;
  const static int front_value = data_1;
  const static int back_value = tarray<data...>::back_value;
  const static int size = sizeof...(data) + 1;
};

// 添加
template <typename T, int data>
class tarray_push_back;

template <int ...data, int data_back>
class tarray_push_back<tarray<data...>, data_back> {
public:
  typedef tarray<data..., data_back> type;
};


// 获得T的index数字
template <typename T, int index>
class tarray_get {
public:
  const static int value = tarray_get<typename T::pop_front_type, index - 1>::value;
};


template <typename T>
class tarray_get<T, 0> {
public:
  const static int value = T::front_value;
};

// 是否存在
template <typename T, int data>
class tarray_is_contain {
private:
  const static bool first_value = T::front_value == data;
  const static bool other_value = tarray_is_contain<typename T::pop_front_type, data>::value;
public:
  const static bool value = first_value || other_value;
};

template <int data>
class tarray_is_contain<tarray<>, data> {
public:
  const static bool value = false;
};

#endif
