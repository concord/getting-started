#!/bin/bash --login
echo "Killing all concord tasks now..."
concord kill -a &>> /dev/null
sudo service concord stop
sudo service mesos-master stop
sudo service mesos-slave stop

echo "Killing all tracing tasks now..."
sudo service kafka stop

sudo service web stop
sudo service query stop
sudo service collector stop
sudo service cassandra stop


sudo service zookeeper stop
sudo rm -rf /var/lib/zookeeper/version-2/*
zkcli=/usr/share/zookeeper/bin/zkCli.sh
sudo service zookeeper start &>> /dev/null
echo "Cleaning /concord from zookeeper"
echo "rmr /concord" | $zkcli &>> /dev/null
echo "Cleaning /mesos from zookeeper"
echo "rmr /mesos" | $zkcli &>> /dev/null
echo "Cleaning /kafka from zookeeper"
echo "rmr /kafka" | $zkcli &>> /dev/null

sudo mkdir -p /var/log/mesos
sudo mkdir -p /var/log/concord
sudo rm -rf /var/log/mesos/*
sudo rm -rf /var/log/concord/*

echo "Restarting all services"
sudo service mesos-slave start
sudo service mesos-master start
sudo service concord start

sudo service kafka start
sudo service cassandra start
sudo service collector start
sudo service query start
sudo service web start
