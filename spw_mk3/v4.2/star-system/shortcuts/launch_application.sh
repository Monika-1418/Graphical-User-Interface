#!/bin/sh

#+
# Name:
#    launch_application.sh
#
# Purpose:
#    Launches the given application in a terminal.
#
# Syntax:
#    launch_star_cuba.sh
#
# Description:
#    This script will:
#    - Launch specified application in a terminal.
#    - Try xterm, gnome-terminal and konsole in this order.
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2018
#
# Notes:
#    This script is intended to be run as
#    a shortcut to an application that has 
#    been installed.
#-

# Script directory
dir=`dirname $0`

# Get path to application to be launched
application_path=$1

# Try xterm
if xterm -hold -e "$application_path"
then
   echo "$application_path" "opened"
else
   # Try gnome-terminal
   if gnome-terminal -e "bash -c '"$application_path";$SHELL'"
   then
       echo "$application_path" "opened"
   else
       # Try konsole
       if konsole --hold -e "$application_path"
       then
           echo "$application_path" "opened"
       else
           echo "##ERROR## Failed to open" "$application_path"
       fi
   fi
   exit 1
fi

# Nice exit code
exit 0
