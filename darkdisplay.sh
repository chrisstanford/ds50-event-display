#!/bin/bash

# Usage info                                                                                                                 
show_help() {
cat <<EOF
Usage: ${0} [-h] [-t] [-o] [-r RUN] [-e EVENT] [-c CONSEC]

    -h          display this help and exit
    -t          enable tpc
    -o          enable od
    -r RUN      desired run id
    -e EVENT    desired event id [1 if unspecified]
    -c CONSEC   number of consecutive events desired [1 if unspecified]

Examples:
If you want to look at a specific event in the TPC:
./darkdisplay.sh -t -r 5475 -e 42474

If you want to look at a specific event in the OD:
./darkdisplay.sh -o -r 8000 -e 314 

If you want to look at a specific event in the TPC and OD:
./darkdisplay.sh -to -r 8000 -e 401

If you want to look at 5 consecutive events in the OD starting at event 300
./darkdisplay.sh -o -r 8000 -e 300 -c 5

If you want to save the display output root files but don't want to open up the display:
./darkdisplay.sh -t -r 5475 -e 42474 --nodisplay

If you want to show the V1724 waveforms for the TPC (will only work for runs that have V1724 data obviously)
./darkdisplay.sh -t -r 12561 -e 20644 --useV1724

If you want to enable 3D event reconstruction
./darkdisplay.sh -t -r 5475 -e 42474 --enable3D

* See redmine wiki on how to open up root files you've already created and for how to submit a batch of many events
EOF
}
# Set defaults. These will change based on command line arguments
tpc_enabled=false
od_enabled=false
current_directory="${PWD}";
enabled_3d=""
# Get command line options
while getopts "htor:e:c:f:d:-:" opt; do
    case "$opt" in
        h)
            show_help
	    kill -INT $$ # Stop the script
            ;;
        t)
            tpc_enabled=true
            ;;
        o)  od_enabled=true
            ;;
        r)  run=$OPTARG
            ;;
        e)  event=$OPTARG
            ;;
        c)  consecutive_events=$OPTARG
            ;;
        f)  odfile_subrun1=$OPTARG
            ;;
	d)  output_directory=$OPTARG
	    ;;
	-)  
	    case ${OPTARG} in
		"nodisplay"*) nodisplay=true
		    ;;
		"usebestguess"*) usebestguess=true
		    ;;
		"useV1724"*) useV1724=true
		    ;;
		"enable3D"*) enabled_3d="--enable3D";
	    esac
    esac
done

# Set fcl files to be used
if [ "$useV1724" = "true" ]; then
    tpcfcl=tpcdisplay_V1724.fcl
else 
    tpcfcl=tpcdisplay.fcl
fi
odfcl=oddisplay.fcl



# Verify inputs 
if [[ "$tpc_enabled" = "false" ]] && [[ "$od_enabled" = "false" ]]; then
    printf "\nERROR: No detector enabled. For help try: ./darkdisplay.sh -h\n"
    kill -INT $$ # Stop the script
fi
if [ -z "$run" ]; then
    printf "\nERROR: No run specified.  For help try: ./darkdisplay.sh -h\n"
    kill -INT $$ # Stop the script
fi
if [ -z "$event" ]; then 
    printf "\nWARNING: No event specified. Setting event=1\n"
    event=1
fi
if [ -z "$consecutive_events" ]; then consecutive_events=1; fi


