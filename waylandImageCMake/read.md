# to start app at start
cp wayImg.service /etc/systemd/system
systemctl enable wayImg.service

cp startWayImg /usr/bin

# set ip on start
/etc/rc.local

ifconfig eth0 192.168.0.1 netmask 255.255.255.0
