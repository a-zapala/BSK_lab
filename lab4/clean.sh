#!/bin/bash
source constants

for u in $users; do
    userdel -r $u
done

groupdel sprzedawca
groupdel manager