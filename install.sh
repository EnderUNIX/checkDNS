#!/bin/sh

echo
echo ">>> Starting CheckDNS installation script <<<"
sleep 2
echo
echo ">>>>>> I hope that you read the INSTALL and README file ! <<<<<<"
sleep 3
echo
echo ">>> If No, Please break and Read ! <<<"
sleep 4
echo
echo "Hmmms, You did not break yet... I think you read it..."
sleep 5
echo

echo
echo "Then, Compiling source..."
sleep 1
make
echo

echo
echo "And, Installing CheckDNS Binary..."
sleep 1
cp -f checkdns /usr/local/bin/checkdns
chown root /usr/local/bin/checkdns
chmod 700 /usr/local/bin/checkdns
echo

echo
echo "Making clean..."
sleep 1
make clean
echo

echo
echo "Copying configuration files..."
sleep 1
if ( test -f /usr/local/etc/checkdns.conf ); then
	DATE=`date '+%Y-%m-%d'`
	cp -pf /usr/local/etc/checkdns.conf /usr/local/etc/checkdns.conf-$DATE
	cp -pf checkdns.conf-dist /usr/local/etc/checkdns.conf
	echo "checkdns.conf backed up to /usr/local/etc/checkdns.conf-$DATE"
else
	cp -pf checkdns.conf-dist /usr/local/etc/checkdns.conf
fi
echo

echo
echo "Preparing /usr/local/share/checkdns directory..."
mkdir -p /usr/local/share/checkdns/lang
cp -f README THANKS TODO AUTHORS COPYING INSTALL ChangeLog uninstall.sh /usr/local/share/checkdns
cp -f lang/* /usr/local/share/checkdns/lang
echo

sleep 2
echo
echo "  --------------------------------------------------------------------"
echo " |            >>> ATTENTION PLEASE <<<                                |"
echo " |                                                                    |"
echo " |   Please don't forget these :                                      |"
echo " |         - editing /usr/local/etc/checkdns.conf file !              |"
echo " |         - copying checkdns.css file to Html Output Directory !     |"
echo " |                                                                    |"
echo "  --------------------------------------------------------------------"
echo

sleep 4
echo
echo "  --------------------------------------------------------------------"
echo " |                                                                    |"
echo " | uninstallation script has been stored to /usr/local/share/checkdns |"
echo " | so you can use it to uninstall checkdns in the future.             |"
echo " |                                                                    |"
echo " | (C), EnderUNIX Software Development Team @ Turkey                  |"
echo " |                http://www.enderunix.org/checkdns/                  |"
echo " |                                                                    |"
echo " |                                    Istanbul, 2005                  |"
echo "  --------------------------------------------------------------------"
echo
