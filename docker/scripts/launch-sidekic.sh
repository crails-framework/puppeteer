#!/bin/bash -ex

current_dir=`dirname $0`
cd $current_dir
source ../env
logfile_base=/var/log/$INSTANCE_NAME/sidekic
logfile="$logfile_base"_`date +"%Y%m%d_%H%M"`

find "$logfile_base"_* -mtime +7 -exec rm {} \; || echo "eyup"

bin/tasks/sidekic/task --pidfile $HOME/sidekic.pid \
  > $logfile 2>&1 \
  &

sleep 2
