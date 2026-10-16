#!/bin/sh

# Defining variables
REPO="https://github.com/LouiseLeciak/Module-elec-LilyPad/releases/latest/download" # Will always fetch the latest release, it's not pinned to a specific one
PORT="/dev/ttyUSB0"                                                                 # Fallback port
THEME="default"                                                                     # Fallback theme

usage() {
  echo "Usage: $0 [-p PORT] [THEME]" #$0 is the name/path to the script
  echo "  -p PORT   Serial port (default: $PORT)"
  echo "  THEME     default | halloween | candy | bw"
  exit 1
}

# case statements have the following syntax
# case VAR in
# value) command.s ;;
# value) command.s ;;
# esac
#
# `)` delimits the value check and the command.s
# `;;` signals the end of the branch

# getopts is a cmd
# "p:h" is its arg, it specifies the options, `:` means the aforementioned option expects an argument
# opt is the recipient for the current evaluated option letter
while getopts "p:h" opt; do
  case "$opt" in       # Evaluates $opt
  p) PORT="$OPTARG" ;; #If $opt == p
  h | *) usage ;;      # If $opt == h or anything else
  esac
done

# $OPTIND holds the position of the next unprocessed argument, if getopt consumed the first two, it will set OPTIND to 3, and we only need to shift by 2
shift $((OPTIND - 1))

[ $# -gt 1 ] && usage      # $#->number of remaining arguments, -gt is greater, the syntax for the check expects this to be wrapped in `[]`
[ $# -eq 1 ] && THEME="$1" #Ditto, -eq is equal

case "$THEME" in                       # Evaluates $THEME
default) HEX="main.hex" ;;             # If $THEME == default
halloween) HEX="main_halloween.hex" ;; # If $THEME == halloween
candy) HEX="main_candy.hex" ;;         # If $THEME == candy
bw) HEX="main_bw.hex" ;;               # If $THEME == bw
*)                                     # Fallback case
  echo "Unknown theme: $THEME" >&2
  echo "Available themes: default, halloween, candy, bw" >&2
  exit 1
  ;;
esac

echo "Downloading $HEX (theme: $THEME)..."
curl -fLO "$REPO/$HEX" || exit 1 # If curl fails, then exit 1

echo "Flashing $HEX on $PORT..."
avrdude -v -p atmega2560 -c wiring -D -P "$PORT" -b 115200 -U "flash:w:$HEX:i"
rm "$HEX"
