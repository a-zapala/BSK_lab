#!/bin/bash
source constants

for u in $users; do
   useradd -m $u
   # shellcheck disable=SC2046
   usermod --password $(echo pass | openssl passwd -1 -stdin) $u
done

groupadd manager
groupadd sprzedawca

for m in $managers; do
  usermod -a -G manager $m
done

for s in $sellers; do
  usermod -a -G sprzedawca $s
done




