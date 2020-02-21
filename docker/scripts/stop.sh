#!/bin/bash -ex

current_dir=`dirname $0`
cd $current_dir

./stop-crailsapp.sh || echo "crailsapp already stopped"
./stop-sidekic.sh   || echo "sidekic already stopped"
./stop-faye.sh      || echo "faye already stopped"
