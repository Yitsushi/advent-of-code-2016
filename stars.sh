#!/bin/sh

curl -s http://adventofcode.com/2016/stats | \
  grep stats-both | \
  sed -Ee 's/.*<a[^>]*> *([0-9]+) *<s[^>]*> *([0-9]+)<\/span> *<s[^>]*> *([0-9]+).*/\1 \2 \3/g' | \
  awk '
    BEGIN { hard_day=0; hard_percentage=100; }
    END {
      printf("\nThe hardest day was Day-%d, only %.2f%% acquired 2 stars.\n", hard_day, hard_percentage)
    }
    {
      percentage = (1-$3/$2)*100;
      printf("%d: %d/%d => %.2f%% finished both\n", $1, $3, $2, percentage);
      if (hard_percentage > percentage) { hard_percentage=percentage; hard_day=$1; }
    }'
