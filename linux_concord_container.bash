#!/bin/bash --login
#set -ex
if [ $OSTYPE == "linux-gnu" ]; then
    echo "Linux Detected... Determining hardware specs..."
else
    echo "This is only for linux boxes"
    exit 1
fi

function -h {
    echo "linux_concord_container.bash [--update]";
}
function --help {
    -h
}
function --update {
    echo "Updating docker... $update_docker"
    sudo docker pull concord/client_devbox
}

echo "Starting container $run_docker"
exec sudo docker run --privileged=true -t -i -v $(pwd):/workspace -p 5050:5050 -p 5051:5051 -p 9000:9000 concord/client_devbox
