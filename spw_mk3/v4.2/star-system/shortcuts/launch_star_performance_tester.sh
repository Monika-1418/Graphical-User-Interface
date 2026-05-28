#!/bin/sh

#+
# Name:
#    launch_star_performance_tester.sh
#
# Purpose:
#    Launches STAR-System performance tester.
#
# Syntax:
#    launch_star_performance_tester.sh
#
# Description:
#    This script will:
#    - Launches STAR performance
#      tester in a terminal.
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2018
#
# Notes:
#    This script is intended to be run as
#    a shortcut to STAR Performance Tester.
#-

# Script directory
dir=`dirname $0`

# This is the STAR performance tester application.
star_performance_tester_path="bin/star_performance_tester"

# Open STAR-System Test application in terminal
$dir/launch_application.sh $dir/$star_performance_tester_path

# Nice exit code
exit 0
