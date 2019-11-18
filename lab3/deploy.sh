cp -a ./pam.d/. /etc/pam.d

mkdir "katalog_przedmiotowy"
mkdir "katalog_alfabetyczny"
i=0;
max=9;
while [ "$i" -le "$max" ]; do
  j=0;
  mkdir "katalog_przedmiotowy/$i"
  while [ "$j" -le "$max" ]; do
    mkdir "katalog_przedmiotowy/$i/$j"
    j=`expr "$j" + 1`;
  done
  i=`expr "$i" + 1`;
done

i=0;
max=4;
while [ "$i" -le "$max" ]; do
  echo "opis $i" >> "katalog_alfabetyczny/ksiazka_$i"
  i=`expr "$i" + 1`;
done

gcc -o  program main.c  -lpam -lpam_misc
chmod u+s ./program
python3 ../lab1/main.py ../lab1/users_list
addgroup sprzedawca_manager -q
usermod -a -G sprzedawca_manager 1002
