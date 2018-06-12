#!/bin/bash

PKG_OK=$(dpkg-query -W --showformat='${Status}\n' squid3|grep "install ok installed")

if [ "" == "$PKG_OK" ]; then
  sudo apt-get --force-yes --yes install squid3;
fi

mkdir -p /tmp/squid-redirect;

make &> /dev/null;

cp redirections.json /tmp/squid-redirect/redirections.json;
cp src/main /tmp/squid-redirect/main;

make clean &> /dev/null;