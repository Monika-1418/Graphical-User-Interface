#!/bin/sh

#
# STAR-System uninstall script for Linux, to remove all components added by
# build_star-system.sh and change_permissions.sh.
#
# STAR-Dundee Ltd
# STAR House
# 166 Nethergate
# Dundee, DD1 4EE
# Scotland, UK
# e-mail: support@star-dundee.com
#
# Copyright (c) 2012 STAR-Dundee Ltd.
# 
 
 
# The version of STAR-System
MAJOR_VERSION=4
MINOR_VERSION=02

# Properties of the system
KVERSION=`uname -r`						# Current kernel version
DRIVER_PATH="/lib/modules/$KVERSION/kernel/drivers/spacewire/"	# Kernel's driver dir
 
# Locations and names of installed files
DEVICE_CONFIG_EXE=star_conf_service		# Name of the Device Config executable 
DEVICE_CONFIG_SCRIPT=star_device_config	# Name of the Device Config script 
RULES_DIR=/etc/udev/rules.d/			# The directory to put the rules
PCI_RULES_SCRIPT=99-star_spw_pci.rules	# Name of the PCI rules script
USB_RULES_SCRIPT=99-star_spw_usb.rules	# Name of the USB rules script
DRIVERS_DIR=driver_src					# Location of the driver sources
PCI_DRIVER_DIR=pci_driver				# Location of the PCI driver source
PCI_DRIVER=star_spw_pci					# Name of the PCI driver module				
PCI_DRIVER_PATH="$DRIVER_PATH$PCI_DRIVER.ko"	# Path to the PCI driver module
USB_DRIVER_DIR=usb_driver				# Location of the USB driver source
USB_DRIVER=star_spw_usb					# Name of the USB driver module				
USB_DRIVER_PATH="$DRIVER_PATH$USB_DRIVER.ko"	# Path to the USB driver module
EXAMPLES_DIR=examples					# Name of directory containing examples
PROGRAMS_DIR=bin						# Name of directory containing binaries
STAR_SYSTEM_TEST_DIR=star_system_test	# Name of STAR-System Test directory
STAR_SYSTEM_TEST=star-system_test		# Name of STAR-System Test binary
PERF_TESTER_DIR=performance_tester		# Name of Performance Tester directory
PERF_TESTER=star_performance_tester		# Name of Performance Tester binary
TIME_CODE_TEST_DIR=time-code_test		# Name of STAR-System Test directory
TIME_CODE_TEST=time-code_test			# Name of STAR-System Test binary
CONFIG_EXAMPLE_DIR=config				# Name of Config API example directory
PCI_MK2_CONFIG_EXAMPLE_DIR=pciMk2		# Name of PCI Mk2 Config API example directory
PCI_MK2_CONFIG_EXAMPLE=bin/pciMk2Configuration	# Name of PCI Mk2 Config API example binary
BRICK_MK2_CONFIG_EXAMPLE_DIR=brickMk2		# Name of Brick Mk2 Config API example directory
BRICK_MK2_CONFIG_EXAMPLE=bin/brickMk2Configuration	# Name of Brick Mk2 Config API example binary
ROUTER_MK2S_CONFIG_EXAMPLE_DIR=routerMk2S		# Name of Router Mk2S Config API example directory
ROUTER_MK2S_CONFIG_EXAMPLE=bin/routerMk2SConfiguration	# Name of Router Mk2S Config API example binary
RMAP_EXAMPLE_DIR=rmap					# Name of RMAP API example directory
RMAP_EXAMPLE=rmap_examples				# Name of RMAP API example binary
STAR_API_LIB=libstar-api.so				# Name of the STAR-API lib
RMAP_LIB=librmap_packet_library.so		# Name of the RMAP Packet Library lib
RMAP_INITIATOR_LIB=libstar_rmap_initiator.so	# Name of the RMAP Initiator lib
MK2_CONFIG_LIB=libstar_conf_api_mk2.so	# Name of the Mk2 Config lib
PCIMK2_CONFIG_LIB=libstar_conf_api_pci_mk2.so	# Name of the PCI Mk2 Config lib
PXI_CONFIG_LIB=libstar_conf_api_pxi.so	# Name of the PXI Config lib
SPFI_CONFIG_LIB=libstar_conf_api_spfi_router.so	# Name of the SpFi router Config lib
BRICKMK3_CONFIG_LIB=libstar_conf_api_brick_mk3.so	# Name of the Brick Mk3 Config lib
BRICKMK2_CONFIG_LIB=libstar_conf_api_brick_mk2.so	# Name of the Brick Mk2 Config lib
ROUTERMK2S_CONFIG_LIB=libstar_conf_api_router_mk2s.so	# Name of the Router Mk2S Config lib
ROUTER_CONFIG_LIB=libstar_conf_api_router.so	# Name of the Router Config lib
CASTOR_CONFIG_LIB=libstar_conf_api_castor.so	# Name of the castor Config lib
GENERIC_CONFIG_LIB=libstar_conf_api_generic.so	# Name of the generic Config lib
PCIMK2_PACKET_SUBSYS_LIB=libstar_packet_subsystem.so	# Name of the PCI Mk2 packet subsystem lib
CONFIG_MESSENGER_LIB=libstar_conf_mssgr.so	# Name of the Config Messenger lib
RMAP_TARGET_LIB=libstar_rmap_target.so	# Name of the RMAP_TARGET lib
TRIGGERING_LIB=libstar_triggering.so	# Name of the Triggering lib
TCP_DRIVER_LIB=star_tcp_driver.so	# Name of the TCP/IP driver lib
# The list of lib files
LIB_FILE_LIST="${STAR_API_LIB:?} ${RMAP_LIB:?} ${RMAP_INITIATOR_LIB:?} ${MK2_CONFIG_LIB:?} ${PCIMK2_CONFIG_LIB:?} ${PXI_CONFIG_LIB:?} ${SPFI_CONFIG_LIB:?} ${BRICKMK2_CONFIG_LIB:?} ${BRICKMK3_CONFIG_LIB:?} ${ROUTERMK2S_CONFIG_LIB:?} ${ROUTER_CONFIG_LIB:?} ${CASTOR_CONFIG_LIB:?} ${GENERIC_CONFIG_LIB:?} ${PCIMK2_PACKET_SUBSYS_LIB:?} ${CONFIG_MESSENGER_LIB:?} ${RMAP_TARGET_LIB:?} ${TRIGGERING_LIB:?} ${TCP_DRIVER_LIB:?}"
DEVICE_CONFIG_GUI="star_device_config"	# Name of the Device Configuration GUI application
RECEIVE_GUI="star_receive"				# Name of the Receive GUI application
TRANSMIT_GUI="star_transmit"			# Name of the Transmit GUI application
SINK_GUI="star_sink"					# Name of the Sink GUI application
SOURCE_GUI="star_source"				# Name of the Source GUI application
DEVICE_UPDATE_GUI="star_device_update"	# Name of the Device Update GUI application
ERROR_INJECTION_GUI="star_error_inject"	# Name of the Error Injection GUI application
TIME_CODE_GUI="star_time-code"			# Name of the Time-code GUI application
RMAP_TARGET_GUI="star_rmap_target"		# Name of the RMAP Target GUI application
GUI_APP_LIST="${DEVICE_CONFIG_GUI:?} ${RECEIVE_GUI:?} ${TRANSMIT_GUI:?} ${SINK_GUI:?} ${SOURCE_GUI:?} ${DEVICE_UPDATE_GUI:?} ${ERROR_INJECTION_GUI:?} ${TIME_CODE_GUI:?} ${RMAP_TARGET_GUI:?}"
CUBA_SOFTWARE="star_cuba"               # Name of the CUBA Software application

