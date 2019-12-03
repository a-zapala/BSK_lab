#!/bin/bash
source constants

apt install sshpass
echo "pass" >> .password
ip=$server_ip
for u in $users; do
    sudo su -c "ssh-keygen -t rsa -N '' -f ~/.ssh/id_rsa" -s /bin/sh $u
    sudo su -c "ssh-keyscan -H $ip >> ~/.ssh/known_hosts" -s /bin/sh $u
    sudo su -c "sshpass -f .password ssh-copy-id $u@$ip" -s /bin/sh $u
done


