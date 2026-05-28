#!/bin/sh

#+
# Name:
#    launch_star-system_test.sh
#
# Purpose:
#    Launches STAR-System Test.
#
# Syntax:
#    launch_star-system_test.sh
#
# Description:
#    This script will:
#    - Launches STAR-System Test in a terminal.
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2018
#
# Notes:
#    This script is intended to be run as
#    a shortcut to STAR-System Test.
#-

# Script directory
dir=`dirname $0`

# This is the STAR-System Test application.
star_system_test_path="bin/star-system_test"

# Open STAR-System Test application in terminal
$dir/launch_application.sh $dir/$star_system_test_path

# Nice exit code
exit 0
