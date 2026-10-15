#!/bin/sh
# Downloads and flashes a themed firmware from the latest release.
#
# Usage: ./update.sh [-p PORT] [THEME]
#   -p PORT   Serial port of the board (default: /dev/cu.usbserial-110)
#   THEME     default | halloween | candy | bw  (default: default)

REPO="https://github.com/LouiseLeciak/Module-elec-LilyPad/releases/latest/download"
PORT="/dev/ttyUSB0"
THEME="default"

usage() {
  echo "Usage: $0 [-p PORT] [THEME]" >&2
  echo "  -p PORT   Serial port (default: $PORT)" >&2
  echo "  THEME     default | halloween | candy | bw" >&2
  exit 1
}

while getopts "p:h" opt; do
  case "$opt" in
  p) PORT="$OPTARG" ;;
  h | *) usage ;;
  esac
done
shift $((OPTIND - 1))

[ $# -gt 1 ] && usage
[ $# -eq 1 ] && THEME="$1"

case "$THEME" in
default) HEX="main.hex" ;;
halloween) HEX="main_halloween.hex" ;;
candy) HEX="main_candy.hex" ;;
bw) HEX="main_bw.hex" ;;
*)
  echo "Unknown theme: $THEME" >&2
  echo "Available themes: default, halloween, candy, bw" >&2
  exit 1
  ;;
esac

echo "Downloading $HEX (theme: $THEME)..."
curl -fLO "$REPO/$HEX" || exit 1

echo "Flashing $HEX on $PORT..."
avrdude -v -p atmega2560 -c wiring -D -P "$PORT" -b 115200 -U "flash:w:$HEX:i"
rm $HEX
