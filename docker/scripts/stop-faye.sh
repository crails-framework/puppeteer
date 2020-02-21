#!/bin/bash -ex

current_dir=`dirname $0`
cd $current_dir
source ../env

pidfile=$HOME/faye.pid

if [[ -e "$pidfile" ]] ; then
  pid=`cat "$pidfile"`
  echo "Stopping faye with pid $pid"
  kill -INT $pid
  while [[ $? -ne 0 ]] ; do
    sleep 1
    kill -= $pid
  done
  rm -f "$pidfile"
else
  echo "Cannot stop: file $pidfile not found"
  exit -1
fi

