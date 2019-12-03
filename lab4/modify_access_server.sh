source constants

sed -i 's/PermitRootLogin yes/PermitRootLogin no/g' /etc/ssh/sshd_config
echo "PasswordAuthentication no" >> /etc/ssh/sshd_config

echo "AllowUsers  Abacka@$client_ip" >> /etc/ssh/sshd_config

for u in $sellers; do
    echo "AllowUsers  $u" >> /etc/ssh/sshd_config
done
service sshd restart

mkdir /opisy_ksiazek
setfacl -m g:sprzedawca:r-x /opisy_ksiazek
setfacl -d -m  g:sprzedawca:r-- /opisy_ksiazek
setfacl -m g:manager:rwx /opisy_ksiazek
setfacl -d -m g:manager:rw- /opisy_ksiazek



