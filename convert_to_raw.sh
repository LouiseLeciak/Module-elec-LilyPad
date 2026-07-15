#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: ./convert_to_raw.sh <input_image>"
  exit 1
fi

INPUT=$1
OUTPUT="${INPUT%.*}.raw"

# -s 480x320 forces the exact screen dimensions
# -pix_fmt rgb565be sets the 16-bit format (Big-Endian)
# -f rawvideo strips all headers so it's pure pixel data
ffmpeg -y -v error -i "$INPUT" -s 320x480 -pix_fmt rgb565le -f rawvideo "$OUTPUT"

echo "Success: $INPUT converted to $OUTPUT"
