#!/bin/sh

#
# STAR-System command line installation script.
#
# STAR-Dundee Ltd
# STAR House
# 166 Nethergate
# Dundee, DD1 4EE
# Scotland, UK
# e-mail: support@star-dundee.com
#
# Copyright (c) 2019 STAR-Dundee Ltd.
#

# Installer flags
INSTALL_DIR=0
INSTALL_QT4=0
INSTALL_QT5=0
INSTALL_EXAMPLES=0
INSTALL_INCLUDES=0
INSTALL_DOC=0
INSTALL_APP_NOTES=0
INSTALL_PCI_DRIVER=0
INSTALL_USB_DRIVER=0
INSTALL_CPP_DOC=0
INSTALL_CPP_INC=0
INSTALL_CPP_EXAMPLES=0
OVERWRITE=0
DISPLAY_HELP=0

# Display help if no arguments specified
if [ -z "$1" ]
  then
    set -- "--help"
fi

# Handle arguments
for arg in "$@"
do
	while [ "$arg" ] ;do
		split_arg=${arg%%,*}
		
		case ${split_arg:?} in 
		  --install-dir=* ) INSTALL_DIR=${split_arg#--install-dir=} ;;
		  --qt4 ) INSTALL_QT4=1 ;;
		  --qt5 ) INSTALL_QT5=1 ;;
		  --examples ) INSTALL_EXAMPLES=1 ;;
		  --includes ) INSTALL_INCLUDES=1 ;;
		  --doc ) INSTALL_DOC=1 ;;
		  --app-notes ) INSTALL_APP_NOTES=1 ;;
		  --pci-driver ) INSTALL_PCI_DRIVER=1 ;;
		  --usb-driver ) INSTALL_USB_DRIVER=1 ;;
		  --cpp-doc ) INSTALL_CPP_DOC=1 ;;
		  --cpp-inc ) INSTALL_CPP_INC=1 ;;
		  --cpp-examples ) INSTALL_CPP_EXAMPLES=1 ;;
		  --all )
			INSTALL_EXAMPLES=1
			INSTALL_QT4=1
			INSTALL_QT5=1
			INSTALL_INCLUDES=1
			INSTALL_DOC=1
			INSTALL_APP_NOTES=1
			INSTALL_PCI_DRIVER=1
			INSTALL_USB_DRIVER=1
			INSTALL_CPP_DOC=1
			INSTALL_CPP_INC=1
			INSTALL_CPP_EXAMPLES=1 ;;
		  --overwrite ) OVERWRITE=1 ;;
		  --help ) DISPLAY_HELP=1 ;;
		  * ) echo "Unrecognised option: ${split_arg:?}" ;;
		esac
		
		[ "$arg" = "$split_arg" ] && \
			arg='' || \
			arg="${arg#*,}"
	done
done

# Get location of script
script_dir=$(dirname "$0")

# Check for empty install directory
if [ "${INSTALL_DIR:?}" = "0" ]; then
    INSTALL_DIR=/usr/local/STAR-Dundee/STAR-System
fi

# The archive containing the STAR-System files
tar_file=star-system.tgz

# Display help if requested
if [ ${DISPLAY_HELP:?} = 1 ]; then
	echo "Available options:"
	echo "  --install-dir=[target]  Default /usr/local/STAR-Dundee/STAR-System"
	echo "  --qt4                   Qt4 GUI applications"
	echo "  --qt5                   Qt5 GUI applications"
	echo "  --examples              C API example files"
	echo "  --includes              C API header files"
	echo "  --doc                   C API documentation"
	echo "  --app-notes             Application notes"
	echo "  --pci-driver            The PCI driver"
	echo "  --usb-driver            The USB driver"
	echo "  --cpp-doc               C++ API documentation"
	echo "  --cpp-inc               C++ API header files"
	echo "  --cpp-examples          C++ API example files"
	echo "  --all                   Installs all optional features"
	
	exit 0
fi

# Create the installation location
mkdir -p ${INSTALL_DIR:?}

# Ensure directory is not empty (unless OVERWRITE flag specified)
if [ ! ${OVERWRITE:?} = 1 ]; then
	if [ "$(ls -A ${INSTALL_DIR:?})" ]; then
		echo "##ERROR## Install directory not empty: ${INSTALL_DIR:?}"
		exit 1
	fi
fi

# Extract core files
if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "build_star-system.sh" "remove_star-system.sh" "scripts/star_device_config" "lib"
then
	echo "Core files unpacked"