# Get target dir passed from installer
TARGET_DIR=$1

# Whether the uninstall has failed
failed="no"


# Make sure that the init.d directory exists
INST_ETC="/etc/rc.d"
if [ ! -d "${INST_ETC:?}" ]; then
	INST_ETC="/etc"
fi
RC_ETC="${INST_ETC:?}"
if [ ! -d "${INST_ETC:?}/init.d" ]; then
	INST_ETC="/etc"
fi
if [ -d "${INST_ETC:?}/init.d" ]; then

	# Stop the Device Configuration Service
	echo "Stopping Device Configuration Service ..."
	"${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}" stop

	# Remove the Device Configuration Service script from init.d
	if rm -f  "${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}"; then
		echo ""
	else
		echo "##ERROR## Unable to remove Device Configuration Service script from ${INST_ETC:?}/init.d"
		failed="yes"
	fi

	if update-rc.d ${DEVICE_CONFIG_SCRIPT:?} remove; then
		echo "Removed symlinks to ${DEVICE_CONFIG_SCRIPT:?}"
	else
		# Remove soft links from rc{2,3,5}.d/S99star_device_config
		for RUN_LEVEL in 2 3 5; do
			if [ -e ${RC_ETC:?}/rc${RUN_LEVEL:?}.d ]; then
				if rm -f "${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S99${DEVICE_CONFIG_SCRIPT:?}"; then
					echo ""
				else
					echo "##ERROR## Unable to remove symlink ${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S99${DEVICE_CONFIG_SCRIPT:?}"
					failed="yes"
				fi
			else
				echo "##ERROR## No ${RC_ETC:?}/rc${RUN_LEVEL:?}.d directory, unable to remove symlink ${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S99${DEVICE_CONFIG_SCRIPT:?}"
				failed="yes"
			fi
		done
	fi