# Remove leading zeros
run=$((10#$run))
event=$((10#$event))

# set output directory
if [ -z "$output_directory" ]; then
    output_directory="${current_directory}/"
else
    output_directory="$(readlink -e ${output_directory})/"
fi

# set output file names 
od_display_output="${output_directory}od_display_output_r${run}_e${event}.root"
tpc_display_output="${output_directory}tpc_display_output_r${run}_e${event}.root"

# print output file locations
printf "#################################\n"
if [ "$tpc_enabled" = "true" ]; then
    printf "Output file will be generated at $tpc_display_output\n"
fi
if [ "$od_enabled" = "true" ]; then
    printf "Output file will be generated at $od_display_output\n"
fi
printf "#################################\n"

# Change to build directory
if [ ! -d "$CETPKG_BUILD" ]; then
    printf "\nERROR: High level analysis is not set up! Setting it up for you...\n"
    if [ ! -f /ds50/app/ds50/setup_ds50 ]; then
	echo "Could not find /ds50/app/ds50/setup_ds50. Are you sure you're on ds50srv.fnal.gov?"
	kill -INT $$ # Stop the script
    fi
    source /ds50/app/ds50/setup_ds50
    source /ds50/app/ds50/setup_ds50_build
fi
echo "Changing directory to darkart build directory: ${CETPKG_BUILD}"
cd $CETPKG_BUILD
#cd $output_directory
# Set fcl filepath
export FHICL_FILE_PATH=$FHICL_FILE_PATH:/ds50/app/user/${USER}/work/darkart/fcl

# Create temp fcl file with desired event number and number of consecutive events desired.
# The od and tpc display fcl files look for this fcl file so they know which events to process.
# generate temp fhicl file to be fed into art
printf \
"single_event : ${event}\n\
consecutive_events : ${consecutive_events}\n\
od_display_output : \"${od_display_output}\"\n\
tpc_display_output : \"${tpc_display_output}\"\n"\
>event_selection.fcl

# # Check if run is defined
# if [ -z "$run" ]; then
#     printf "\nERROR: No run specified!\n"
#     kill -INT $$ # Stop the script
# fi


###########
# TPC art #
###########
if [ "$tpc_enabled" = "true" ]; then

    # Get subrun
    subrun=$(/ds50/app/user/jcjs/work/SubrunListGenerator/GetSubrun.sh $run $event)
    # Check to make sure subrun was found
    if [ "$subrun" = "0" ]; then
	printf "\nCould not find subrun for run $run event $event\n"
	eventspersubrun=$(/ds50/app/user/jcjs/work/SubrunListGenerator/GetEventsPerSubrun.sh $run)
	guess=$(( $event/$eventspersubrun + 1 ))
	if [ "$usebestguess" = true ]; then
	    printf "\nWarning: Using best-guess subrun: ($guess)\n\n"
	    subrun=$guess
	else
	    printf "Enter subrun, or press enter to use best-guess subrun ($guess): "
	    read subrun
	    if [ -z "$subrun" ]; then
		subrun=$guess
	    fi
	fi
    fi

    # Access tpc file
    kx509
    voms-proxy-init --noregen --voms fermilab:/fermilab/darkside
    
    FULLRUNNUM=$(printf "%06d\n" $run)
    FULLSUBNUM=$(printf "%03d\n" $subrun)   
    echo "Retrieving file list..."
    let mathfriendly=10#$FULLRUNNUM
    type=commissioning
    echo "Searching in $type "
    if [ ${mathfriendly} -lt 7560 ]; then
	tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:4}xx/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
    else
	tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:5}x/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `

	if [ ! -n "$tpcfile" ]; then
            type="calibration"
	    echo "Searching in $type "
            tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:5}x/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
	fi
	if [ ! -n "$tpcfile" ]; then
	    type="wimp_search"
	    echo "Searching in $type "
	    tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:5}x/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
	fi
    fi



    # if [ ${mathfriendly} -lt 7560 ]; then
    # 	tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:4}xx/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
    # else
    # 	tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:5}x/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
    # 	if [ -n "$tpcfile" ]; then
    #         type="commissioning"
    # 	else
    # 	    type="calibration"
    #         tpcfile=`uberftp -ls gsiftp://fndca4a.fnal.gov:2811/raw/${type}/Run${FULLRUNNUM:0:2}xxxx/Run${FULLRUNNUM:0:5}x/ds50_r${FULLRUNNUM}_sr${FULLSUBNUM}*.root | awk '{print $8}' `
    # 	fi
    # fi

    # Run art on tpc file
    art -c $tpcfcl xroot://fndca4a.fnal.gov:1094/pnfs/fnal.gov/usr/darkside/$tpcfile
fi

##########
# OD art #
##########
if [ "$od_enabled" = "true" ]; then
    # Check if path to first subrun is defined
    # if [ -z "$odfile_subrun1" ]; then
    # 	printf "\nERROR: No path to first subrun was specified!\n"
    # 	kill -INT $$ # Stop the script
    # fi
    
    # Get subrun
    echo /ds50/app/user/jcjs/work/SubrunListGenerator/GetSubrunOD.sh $run $event
    subrun=$(/ds50/app/user/jcjs/work/SubrunListGenerator/GetSubrunOD.sh $run $event)
    # Check to make sure subrun was found
    if [ "$subrun" = "0" ]; then
	printf "\nCould not find subrun for run $run event $event\n"
	eventspersubrun=$(/ds50/app/user/jcjs/work/SubrunListGenerator/GetEventsPerSubrunOD.sh $run)
	guess=$(( $event/$eventspersubrun + 1 ))
	if [ "$usebestguess" = true ]; then
	    printf "\nWarning: Using best-guess subrun: ($guess)\n\n"
	    subrun=$guess
	else
	    printf "Enter subrun, or press enter to use best-guess subrun ($guess): "
	    read subrun
	    if [ -z "$subrun" ]; then
		subrun=$guess
	    fi
	fi
    fi
    
    # Copy OD run to scratch    
    kx509
    voms-proxy-init --noregen --voms fermilab:/fermilab/darkside
    
    # let RUNABC=run/10
    # let RUN100=run/100
    FULLRUNNUM=`printf %06d $run`
    FULLSUBNUM=`printf %03d $subrun`
    RUNAB=${FULLRUNNUM:0:2}
    RUNABCDE=${FULLRUNNUM:0:5}
    phase=commissioning
    
    if [ ! -f /scratch/darkside/rawdata/odrawdata/ODRun${FULLRUNNUM}/ODRun${FULLRUNNUM}_001.dat ]; then
	globus-url-copy -vb -g2 -cd -p 10 gsiftp://fndca1.fnal.gov:2811/raw/${phase}/lsv/ODRun${RUNAB}xxxx/ODRun${RUNABCDE}x/ODRun${FULLRUNNUM}_001.dat	/scratch/darkside/rawdata/odrawdata/ODRun${FULLRUNNUM}/
    fi
    if [ ! -f /scratch/darkside/rawdata/odrawdata/ODRun${FULLRUNNUM}/ODRun${FULLRUNNUM}_${FULLSUBNUM}.dat ]; then
	globus-url-copy -vb -g2 -cd -p 10 gsiftp://fndca1.fnal.gov:2811/raw/${phase}/lsv/ODRun${RUNAB}xxxx/ODRun${RUNABCDE}x/ODRun${FULLRUNNUM}_${FULLSUBNUM}.dat /scratch/darkside/rawdata/odrawdata/ODRun${FULLRUNNUM}/
    fi
    odfile_subrun1=/scratch/darkside/rawdata/odrawdata/ODRun${FULLRUNNUM}/ODRun${FULLRUNNUM}_001.dat

    art -c $odfcl $odfile_subrun1
fi

#################
# Event Display #
#################

#cd $output_directory


if [ "$nodisplay" = true ]; then
    kill -INT $$ # Stop the script
fi

if [[ "$tpc_enabled" = "true" && "$od_enabled" = "true" ]]; then
    /ds50/app/user/jcjs/work/EventDisplay/EventDisplay "${tpc_display_output}" "${od_display_output}" "${enabled_3d}"
elif [ "$tpc_enabled" = "true" ]; then
    /ds50/app/user/jcjs/work/EventDisplay/EventDisplay "${tpc_display_output}" "${enabled_3d}"
elif [ "$od_enabled" = "true" ]; then
    /ds50/app/user/jcjs/work/EventDisplay/EventDisplay "${od_display_output}" "${enabled_3d}"
else
    printf "\nERROR: No detectors were enabled! See usage: ./darkdisplay -h\n"
    kill -INT $$ # Stop the script
fi


# Remove temp fcl file
#rm event_selection.fcl

