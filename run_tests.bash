#!/usr/bin/env bash

failed=0
total=0

for t in backend/build/[A-Z]*Test; do
    if [ -x "$t" ]; then
        total=$((total+1))
        echo "Running $t"
        if ! "$t"; then
            failed=$((failed+1))
        fi
    fi
done

passed=$((total-failed))

if [ "$total" -gt 0 ]; then
    percent=$((100*failed/total))
else
    percent=0
fi

echo "-----------------------------"
echo "Total test binaries: $total"
echo "Passed: $passed"
echo "Failed: $failed"
echo "Failure rate: $percent%"

if [ "$failed" -ne 0 ]; then
    exit 1
fi