else
	echo "##ERROR## Unable to find boot script directory ${INST_ETC:?}/init.d"
	failed="yes"
fi


# Removing the Device Configuration Service
if [ -e /sbin/${DEVICE_CONFIG_EXE:?} ]; then
	if rm -f "/sbin/${DEVICE_CONFIG_EXE:?}"; then
		echo ""
	else
		echo "##ERROR## Unable to remove /sbin/${DEVICE_CONFIG_EXE:?}"
		failed="yes"
	fi
fi

# Remove the files to set access permissions to the PCI and USB drivers
if [ -e ${RULES_DIR:?} ]; then
	if rm -f "${RULES_DIR:?}/${PCI_RULES_SCRIPT:?}"; then
		echo ""
	else
		echo "##ERROR## Unable to remove ${RULES_DIR:?}/${PCI_RULES_SCRIPT:?}"
		failed="yes"
	fi
	if rm -f "${RULES_DIR:?}/${USB_RULES_SCRIPT:?}"; then
		echo ""
	else
		echo "##ERROR## Unable to remove ${RULES_DIR:?}/${USB_RULES_SCRIPT:?}"
		failed="yes"
	fi
fi

 
# Remove the PCI Driver module
if [ -e ${PCI_DRIVER_PATH:?} ]; then
	echo "Removing PCI driver module ..."
	if rm -f ${PCI_DRIVER_PATH:?}; then
		echo ""
	else
		echo "##ERROR## Warning: unable to remove PCI driver module ${PCI_DRIVER_PATH:?}"
		echo ""
		failed="yes"
	fi
fi

# Remove the USB Driver module
if [ -e ${USB_DRIVER_PATH:?} ]; then
	echo "Removing USB driver module ..."
	if rm -f ${USB_DRIVER_PATH:?}; then
		echo ""
	else
		echo "##ERROR## Warning: unable to remove USB driver module ${USB_DRIVER_PATH:?}"
		echo ""
		failed="yes"
	fi
fi

if [ -e ${DRIVERS_DIR:?} ]; then
	cd ${DRIVERS_DIR:?}
	
	# Remove the PCI Driver objects
	if [ -e ${PCI_DRIVER_DIR:?} ]; then
		cd "${PCI_DRIVER_DIR:?}"
		make -f Kbuild clean
		cd ..
	fi
	
	# Remove the USB Driver objects
	if [ -e ${USB_DRIVER_DIR:?} ]; then
		cd "${USB_DRIVER_DIR:?}"
		make -f Kbuild clean
		cd ..
	fi

	cd ..
fi


