#!/bin/sh

#+
# Name:
#    launch_cpp_docs.sh
#
# Purpose:
#    Opens the STAR-System C++ API documentation
#    in the user's browser.
#
# Syntax:
#    launch_cpp_docs.sh
#
# Description:
#    This script will:
#    - Open the STAR-System C++ API documentation
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2011
#
# Notes:
#    This script is intended to be run as
#    a shortcut to the STAR-System C++ API documentation.
#
# Authors:
#    Craig Ramsay(support@star-dundee.com)
#    {list_authors_here}
#
# History:
#    24-June-2013:
#       Created
#    {list_changes_here}
#-

# Script directory
dir=`dirname $0`

# This is the STAR-System C++ API documentation index file.
index_file="apis/cpp_api/doc/index.xhtml"

# Open the documentation index file
if xdg-open "$dir"/"$index_file"
then
   echo "STAR-System C++ API documentation opened"
else
   echo "##ERROR## Failed to open STAR-System C++ API documentation"
   exit 1
fi

# Nice exit code
exit 0
