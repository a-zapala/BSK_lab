vboxmanage import ./vm/vm_root.ova --vsys 0 --vmname "firewall"
vboxmanage snapshot firewall take firewall-snap
vboxmanage clonevm firewall --name="www" --register --mode=machine --snapshot=firewall-snap
vboxmanage clonevm firewall --name="radegast" --register --mode=machine --snapshot=firewall-snap
vboxmanage clonevm firewall --name="manager" --register --mode=machine --snapshot=firewall-snap

vboxmanage modifyvm "firewall" --nic2 intnet
vboxmanage modifyvm "firewall" --nicpromisc2 allow-all

vboxmanage modifyvm "firewall" --nic3 intnet
vboxmanage modifyvm "firewall" --nicpromisc3 allow-all
vboxmanage modifyvm "firewall" --intnet3 "siknet2"

vboxmanage modifyvm "www" --nic2 intnet
vboxmanage modifyvm "www" --nicpromisc2 allow-all

vboxmanage modifyvm "radegast" --nic2 intnet
vboxmanage modifyvm "radegast" --nicpromisc2 allow-all

vboxmanage modifyvm "manager" --nic2 intnet
vboxmanage modifyvm "manager" --intnet2 "siknet2"

VBoxManage modifyvm "firewall" --natpf1 "guestssh,tcp,,2001,,22"
VBoxManage modifyvm "www" --natpf1 "guestssh,tcp,,2002,,22"
VBoxManage modifyvm "radegast" --natpf1 "guestssh,tcp,,2003,,22"
VBoxManage modifyvm "manager" --natpf1 "guestssh,tcp,,2004,,22"

vboxheadless -s firewall &>/dev/null &
vboxheadless -s www &>/dev/null &
vboxheadless -s radegast &>/dev/null &
vboxheadless -s manager &>/dev/null &
