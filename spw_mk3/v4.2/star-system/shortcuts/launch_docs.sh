#!/bin/sh

#+
# Name:
#    launch_docs.sh
#
# Purpose:
#    Opens the STAR-System documentation
#    in the users browser.
#
# Syntax:
#    launch_docs.sh
#
# Description:
#    This script will:
#    - Opens the STAR-System documentation
#
# Language:
#    Bourne shell script
#
# Copyright:
#    (c) STAR-Dundee Ltd, 2011
#
# Notes:
#    This script is intended to be run as
#    a shortcut to the STAR-System documentation.
#
# Authors:
#    Stephen Mudie (support@star-dundee.com)
#    {list_authors_here}
#
# History:
#    15-Dec-2011:
#       Created
#    {list_changes_here}
#-

# Script directory
dir=`dirname $0`

# This is the STAR-System documentation index file.
index_file="doc/index.xhtml"

# Open the documentation index file
if xdg-open "$dir"/"$index_file"
then
   echo "STAR-System documentation opened"
else
   echo "##ERROR## Failed to open STAR-System documentation"
   exit 1
fi

# Nice exit code
exit 0
