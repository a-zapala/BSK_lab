#!/bin/bash
source constants

for u in $users; do
    echo  "Host radegast
           HostName $server_ip
           User $u" >> /home/$u/.ssh/config
done
