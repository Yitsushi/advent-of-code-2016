#!/bin/sh

find . -name "main.cpp" -print | \
  xargs -n1 dirname | \
  xargs -n1 dirname | \
  xargs -n1 basename | \
  uniq -c
