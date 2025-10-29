sudo tcpdump -i lo udp port 5000 -l -n | pv -l > /dev/null\
