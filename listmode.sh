#!/bin/bash
listmode_file=$1
if [ -z "$listmode_file" ]; then
    printf "Usage: ./listmode <text file containing run and event ids>"
    kill -INT $$ # Stop the script
fi

# make directory for outputs
now="$(date +'%Y%m%d%H%M%S')"
output_directory="tpc_display_output_${now}"
mkdir "${output_directory}"

# run darkdisplay on each event in the listmode file (tpc only)
while read run event
do 
    echo $run $event
#    ./darkdisplay.sh "-t" "-r" "${run}" "-e" "${event}" "-d" "${output_directory}" "--nodisplay" "--usebestguess"; 
done < "$listmode_file"

printf "\nComplete. Now do:\n\n./EventDisplay -d ${output_directory}\n\nto open the display.\n"