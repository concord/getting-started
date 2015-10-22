#!/bin/bash --login
echo "Killing all concord tasks now..."
concord kill -a &>> /dev/null
sudo service concord stop
sudo service mesos-master stop
sudo service mesos-slave stop

sudo service zookeeper stop
sudo rm -rf /var/lib/zookeeper/version-2/*
zkcli=/usr/share/zookeeper/bin/zkCli.sh
sudo service zookeeper start &>> /dev/null
echo "Cleaning /concord from zookeeper"
echo "rmr /concord" | $zkcli &>> /dev/null
echo "Cleaning /mesos from zookeeper"
echo "rmr /mesos" | $zkcli &>> /dev/null

sudo mkdir -p /var/log/mesos
sudo mkdir -p /var/log/concord
sudo rm -rf /var/log/mesos/*
sudo rm -rf /var/log/concord/*

sudo service mesos-slave start
sudo service mesos-master start
sudo service concord start
