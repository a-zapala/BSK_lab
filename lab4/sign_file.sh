gpg --sign $1
u="$USER"
scp $1.gpg $u@$2:/opisy_ksiazek

