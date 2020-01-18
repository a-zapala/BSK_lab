sshpass -p "mimuw" scp -P 2001 ./conf/firewall.conf root@localhost:/etc/network/interfaces
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'echo "net.ipv4.ip_forward = 1" >> /etc/sysctl.conf'
sshpass -p "mimuw" ssh -t -p 2001 root@localhost 'reboot'

sshpass -p "mimuw" ssh -t -p 2002 root@localhost 'yes | apt-get install apache2'
sshpass -p "mimuw" scp -P 2002 ./conf/ports.conf root@localhost:/etc/apache2/ports.conf
sshpass -p "mimuw" scp -P 2002 ./conf/index.html root@localhost:/var/www/html/index.html
sshpass -p "mimuw" ssh -t -p 2002 root@localhost 'servive apache2 restart'
sshpass -p "mimuw" scp -P 2002 ./conf/www.conf root@localhost:/etc/network/interfaces
sshpass -p "mimuw" ssh -t -p 2002 root@localhost 'reboot'

sshpass -p "mimuw" scp -P 2003 ./conf/radegast.conf root@localhost:/etc/network/interfaces
sshpass -p "mimuw" ssh -t -p 2003 root@localhost 'reboot'

sshpass -p "mimuw" scp -P 2004 ./conf/manager.conf root@localhost:/etc/network/interfaces
sshpass -p "mimuw" ssh -t -p 2004 root@localhost 'reboot'