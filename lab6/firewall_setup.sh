sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -F'

sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -t nat -A PREROUTING -p tcp -i eth2 --dport 80 -j DNAT --to-destination 192.12.14.2:8080'
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -A FORWARD -p tcp -d 192.12.14.2 --dport 8080 -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT'
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -A FORWARD -p tcp -s 192.12.14.2 -m state --state ESTABLISHED,RELATED -j ACCEPT'
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -A FORWARD  -j DROP'

sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -A INPUT -p udp -i eth2 --dport openvpn -j ACCEPT'
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'iptables -A INPUT -i eth2 -j DROP'



sshpass -p "mimuw" ssh -t -p 2002 root@localhost 'iptables -A INPUT -m iprange --src-range 192.12.14.2-192.12.14.255 -j DROP'
sshpass -p "mimuw" ssh -t -p 2003 root@localhost 'iptables -A INPUT -m iprange --src-range 192.12.14.2-192.12.14.255 -j DROP'





