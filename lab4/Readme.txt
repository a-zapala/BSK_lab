1. Po postawieniu maszyn, należy zmienić ip_servera i ip_clienta w pliku constants.
Następnie przesłać wszystkie pliki na oba serwery.
2. Wykonać na obu maszynach create_users.sh
3. Wykonać na maszynie klienckiej gen_ssh_keys.sh
4. Wykonać na maszynie serwera modify_access_server.sh
5. Wykonać add_alias.sh oraz mount_opisy_ksiazek.sh na maszynie klienckiej.

Miejsca implementacji podpunktów zadania:

1. gen_ssh_keys.sh
2. modify_access_server.sh
3. modify_access_server.sh
4. modify_acces_server.sh
5. należy zalogować się na menadżera na maszynie klienckiej i wykonać pgp --gen-key
6. sign_file.sh
7. mount_opisy_ksiazek.sh
9. add_alias.sh