else
	echo "##ERROR## Failed to unpack core files"
	exit 1
fi

# Extract optional files
if [ ${INSTALL_EXAMPLES:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "examples"
	then
		echo "C API examples extracted"
	else
		echo "##ERROR## Failed to unpack C API examples"
		exit 1
	fi
fi
if [ ${INSTALL_INCLUDES:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "inc"
	then
		echo "C API includes extracted"
	else
		echo "##ERROR## Failed to unpack C API includes"
		exit 1
	fi
fi
if [ ${INSTALL_DOC:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "doc"
	then
		echo "C API documentation extracted"
	else
		echo "##ERROR## Failed to unpack C API documentation"
		exit 1
	fi
fi
if [ ${INSTALL_APP_NOTES:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "application_notes"
	then
		echo "Application notes extracted"
	else
		echo "##ERROR## Failed to unpack application notes"
		exit 1
	fi
fi
if [ ${INSTALL_PCI_DRIVER:?} = 1 ] || [ ${INSTALL_USB_DRIVER:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "driver_src"
	then
		echo "drivers extracted"
	else
		echo "##ERROR## Failed to unpack drivers"
		exit 1
	fi
	if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then
		if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "scripts/99-star_spw_pci.rules"
		then
			echo "PCI driver rules extracted"
		else
			echo "##ERROR## Failed to unpack PCI driver rules"
			exit 1
		fi
	fi
	if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then
		if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "scripts/99-star_spw_usb.rules"
		then
			echo "USB driver rules extracted"
		else
			echo "##ERROR## Failed to unpack USB driver rules"
			exit 1
		fi
	fi
fi
if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "apis/cpp_api/doc"
	then
		echo "C++ API documentation extracted"
	else
		echo "##ERROR## Failed to unpack C++ API documentation"
		exit 1
	fi
fi
if [ ${INSTALL_CPP_INC:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "apis/cpp_api/inc"
	then
		echo "C++ API includes extracted"
	else
		echo "##ERROR## Failed to unpack C++ API includes"
		exit 1
	fi
fi
if [ ${INSTALL_CPP_EXAMPLES:?} = 1 ]; then
	if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "apis/cpp_api/examples"
	then
		echo "C++ API examples extracted"
	else
		echo "##ERROR## Failed to unpack C++ API examples"
		exit 1
	fi
fi

# Extract shortcut files
if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} "shortcuts"
then
	echo "shortcuts extracted"
else
	echo "##ERROR## Failed to unpack shortcuts"
	exit 1
fi

# Construct build arguments
build_args=0
if [ ${INSTALL_EXAMPLES:?} = 1 ]; then
	if [ "${build_args:?}" = "0" ]; then
		build_args="--examples "
	else
		build_args="${build_args:?}--examples "
	fi
fi
if [ ${INSTALL_QT4:?} = 1 ]; then
	if [ "${build_args:?}" = "0" ]; then
		build_args="--qt4 "
	else
		build_args="${build_args:?}--qt4 "
	fi
fi
if [ ${INSTALL_QT5:?} = 1 ]; then
	if [ -z "${build_args:?}" ]; then
		build_args="--qt5 "
	else
		build_args="${build_args:?}--qt5 "
	fi
fi
if [ ${INSTALL_DOC:?} = 1 ]; then
	if [ -z "${build_args:?}" ]; then
		build_args="--doc "
	else
		build_args="${build_args:?}--doc "
	fi
fi
if [ ${INSTALL_PCI_DRIVER:?} = 0 ]; then
	if [ -z "${build_args:?}" ]; then
		build_args="--no-pci-driver "
	else
		build_args="${build_args:?}--no-pci-driver "
	fi
fi
if [ ${INSTALL_USB_DRIVER:?} = 0 ]; then
	if [ -z "${build_args:?}" ]; then
		build_args="--no-usb-driver "
	else
		build_args="${build_args:?}--no-usb-driver "
	fi
fi
if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
	if [ -z "${build_args:?}" ]; then
		build_args="--cpp-doc "
	else
		build_args="${build_args:?}--cpp-doc "
	fi
fi

# Build STAR-System
cd ${INSTALL_DIR:?}
if [ "${build_args:?}" = "0" ]; then
	./build_star-system.sh
else
	./build_star-system.sh ${build_args:?}
fi

# Check build status
retval=$?
if [ ${retval:?} -eq 0 ]; then
	echo "Installation completed to ${INSTALL_DIR:?}"
else
	echo "Errors during installation to ${INSTALL_DIR:?}"
fi
