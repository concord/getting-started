#!/bin/bash --login

DOCKERMACHINE=0
NPROCS=1
MEM=2048

if [ $OSTYPE == "linux-gnu" ]; then
  NPROCS=$(grep -c ^processor /proc/cpuinfo)
  MEM=$(grep MemTotal /proc/meminfo | awk '{print $2}')
elif [ $OSTYPE == "darwin" ]; then
  NPROCS=$(sysctl -n hw.ncpu)
  MEM=$(sysctl -n hw.memsize) 
  MEM=`expr $MEM / 1024`
fi

# 1/2 procs
USABLE_PROCS=`expr $NPROCS / 2`
# 3/4 memory
USABLE_MEM=`expr $MEM / 1024 / 4 \* 3`

if [ $(command -v docker-machine) ]; then
  DOCKERMACHINE=1
  if [ $(docker-machine ls | grep -c concord-dev) == 0 ]; then
    echo "Did not find Docker machine concord-dev. Creating machine with $USABLE_MEM mb memory..."
    docker-machine create -d virtualbox --virtualbox-memory "$USABLE_MEM" --virtualbox-boot2docker-url "https://github.com/SvenDowideit/boot2docker/releases/download/x64rc1/boot2docker.iso" concord-dev
  else
    if [ $(docker-machine ls | grep -cP 'concord-dev.+Running') == 0 ]; then
      echo "Docker machine concord-dev not running. Starting..."
      docker-machine start concord-dev
    fi
  fi
  echo "Ensuring machine environment is set..."
  eval "$(docker-machine env concord-dev)"
fi

CMD="docker run -t -i -v $(pwd):/workspace -p 5050:5050 -p 5051:5051 -p 9000:9000"

if [ $DOCKERMACHINE -ne 0 ]; then
  DOCKER_IP=$(docker-machine ip concord-dev)
  CMD="$CMD -e \"VBOX_HOST=$DOCKER_IP\""
else
  CMD="sudo $CMD"
fi

CMD="$CMD concord/client_devbox"

echo "Running $CMD"

# https://github.com/docker/machine/issues/641
# last two comments

# exec $CMD