# Remove the test programs
if [ -e ${EXAMPLES_DIR:?} ]; then
	echo "Removing the test programs..."

	cd "${EXAMPLES_DIR:?}"
	
	if [ -e ${STAR_SYSTEM_TEST_DIR:?} ]; then
		cd "${STAR_SYSTEM_TEST_DIR:?}"
		make clean
		if ! rm -f "" "../../${PROGRAMS_DIR:?}/${STAR_SYSTEM_TEST:?}"; then
			echo "##WARNING## Couldn't delete STAR-System Test executable"
		fi
		cd ..
	fi
	
	if [ -e ${PERF_TESTER_DIR:?} ]; then
		cd "${PERF_TESTER_DIR:?}"
		make clean
		if ! rm -f "" "../../${PROGRAMS_DIR:?}/${PERF_TESTER:?}"; then
			echo "##WARNING## Couldn't delete Performance Tester executable"
		fi
		cd ..
	fi
	
	if [ -e ${TIME_CODE_TEST_DIR:?} ]; then
		cd "${TIME_CODE_TEST_DIR:?}"
		make clean
		if ! rm -f "" "../../${PROGRAMS_DIR:?}/${TIME_CODE_TEST:?}"; then
			echo "##WARNING## Couldn't delete Time-code Test executable"
		fi
		cd ..
	fi
	
	if [ -e ${CONFIG_EXAMPLE_DIR:?} ]; then
		cd "${CONFIG_EXAMPLE_DIR:?}"
		
		if [ -e ${PCI_MK2_CONFIG_EXAMPLE_DIR:?} ]; then
			cd "${PCI_MK2_CONFIG_EXAMPLE_DIR:?}"
			make clean
			if ! rm -f "" "../../../${PROGRAMS_DIR:?}/${PCI_MK2_CONFIG_EXAMPLE:?}"; then
				echo "##WARNING## Couldn't delete PCI Mk2 Device Configuration Example executable"
			fi
			cd ..
		fi
		
		if [ -e ${BRICK_MK2_CONFIG_EXAMPLE_DIR:?} ]; then
			cd "${BRICK_MK2_CONFIG_EXAMPLE_DIR:?}"
			make clean
			if ! rm -f "" "../../../${PROGRAMS_DIR:?}/${BRICK_MK2_CONFIG_EXAMPLE:?}"; then
				echo "##WARNING## Couldn't delete Brick Mk2 Device Configuration Example executable"
			fi
			cd ..
		fi

		if [ -e ${ROUTER_MK2S_CONFIG_EXAMPLE_DIR:?} ]; then
			cd "${ROUTER_MK2S_CONFIG_EXAMPLE_DIR:?}"
			make clean
			if ! rm -f "" "../../../${PROGRAMS_DIR:?}/${ROUTER_MK2S_CONFIG_EXAMPLE:?}"; then
				echo "##WARNING## Couldn't delete Router Mk2S Device Configuration Example executable"
			fi
			cd ..
		fi
		
		cd ..
	fi
	
	if [ -e ${RMAP_EXAMPLE_DIR:?} ]; then
		cd "${RMAP_EXAMPLE_DIR:?}"
		make clean
		if ! rm -f "" "../../${PROGRAMS_DIR:?}/${RMAP_EXAMPLE:?}"; then
			echo "##WARNING## Couldn't delete RMAP Packet Library Example executable"
		fi
		cd ..
	fi
	
	cd ..
fi


# Removing the GUI applications
echo "Removing the GUI applications..."
for GUI_APP in ${GUI_APP_LIST:?}; do
	if ! rm -f "${PROGRAMS_DIR:?}/${GUI_APP:?}"; then
		echo "##WARNING## Couldn't delete $GUI_APP executable"
	fi
done


# Removing the CUBA Software
echo "Removing the CUBA Software..."
if ! rm -f "${PROGRAMS_DIR:?}/${CUBA_SOFTWARE:?}"; then
    echo "##WARNING## Couldn't delete $CUBA_SOFTWARE executable"
fi


# Remove the lib files from the correct directories
echo "Removing the lib files..."
LIB32_EXISTS="[ -e /usr/lib32/ ] && [ ! -L /usr/lib32 ]"
LIB64_EXISTS="[ -e /usr/lib64/ ] && [ ! -L /usr/lib64 ]"
LIB_NOT_LINK="[ ! -L /usr/lib ]"
for libfile in ${LIB_FILE_LIST:?}; do

	if eval $LIB32_EXISTS; then
		if [ -e /usr/lib32/${libfile:?} ]; then
			if rm -f /usr/lib32/${libfile:?}; then
				echo "Removed the 32-bit lib file ${libfile:?} from /usr/lib32/"
			else
				echo "##ERROR## Unable to remove the 32-bit lib file ${libfile:?} from /usr/lib32/"
				echo ""
            	failed="yes"
            fi
        fi
		if [ -e /usr/lib32/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} ]; then
			if rm -f /usr/lib32/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
				echo "Removed the 32-bit lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib32/"
			else
				echo "##ERROR## Unable to remove the 32-bit lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib32/"
				echo ""
	            failed="yes"
	        fi
		fi
	fi
	
	if eval $LIB64_EXISTS; then
		if [ -e /usr/lib64/${libfile:?} ]; then
			if rm -f /usr/lib64/${libfile:?}; then
				echo "Removed the 64-bit lib file ${libfile:?} from /usr/lib64/"
			else
				echo "##ERROR## Unable to remove the 32-bit lib file ${libfile:?} from /usr/lib64/"
				echo ""
            	failed="yes"
            fi
        fi
		if [ -e /usr/lib64/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} ]; then
			if rm -f /usr/lib64/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
				echo "Removed the 64-bit lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib64/"
			else
				echo "##ERROR## Unable to remove the 64-bit lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib64/"
				echo ""
	            failed="yes"
	        fi
		fi
	fi
	
	if eval $LIB_NOT_LINK; then
		if [ -e /usr/lib/${libfile:?} ]; then
			if rm -f /usr/lib/${libfile:?}; then
				echo "Removed the lib file ${libfile:?} from /usr/lib/"
			else
				echo "##ERROR## Unable to remove the lib file ${libfile:?} from /usr/lib/"
				echo ""
            	failed="yes"
            fi
        fi
		if [ -e /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} ]; then
			if rm -f /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
				echo "Removed the lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib/"
			else
				echo "##ERROR## Unable to remove the lib file ${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} from /usr/lib/"
				echo ""
	            failed="yes"
	        fi
		fi
	fi
	
