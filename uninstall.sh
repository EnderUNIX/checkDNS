#!/bin/sh

echo
echo ">>> Starting CheckDNS uninstallation script <<<"
sleep 2
echo
echo

echo "Removing CheckDNS Binary..."
sleep 1
if [ -x /usr/local/bin/checkdns ]; then
  rm -rf /usr/local/bin/checkdns
fi
sleep 1
echo
echo

echo "Please remove conf file manually."
echo "/usr/local/etc/checkdns.*"
echo
