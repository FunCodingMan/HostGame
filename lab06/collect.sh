#!/usr/bin/env bash
# usage: ./collect.sh [корневая_директория] > output.txt
set -euo pipefail

ROOT="${1:-.}"

find "$ROOT" -type f \( -name '*.hpp' -o -name '*.cpp' \) -print0 \
  | sort -z \
  | while IFS= read -r -d '' file; do
      # <путь к файлу><название файла> (полный путь на одной строке)
      printf 'Path to file: %s\nCode:\n' "$file"
      # <код>
      cat "$file"
      # разделитель между файлами
      printf '\n'
  done