sshpass -p "mimuw" scp -P 2001 -r ./certifications/* root@localhost:~/
sshpass -p "mimuw" scp -P 2004 -r ./certifications/* root@localhost:~/
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'openvpn --dev tun --tls-server --ifconfig 10.1.0.0 10.2.0.0 --ca ca.crt --cert vpn-server.crt --key vpn-server.key --dh dh2048.pem --daemon --log-append /var/log/openvpn.log'
sshpass -p "mimuw" ssh -t -p 2004 root@localhost 'openvpn --dev tun --tls-client --ifconfig 10.2.0.0 10.1.0.0 --ca ca.crt --cert vpn-client.crt --key vpn-client.key --remote 172.16.0.1 --daemon'