done


# Update the links for the lib files
echo "Updating the run time bindings for the lib files..."
if eval $LIB32_EXISTS; then
	ldconfig -n /usr/lib32
fi
if eval $LIB64_EXISTS; then
	ldconfig -n /usr/lib64
fi
if eval $LIB_NOT_LINK; then
	ldconfig -n /usr/lib
fi


# Remove folders that were extracted by install script otherwise the 
# uninstaller may fail if not invoked as root.
if [ ! -z "$TARGET_DIR" ]; then
	cd ${TARGET_DIR:?}
fi
if rm -rf "apis" "application_notes" "doc" "driver_src" "examples" "inc" "lib" "scripts"; then
	echo ""
else
	echo "##ERROR## Unable to remove one or more directories"
	failed="yes"
fi

# Shortcut paths
SHORTCUTS_DIR=shortcuts
USR_DESKTOP_DIRECTORIES="/usr/share/desktop-directories"
USR_APPLICATIONS="/usr/share/applications"

# Shortcut files
STAR_DUNDEE_DIR="50A96A08-91F0-4E24-8A58-A3E46483A0ED-STAR-Dundee.directory"
STAR_SYSTEM_DIR="star-system.directory"
CUBA_DESKTOP="star-cuba.desktop"
DEVICE_CONFIG_DESKTOP="star-device-config.desktop"
DEVICE_UPDATE_DESKTOP="star-device-update.desktop"
ERROR_INJECT_DESKTOP="star-error-inject.desktop"
PERF_TESTER_DESKTOP="star-performance-tester.desktop"
RECEIVE_DESKTOP="star-receive.desktop"
RMAP_TARGET_DESKTOP="star-rmap-target.desktop"
SINK_DESKTOP="star-sink.desktop"
SOURCE_DESKTOP="star-source.desktop"
CPP_DOCS_DESKTOP="star-system-cpp-docs.desktop"
DOCS_DESKTOP="star-system-docs.desktop"
STAR_SYSTEM_TEST_DESKTOP="star-system-test.desktop"
STAR_SYSTEM_UNINSTALL_DESKTOP="star-system-uninstall.desktop"
TIME_CODE_DESKTOP="star-time-code.desktop"
TRANSMIT_DESKTOP="star-transmit.desktop"

