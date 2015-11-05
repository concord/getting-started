#Word Count in Scala
===================
Get Vagrant up and running from parent dir:
	
	$ ./bootstrap_vagrant.sh
  	$ vagrant ssh
  	...
  	vagrant@vagrant-ubuntu-trusty-64:~/$ cd /vagrant/scala/


Run pre-package script to build JAR:

  	$ ./pre_package_concord.bash

Open deployment directory and start Concord operators

	$ cd target/concord/
	