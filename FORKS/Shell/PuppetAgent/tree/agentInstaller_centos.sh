#!/bin/bash
# Puppet Agent Install on Debian Wheezy
# Author: John McCarthy
# <http://www.midactstech.blogspot.com> <https://www.github.com/Midacts>
# Date: 3rd of March, 2014
# Version 1.1
#
# To God only wise, be glory through Jesus Christ forever. Amen.
# Romans 16:27, I Corinthians 15:1-4
#---------------------------------------------------------------
######## FUNCTIONS ########
function setHostname()
{
# /etc/hosts
	IP=`hostname -I`
	Hostname=`hostname`
	FQDN=`hostname -f`
	echo -e "127.0.0.1	localhost		localhosts.localdomain	$FQDN\n$IP	$FQDN	$Hostname" > /etc/hosts
	echo -e '\e[1;37;42mThe Puppet Agent'\''s "/etc/hosts" file has been updated successfully!\e[0m'
}
function setMasterHostname()
{
# /etc/hosts
	echo -e '\e[33mWhat is the IP of your Puppet Master?\e[0m'
	read puppetIP
	echo -e '\e[33mWhat is your Puppet Master'\''s FQDN?\e[0m'
	read puppetFQDN
	echo -e '\e[33mWhat is your Puppet Master'\''s hostname?\e[0m'
	read puppetHostname
	echo -e "$puppetIP	$puppetFQDN	$puppetHostname	puppet" >> /etc/hosts
	echo -e '\e[1;37;42mSuccessfully added the Puppet Master to your /etc/hosts file!\e[0m'
}
function puppetRepos()
{
	echo -e '\e[01;34m+++ Getting repositories...\e[0m'
	rpm -ivh http://yum.puppetlabs.com/puppetlabs-release-el-6.noarch.rpm
	echo -e '\e[1;37;42mThe Latest Puppet Repos have been acquired!\e[0m'
}
function installPuppet()
{
	echo -e '\e[01;34m+++ Installing Puppet Agent...\e[0m'
	yum install puppet -y
	echo -e '\e[1;37;42mThe Puppet Agent has been installed!\e[0m'
}
function editPuppet()
{
	if [[ -z "$puppetFQDN" ]]
	then
        	echo -e '\e[33mWhat is your \e[01;33mPuppet Master'\''s\e[0m \e[33mFQDN?\e[0m'
        	read puppetFQDN
	fi
	if [[ -z "$FQDN" ]]
	then
        	echo -e '\e[33mWhat is \e[01;33mTHIS Puppet AGENT'\''s\e[0m \e[33mFQDN?\e[0m'
        	read FQDN
	fi
	echo -e '\e[01;34m+++ Editing "/etc/puppet/puppet.conf"...\e[0m'
	echo
cat <<EOA> /etc/puppet/puppet.conf
[main]
logdir=/var/log/puppet
vardir=/var/lib/puppet
ssldir=/var/lib/puppet/ssl
rundir=/var/run/puppet
factpath=\$vardir/lib/facter
server = $puppetFQDN
report = true
pluginsync = true
certname = $FQDN

[master]
ssl_client_header = SSL_CLIENT_S_DN
ssl_client_verify_header = SSL_CLIENT_VERIFY
EOA

	echo -e '\e[1;37;42mThe "/etc/puppet/puppet.conf" file has been successfully edited!\e[0m'
}
function enablePuppet()
{
	echo -e '\e[01;34m+++ Enabling Puppet Service...\e[0m'
	puppet resource service puppet ensure=running enable=true

	# Configures puppet to start
	sed -i 's/START=no/START=yes/g' /etc/default/puppet
	echo -e '\e[1;37;42mThe Puppet Service has been initiated!\e[0m'
}
function editCrontab()
{
	echo -e '\e[01;34m+++ Editing the Crontab file...\e[0m'
	echo '0,30 * * * * puppet agent --test' >> /var/spool/cron/root
	echo
	echo -e '\e[1;37;42mThe Crontab file has been successfully edited!\e[0m'
}
function doAll()
{
	echo
	echo -e '\e[33m=== Set Puppet Agent'\''s Hostname for Puppet Runs ? [RECOMMENDED] (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		setHostname
	fi

	echo
	echo -e '\e[33m=== Set Puppet Master'\''s Hostname for Puppet Runs ? [RECOMMENDED] (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		setMasterHostname
	fi

	echo
	echo -e '\e[33m=== Get Latest Puppet Repos ? (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		puppetRepos
	fi

	echo
	echo -e '\e[33m=== Install Puppet Agent ? (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		installPuppet
	fi

	echo
	echo -e '\e[33m=== Edit Puppet Configuration File ? (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		editPuppet
	fi

	echo
	echo -e '\e[33m=== Enable Puppet Agent Service ? (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		enablePuppet
	fi

	echo
	echo -e '\e[33m=== Enable Crontab file to auto-schedule Puppet Runs ? (y/n)\e[0m'
	read yesno
	if [ "$yesno" = "y" ]; then
		editCrontab
	fi
	echo
	echo
	echo -e '          \e[01;37;42mWell done! you have completed your Puppet Agent Installation!\e[0m'
	echo
	echo
	echo -e '\e[01;37mCheckout similar material at "midactstech.blogspot.com" and "github.com/Midacts" \e[0m'
	echo
	echo -e '                            \e[01;37m########################\e[0m'
	echo -e '                            \e[01;37m#\e[0m \e[31mI Corinthians 15:1-4\e[0m \e[01;37m#\e[0m'
	echo -e '                            \e[01;37m########################\e[0m'
	echo
	echo
	exit 0
}
# Check privileges
[ $(whoami) == "root" ] || die "You need to run this script as root."
# Welcome to the script
clear
echo
echo
echo -e '                \e[01;37;42mWelcome to Midacts Mystery'\''s Puppet Agent Installer!\e[0m'
echo
echo -e '                    \e[00;31;40m!!! Do not forget to edit your DNS settings !!!\e[0m'
echo
####### MENU #######
case "$go" in
	hostname)
		setHostname ;;
	masterHostname)
		setMasterHostname ;;
	repos)
		puppetRepos ;;
	puppet)
		installPuppet ;;
	editPuppet)
		editPuppet ;;
	enablePuppet)
		enablePuppet ;;
	crontab)
		editCrontab ;;
	* )
		doAll ;;
esac

exit 0
