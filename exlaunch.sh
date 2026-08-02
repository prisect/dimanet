#!/usr/bin/env bash
# DimaNet example launcher.
#
# Build the examples first with `make examples` (or cmake --build), then run
# this script and pick an example.

set -euo pipefail

BIN_DIR="${1:-build}"

declare -a names=(
    "XOR function using backpropagation"
    "XOR function using random search"
    "Load and run a network saved to disk"
    "IRIS data set using backpropagation"
    "Function approximation (sin(x) + cos(2x))"
)

echo ""
echo " DimaNet example launcher"
echo " ========================"
for i in "${!names[@]}"; do
    printf "  %d - %s\n" $((i + 1)) "${names[$i]}"
done
echo "  0 - quit"

while true; do
    read -r -p "Select an example [0-5]: " choice
    case "$choice" in
        1|2|3|4|5)
            bin="$BIN_DIR/example_$choice"
            if [[ ! -x "$bin" ]]; then
                echo "Not found: $bin (run 'make examples' first)"
                continue
            fi
            "$bin"
            ;;
        0) break ;;
        *)
            echo "Invalid choice. Enter a number between 0 and 5."
            ;;
    esac
done
