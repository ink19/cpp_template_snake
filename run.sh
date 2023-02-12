#!/usr/bin/bash

key='a'
echo '1' > cmd.inc
while true;
do
  g++-12 main.cpp -std=c++11
  clear
  if ! ./a.out; then
    break
  fi
  if read -s -n 1 -t 2 input_key; then
    key=${input_key}
  fi
  action=1
  case $key in
    w)
      action=1
      ;;
    s)
      action=2
      ;;
    a)
      action=3
      ;;
    d)
      action=4
      ;;
  esac
  echo ,$action >> cmd.inc 
done