#!/bin/bash

ORTERUN=orterun
#ORTERUN_ARGS=" --report-bindings --map-by node --bind-to core "
ORTERUN_ARGS=" --report-bindings --map-by node --bind-to socket "
ORTERUN_NPROCS="--np 4 "

EXE=./tst-show-affinity
EXE_ARGS=
#EXE_ARGS="-v"


# Abstract OMP_PLACES: threads, cores and sockets 
#export OMP_DISPLAY_ENV=true

#---------------------
# No places, no binding
OMP_ARGS="env OMP_PROC_BIND=false OMP_NUM_THREADS=8 "
cmd="$OMP_ARGS $ORTERUN $ORTERUN_ARGS $ORTERUN_NPROCS $EXE $EXE_ARGS"

echo ""
echo "########################################################"
echo "# CMD: $cmd"
echo "# NOTE: No places, no binding"
echo "########################################################"
$cmd 


#---------------------
# No binding, with places
OMP_ARGS="env OMP_PROC_BIND=false OMP_NUM_THREADS=8 OMP_PLACES=cores "
cmd="$OMP_ARGS $ORTERUN $ORTERUN_ARGS $ORTERUN_NPROCS $EXE $EXE_ARGS"

echo ""
echo "########################################################"
echo "# CMD: $cmd"
echo "# NOTE: No binding, with places"
echo "########################################################"
$cmd 


#---------------------
# With binding, no places
OMP_ARGS="env OMP_PROC_BIND=true  OMP_NUM_THREADS=8 "
cmd="$OMP_ARGS $ORTERUN $ORTERUN_ARGS $ORTERUN_NPROCS $EXE $EXE_ARGS"

echo ""
echo "########################################################"
echo "# CMD: $cmd"
echo "# NOTE: With binding, no places"
echo "########################################################"
$cmd 


#---------------------
# With Binding, with places

OMP_ARGS="env OMP_PROC_BIND=true  OMP_NUM_THREADS=8 OMP_PLACES=cores  "
cmd="$OMP_ARGS $ORTERUN $ORTERUN_ARGS $ORTERUN_NPROCS $EXE $EXE_ARGS"

echo ""
echo "########################################################"
echo "# CMD: $cmd"
echo "# NOTE: With Binding, with places"
echo "########################################################"
$cmd 

#---------------------
# With Binding, with CUSTOM places
#       (below are the same syntax, diff format)
# TODO: Not sure this is doing same PLACES as above,
#       also gets warning msg,
#         libgomp: Number of places reduced from 4 to 1 because some places didn't
#         contain any usable logical CPUs

#OMP_ARGS="env OMP_PROC_BIND=true  OMP_NUM_THREADS=8 OMP_PLACES={1:8},{13:8},{25:8},{37:8}"
OMP_ARGS="env OMP_PROC_BIND=true  OMP_NUM_THREADS=8 OMP_PLACES={1:8}:4:12"
#TJN-DBG: OMP_ARGS="env OMP_NUM_THREADS=8"
cmd="$OMP_ARGS $ORTERUN $ORTERUN_ARGS $ORTERUN_NPROCS $EXE $EXE_ARGS"

echo ""
echo "########################################################"
echo "# CMD: $cmd"
echo "# NOTE: With Binding, with places"
echo "########################################################"
$cmd
