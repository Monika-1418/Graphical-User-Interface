#!/bin/sh

#+
# Name:
#    launch_star_cuba.sh
#
# Purpose:
#    Launches CUBA software.
#
# Syntax:
#    launch_star_cuba.sh
#
# Description:
#    This script will:
#    - Launches CUBA software in a terminal.
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2018
#
# Notes:
#    This script is intended to be run as
#    a shortcut to the CUBA software.
#-

# Script directory
dir=`dirname $0`

# This is the CUBA software executable.
star_cuba_path="bin/star_cuba"

# Open CUBA software in terminal
$dir/launch_application.sh $dir/$star_cuba_path

# Nice exit code
exit 0
