# C_Network_Packet_Sniffer_on_Linux
Observation:
1. Using raw sockets to capture packets on WSL has its limitations, and you can only capture a few certain types of packets. I figured this out because the Ethernet headers on all the packets would be 00 00 00 00 00...00 08 00, so the source and destination MAC addresses were always 00..00, and then it would be 08 00 to show the protocol (IP). So I decided to test out my code in a Virtual Box Ubuntu enviroment, and the packets being captured were very different, actually showing proper MAC addresses.
Conclusion:
1. I will complete this sniffer using raw sockets, but will then create another file which will use Wireshark packet capture, and then analyze the .pcap file in c.
