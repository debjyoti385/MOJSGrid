#!/bin/bash
FILES=../schedule_*.out
echo "energy analysis..."
echo "FILE			START		END		UPTIME		UTILIZATION"
for f in $FILES
do
  ./energy $f
done
for f in $FILES
do
  echo "Check analysis..."
  ./check $f
done
