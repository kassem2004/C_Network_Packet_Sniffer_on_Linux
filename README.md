# C_Network_Packet_Sniffer_on_Linux
Observation:
1. Using raw sockets to capture packets on WSL has its limitations, and you can only capture a few certain types of packets. I figured this out because the Ethernet headers on all the packets would be 00 00 00 00 00...00 08 00, so the source and destination MAC addresses were always 00..00, and then it would be 08 00 to show the protocol (IP). So I decided to test out my code in a Virtual Box Ubuntu enviroment, and the packets being captured were very different, actually showing proper MAC addresses.

2. After adding the IP, TCp and UDP headers, I realized that these packets are all just loopback packets using 127.0.0.1 IP address, which now makes sense of the fact that the Ethernet headers always started with 00 00 00...00 00 00. I believe this is once again due to WSL's limitations.

3. Also, when I went to use tcphdr, for some reason it was giving me an error, which I tried looking up but got nothing for, and I also think this is due to the WSL limitations, so I ended up making my own TCP Header Struct.
