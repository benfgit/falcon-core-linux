#!/bin/sh

if ! type tshark >/dev/null 2>&1; then
  echo "Error: tshark is not installed. Please install it first." >&2
  exit 1
fi

if [ -z "$1" ]; then
  echo "Please specify the UDP port: $0 <udp_port>" >&2
  exit 1
fi

PORT="$1"

echo "Monitoring UDP port $PORT on interface lo..."

sudo tshark -l -i lo -f "udp port $PORT" -T fields -e udp.payload 2>/dev/null \
| while IFS= read -r h; do
  [ -z "$h" ] && continue
  echo
  echo "$h" | tr -d ':' | xxd -r -p | xxd -g1 -c16
  echo
done
