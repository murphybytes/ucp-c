#!/bin/bash

#################################################
# One time setup for tests.  
#
# This script will set up a dummy account called
# 'dummy' and set the current users id_rsa.pub
# key in the dummy users .ssh/known_hosts.  The
# script will also set up a directory in dummy
# called ~/.ucp that will contain AES session 
# keys used in tests
#
################################################

if [ "$(whoami)" != "root" ]; then
    echo "Script Failed. You need to use sudo to run this script"
    exit 1 
fi

if [ ! -x "src/ucp" ]; then
    echo "Script Failed. Run build.sh to generate binaries for test."
    exit 1
fi 



if [ -z $DUMMY_ACCOUNT ]; then   
    export DUMMY_ACCOUNT=dummy    
fi

echo "Dummy account ${DUMMY_ACCOUNT}"

echo "Backing up bashrc"
cp ~/.bashrc ~/bashrc.$(date +%Y%m%d).backup

grep -q DUMMY_ACCOUNT= ~/.bashrc || echo "export DUMMY_ACCOUNT=${DUMMY_ACCOUNT}" >> ~/.bashrc


( cat /etc/passwd | grep -q ${DUMMY_ACCOUNT} ) || userdel --remove ${DUMMY_ACCOUNT}

useradd --create-home ${DUMMY_ACCOUNT}
mkdir -p ~/.ucp
mkdir -p /home/${DUMMY_ACCOUNT}/.ssh 
mkdir -p /home/${DUMMY_ACCOUNT}/.ucp

chown -R ${DUMMY_ACCOUNT}:${DUMMY_ACCOUNT} /home/${DUMMY_ACCOUNT}

cat ~/.ssh/id_rsa.pub >> /home/${DUMMY_ACCOUNT}/.ssh/authorized_keys 


if [ ! -h /home/${DUMMY_ACCOUNT}/ucp ]; then
    ln -s "$(pwd)/src/ucp" /home/${DUMMY_ACCOUNT}/ucp
fi