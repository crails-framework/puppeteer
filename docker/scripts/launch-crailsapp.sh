#!/bin/bash -ex

current_dir=`dirname $0`
cd $current_dir
source ../env
logfile_base=/var/log/$INSTANCE_NAME/server
logfile="$logfile_base"_`date +"%Y%m%d_%H%M"`

find "$logfile_base"_* -mtime +7 -exec rm {} \; || echo "eyup"

bin/server -h 0.0.0.0 -p 3042 --pidfile $HOME/crailsapp.pid \
  > $logfile 2>&1 \
  &

sleep 2

if [[ $1 == "--wait" ]] ; then
  wait `cat $HOME/crailsapp.pid`
fi
