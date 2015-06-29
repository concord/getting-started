#!/bin/bash --login
set -x

TASK_DIRECTORY=$(find /var/lib/mesos/slaves/ -iname "$1")/

if [[ ! -d ${TASK_DIRECTORY} ]]; then
    echo "Task directory in /var/lib/mesos/slaves not found"
    exit 1
fi

echo "Tailing stdout & err"

tail -f ${TASK_DIRECTORY}/../std*