# Installed shortcut files
USR_STAR_DUNDEE_DIR="${USR_DESKTOP_DIRECTORIES:?}/${STAR_DUNDEE_DIR:?}"
USR_STAR_SYSTEM_DIR="${USR_DESKTOP_DIRECTORIES:?}/${STAR_SYSTEM_DIR:?}"
USR_CUBA_DESKTOP="${USR_APPLICATIONS:?}/${CUBA_DESKTOP:?}"
USR_DEVICE_CONFIG_DESKTOP="${USR_APPLICATIONS:?}/${DEVICE_CONFIG_DESKTOP:?}"
USR_DEVICE_UPDATE_DESKTOP="${USR_APPLICATIONS:?}/${DEVICE_UPDATE_DESKTOP:?}"
USR_ERROR_INJECT_DESKTOP="${USR_APPLICATIONS:?}/${ERROR_INJECT_DESKTOP:?}"
USR_PERF_TESTER_DESKTOP="${USR_APPLICATIONS:?}/${PERF_TESTER_DESKTOP:?}"
USR_RECEIVE_DESKTOP="${USR_APPLICATIONS:?}/${RECEIVE_DESKTOP:?}"
USR_RMAP_TARGET_DESKTOP="${USR_APPLICATIONS:?}/${RMAP_TARGET_DESKTOP:?}"
USR_SINK_DESKTOP="${USR_APPLICATIONS:?}/${SINK_DESKTOP:?}"
USR_SOURCE_DESKTOP="${USR_APPLICATIONS:?}/${SOURCE_DESKTOP:?}"
USR_CPP_DOCS_DESKTOP="${USR_APPLICATIONS:?}/${CPP_DOCS_DESKTOP:?}"
USR_DOCS_DESKTOP="${USR_APPLICATIONS:?}/${DOCS_DESKTOP:?}"
USR_STAR_SYSTEM_TEST_DESKTOP="${USR_APPLICATIONS:?}/${STAR_SYSTEM_TEST_DESKTOP:?}"
USR_STAR_SYSTEM_UNINSTALL_DESKTOP="${USR_APPLICATIONS:?}/${STAR_SYSTEM_UNINSTALL_DESKTOP:?}"
USR_TIME_CODE_DESKTOP="${USR_APPLICATIONS:?}/${TIME_CODE_DESKTOP:?}"
USR_TRANSMIT_DESKTOP="${USR_APPLICATIONS:?}/${TRANSMIT_DESKTOP:?}"

# Command for uninstalling shortcuts
SHORTCUT_UNINSTALL_CMD="xdg-desktop-menu uninstall"

# Remove shortcuts
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_CUBA_DESKTOP:?}; then
	echo "Removed ${USR_CUBA_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_CUBA_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DEVICE_CONFIG_DESKTOP:?}; then
	echo "Removed ${USR_DEVICE_CONFIG_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_DEVICE_CONFIG_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DEVICE_UPDATE_DESKTOP:?}; then
	echo "Removed ${USR_DEVICE_UPDATE_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_DEVICE_UPDATE_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_ERROR_INJECT_DESKTOP:?}; then
	echo "Removed ${USR_ERROR_INJECT_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_ERROR_INJECT_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_PERF_TESTER_DESKTOP:?}; then
	echo "Removed ${USR_PERF_TESTER_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_PERF_TESTER_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_RECEIVE_DESKTOP:?}; then
	echo "Removed ${USR_RECEIVE_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_RECEIVE_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_RMAP_TARGET_DESKTOP:?}; then
	echo "Removed ${USR_RMAP_TARGET_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_RMAP_TARGET_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_SINK_DESKTOP:?}; then
	echo "Removed ${USR_SINK_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_SINK_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_SOURCE_DESKTOP:?}; then
	echo "Removed ${USR_SOURCE_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_SOURCE_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_CPP_DOCS_DESKTOP:?}; then
	echo "Removed ${USR_CPP_DOCS_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_CPP_DOCS_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DOCS_DESKTOP:?}; then
	echo "Removed ${USR_DOCS_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_DOCS_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_STAR_SYSTEM_TEST_DESKTOP:?}; then
	echo "Removed ${USR_STAR_SYSTEM_TEST_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_STAR_SYSTEM_TEST_DESKTOP:?} shortcut"
	failed="yes"
fi	
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?}; then
	echo "Removed ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_TIME_CODE_DESKTOP:?}; then
	echo "Removed ${USR_TIME_CODE_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_TIME_CODE_DESKTOP:?} shortcut"
	failed="yes"
fi
if ${SHORTCUT_UNINSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_TRANSMIT_DESKTOP:?}; then
	echo "Removed ${USR_TRANSMIT_DESKTOP:?} shortcut"
else
	echo "##ERROR## Couldn't remove ${USR_TRANSMIT_DESKTOP:?} shortcut"
	failed="yes"
fi
if xdg-desktop-menu forceupdate; then
	echo "Removed desktop shortcuts"
else
	echo "##ERROR## Couldn't remove desktop shortcuts"
	failed="yes"
fi

# Abort now on soft failure
if [ "$failed" = "yes" ]; then
	exit 1
fi


echo "Successfully removed STAR-System"