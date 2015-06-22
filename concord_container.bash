#!/bin/bash

BOOT2DOCKER=0
NPROCS=1
MEM=4096

if [ $OSTYPE == "linux-gnu" ]; then
  echo "Linux Detected... Determining hardware specs..."
  NPROCS=$(grep -c ^processor /proc/cpuinfo)
  MEM=$(grep MemTotal /proc/meminfo | awk '{print $2}')
elif [[ $OSTYPE =~ darwin.* ]]; then
  echo "Mac OS X Detected... Determining hardware specs..."
  NPROCS=$(sysctl -n hw.ncpu)
  MEM=$(sysctl -n hw.memsize) 
  MEM=`expr $MEM / 1024`
fi

# 1/2 procs
USABLE_PROCS=`expr $NPROCS / 2`
# 3/4 memory
USABLE_MEM=`expr $MEM / 1024 / 4 \* 3`

if [ $(command -v boot2docker) ]; then
  BOOT2DOCKER=1

  echo "Checking boot2docker status..."
  # check to see if there is a box (status code 0 == box exists)
  boot2docker status > /dev/null 2>&1

  if [ $? -ne 1 ]; then
    echo "Checking boot2docker memory is sufficient (${USABLE_MEM}MB required)..."
    # if there is a box already, ensure it's configured
    boot2docker_memory=$(boot2docker config | grep Memory | awk '{print $3}')

    if [ $(expr $boot2docker_memory \< $USABLE_MEM) -ne 0 ]; then
      echo "Insufficient memory, reinitializing boot2docker machine..."
      echo "(Note: This is currently deprecated due to unreliable results from boot2docker config)"
      # if memory is too low, delete and reinit
      # boot2docker delete
      # boot2docker init -m $USABLE_MEM > /dev/null 2>&1
    fi
  else
    echo "Initializing new boot2docker virtual machine with ${USABLE_MEM}MB memory..."
    # if there isn't a box, init it
    boot2docker init -m $USABLE_MEM > /dev/null 2>&1
  fi

  # start the box (idempotent)
  echo "Starting boot2docker virtualbox..."
  boot2docker start > /dev/null 2>&1
  # set environment
  echo "Initializing Docker environment..."
  eval `boot2docker shellinit`
fi

CMD="docker run -t -i -v $(pwd):/workspace -p 5050:5050 -p 5051:5051 -p 9000:9000"

if [ $BOOT2DOCKER -ne 0 ]; then
  # boot2docker systems need to share vbox ip to container
  boot2docker_ip=$(boot2docker ip)
  echo "The boot2docker IP address is $boot2docker_ip"
  CMD="$CMD -e \"VBOX_HOST=$boot2docker_ip\""
else
  # linux systems need sudo
  CMD="sudo $CMD"
fi

CMD="$CMD concord/client_devbox"

echo "Running $CMD"

exec $CMD

