source constants
sudo apt install sshfs
for u in $users; do
    su -c "mkdir /home/$u/opis" -s /bin/sh $u
    su -c "sshfs $u@$server_ip:/opisy_ksiazek /home/$u/opis" -s  /bin/sh $u
done