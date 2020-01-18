vboxmanage controlvm "firewall" poweroff
vboxmanage controlvm "www" poweroff
vboxmanage controlvm "radegast" poweroff
vboxmanage controlvm "manager" poweroff

vboxmanage unregistervm --delete "firewall"
vboxmanage unregistervm --delete "www"
vboxmanage unregistervm --delete "radegast"
vboxmanage unregistervm --delete "manager"
