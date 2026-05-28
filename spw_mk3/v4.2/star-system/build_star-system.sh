#!/bin/sh

#
# STAR-System install script for Linux, to build the STAR-System components and
# copy them from/to the appropriate locations.
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

# Get arguments
INSTALL_QT4=0
INSTALL_QT5=0
INSTALL_EXAMPLES=0
INSTALL_DOC=0
INSTALL_PCI_DRIVER=1
INSTALL_USB_DRIVER=1
INSTALL_CPP_DOC=0
for arg in "$@"
do
    if [ "${arg:?}" = "--qt4" ]; then
		INSTALL_QT4=1
	elif [ "${arg:?}" = "--qt5" ]; then
		INSTALL_QT5=1
	elif [ "${arg:?}" = "--examples" ]; then
		INSTALL_EXAMPLES=1
	elif [ "${arg:?}" = "--doc" ]; then
		INSTALL_DOC=1
	elif [ "${arg:?}" = "--no-pci-driver" ]; then
		INSTALL_PCI_DRIVER=0
	elif [ "${arg:?}" = "--no-usb-driver" ]; then
		INSTALL_USB_DRIVER=0
	elif [ "${arg:?}" = "--cpp-doc" ]; then
		INSTALL_CPP_DOC=1
	fi
done

# The version of STAR-System
MAJOR_VERSION=4
MINOR_VERSION=02

# Properties of the system
STAR_SYSTEM_CPU="garbage"				# CPU type in use
STAR_SYSTEM_REGPARM="garbage"			# Whether kernel has -mregparm flag set
STAR_SYSTEM_TMP_DRIVER=".tmp_star_system_driver"	# Location of temp driver
KVERSION=`uname -r`						# Current kernel version
KDIR=/lib/modules/${KVERSION:?}/build	# Kernel's modules directory 
PWD=`/bin/pwd`							# Current directory

# Locations and names of files in the install directory
DRIVERS_DIR=driver_src					# Location of the driver sources
PCI_DRIVER_DIR=pci_driver				# Location of the PCI driver source
USB_DRIVER_DIR=usb_driver				# Location of the USB driver source
# Pre-built object files for the PCI driver 
PCI_MODULE_FILES_LIST="pcimk2_driver pcimk2_ioctl pcimk2_interrupts pcimk2_dma pcimk2_fileops pcimk2_buffers pcimk2_registers"
# Pre-built object files for the USB driver 
USB_MODULE_FILES_LIST="spwusb_driver spwusb_ioctl spwusb_fileops spwusb_buffers spwusb_datahandling"
PCI_DRIVER=star_spw_pci						# Name of the PCI driver module
USB_DRIVER=star_spw_usb						# Name of the USB driver module
STAR_SYSTEM_LIB32_DIR=lib/x86-32			# Location of the 32-bit libs
STAR_SYSTEM_LIB64_DIR=lib/x86-64			# Location of the 64-bit libs
STAR_SYSTEM_LIB32_QT5_DIR=lib/x86-32/Qt5	# Location of the 32-bit Qt5 GUI applications
STAR_SYSTEM_LIB64_QT5_DIR=lib/x86-64/Qt5	# Location of the 64-bit Qt5 GUI applications
STAR_API_LIB=libstar-api.so					# Name of the STAR-API lib
RMAP_LIB=librmap_packet_library.so			# Name of the RMAP Packet Library lib
RMAP_INITIATOR_LIB=libstar_rmap_initiator.so	# Name of the RMAP Initiator lib
MK2_CONFIG_LIB=libstar_conf_api_mk2.so	# Name of the Mk2 Config lib
PCIMK2_CONFIG_LIB=libstar_conf_api_pci_mk2.so	# Name of the PCI Mk2 Config lib
PXI_CONFIG_LIB=libstar_conf_api_pxi.so	# Name of the PXI Config lib
SPFI_CONFIG_LIB=libstar_conf_api_spfi_router.so	# Name of the SpFi router Config lib
BRICKMK3_CONFIG_LIB=libstar_conf_api_brick_mk3.so	# Name of the Brick Mk3 Config lib
GBEBRICK_CONFIG_LIB=libstar_conf_api_gbe_brick.so	# Name of the GbE Brick Config lib
BRICKMK2_CONFIG_LIB=libstar_conf_api_brick_mk2.so	# Name of the Brick Mk2 Config lib
ROUTERMK2S_CONFIG_LIB=libstar_conf_api_router_mk2s.so	# Name of the Router Mk2S Config lib
ROUTER_CONFIG_LIB=libstar_conf_api_router.so	# Name of the Router Config lib
CASTOR_CONFIG_LIB=libstar_conf_api_castor.so	# Name of the CASTOR Config lib
GENERIC_CONFIG_LIB=libstar_conf_api_generic.so	# Name of the generic Config lib
PCIMK2_PACKET_SUBSYS_LIB=libstar_packet_subsystem.so	# Name of the PCI Mk2 packet subsystem lib
CONFIG_MESSENGER_LIB=libstar_conf_mssgr.so	# Name of the Config Messenger lib
RMAP_TARGET_LIB=libstar_rmap_target.so	# Name of the RMAP Target lib
TRIGGERING_LIB=libstar_triggering.so	# Name of the Triggering lib
TCP_DRIVER_LIB=star_tcp_driver.so	# Name of the TCP/IP driver lib
# The list of lib files
LIB_FILE_LIST="${STAR_API_LIB:?} ${RMAP_LIB:?} ${RMAP_INITIATOR_LIB:?} ${MK2_CONFIG_LIB:?} ${PCIMK2_CONFIG_LIB:?} ${PXI_CONFIG_LIB:?} ${SPFI_CONFIG_LIB:?} ${BRICKMK2_CONFIG_LIB:?} ${BRICKMK3_CONFIG_LIB:?} ${GBEBRICK_CONFIG_LIB:?} ${ROUTERMK2S_CONFIG_LIB:?} ${ROUTER_CONFIG_LIB:?} ${CASTOR_CONFIG_LIB:?} ${GENERIC_CONFIG_LIB:?} ${PCIMK2_PACKET_SUBSYS_LIB:?} ${CONFIG_MESSENGER_LIB:?} ${RMAP_TARGET_LIB:?} ${TRIGGERING_LIB:?} ${TCP_DRIVER_LIB:?}"
EXAMPLES_DIR=examples					# Name of directory containing examples
INCLUDES_DIR=inc    					# Name of directory containing includes
PROGRAMS_DIR=bin						# Name of directory containing binaries
STAR_SYSTEM_TEST_DIR=star_system_test	# Name of STAR-System Test directory
STAR_SYSTEM_TEST=bin/star-system_test	# Name of STAR-System Test binary
PERF_TESTER_DIR=performance_tester		# Name of Performance Tester directory
PERF_TESTER=bin/star_performance_tester	# Name of Performance Tester binary
TIME_CODE_TEST_DIR=time-code_test		# Name of STAR-System Test directory
TIME_CODE_TEST=bin/time-code_test		# Name of STAR-System Test binary
CONFIG_EXAMPLE_DIR=config				# Name of Config API examples directory
PCI_MK2_CONFIG_EXAMPLE_DIR=pciMk2_configuration		# Name of PCI Mk2 Config API example directory
PCI_MK2_CONFIG_EXAMPLE=bin/pciMk2Configuration	# Name of PCI Mk2 Config API example binary
BRICK_MK2_CONFIG_EXAMPLE_DIR=brickMk2_configuration		# Name of Brick Mk2 Config API example directory
BRICK_MK2_CONFIG_EXAMPLE=bin/brickMk2Configuration	# Name of Brick Mk2 Config API example binary
ROUTER_MK2S_CONFIG_EXAMPLE_DIR=routerMk2S_configuration		# Name of Router Mk2S Config API example directory
ROUTER_MK2S_CONFIG_EXAMPLE=bin/routerMk2SConfiguration	# Name of Router Mk2S Config API example binary
MK2_CONFIG_EXAMPLE_DIR=mk2_configuration		# Name of Mk2 Config API example directory
MK2_CONFIG_EXAMPLE=bin/mk2_configuration_example	# Name of Mk2 Config API example binary
PACKET_SUBSYSTEM_EXAMPLE_DIR=packet_subsystem		# Name of Packet Subsystem example directory
PACKET_SUBSYSTEM_EXAMPLE=bin/packet_subsystem_example	# Name of Packet Subsystem example binary
DEVICE_IDENTIFIER_EXAMPLE_DIR=device_identifier		# Name of Device Identifier example directory
DEVICE_IDENTIFIER_EXAMPLE=bin/device_identifier_example	# Name of Device Identifier example binary
PORT_CONTROL_STATUS_EXAMPLE_DIR=port_control_status		# Name of Port Control Status example directory
PORT_CONTROL_STATUS_EXAMPLE=bin/port_control_status_example	# Name of Port Control Status example binary
ROUTER_CONFIG_EXAMPLE_DIR=router_configuration		# Name of Router Configuration example directory
ROUTER_CONFIG_EXAMPLE=bin/router_configuration_example	# Name of Router Configuration example binary
ROUTING_TABLE_EXAMPLE_DIR=routing_table		# Name of Routing Table example directory
ROUTING_TABLE_EXAMPLE=bin/routing_table_example	# Name of Routing Table example binary
USER_REGISTERS_EXAMPLE_DIR=user_registers		# Name of User Registers example directory
USER_REGISTERS_EXAMPLE=bin/user_registers_example	# Name of User Registers example binary
RMAP_EXAMPLE_DIR=rmap					# Name of RMAP API example directory
RMAP_EXAMPLE=bin/rmap_examples			# Name of RMAP API example binary
LINK_EVENTS_EXAMPLE_DIR=link_events		# Name of Link Events example directory
LINK_EVENTS_EXAMPLE=bin/link_events		# Name of Link Events example binary
API_TEST_APP_EXAMPLE_DIR=api_test_app		# Name of API Test App example directory
API_TEST_APP_EXAMPLE=bin/api_test_app		# Name of API Test App example binary
RMAP_TARGET_EXAMPLE_DIR=rmap_target		# Name of RMAP Target example directory
RMAP_TARGET_EXAMPLE=bin/rmap_target		# Name of RMAP Target example binary
TIMESTAMP_TEST_EXAMPLE_DIR=timestamp_test	# Name of Timestamp example directory
TIMESTAMP_TEST_EXAMPLE=bin/timestamp_test	# Name of Timestamp example binary
TRIGGERING_EXAMPLE_DIR=triggering		# Name of Triggering example directory
TRIGGERING_EXAMPLE=bin/triggering_example	# Name of Triggering example binary
DEVICE_CONFIG_EXE=star_conf_service		# Name of the Device Config executable 
SCRIPTS_DIR=scripts						# Name of directory containing scripts
DEVICE_CONFIG_SCRIPT=star_device_config	# Name of the Device Config script 
RULES_DIR=/etc/udev/rules.d/			# The directory to put the rules
PCI_RULES_SCRIPT=99-star_spw_pci.rules	# Name of the PCI rules script
USB_RULES_SCRIPT=99-star_spw_usb.rules	# Name of the USB rules script

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
QT5_GUI_APP_LIST="${DEVICE_CONFIG_GUI:?} ${RECEIVE_GUI:?} ${TRANSMIT_GUI:?} ${SINK_GUI:?} ${SOURCE_GUI:?} ${DEVICE_UPDATE_GUI:?} ${ERROR_INJECTION_GUI:?} ${TIME_CODE_GUI:?} ${RMAP_TARGET_GUI:?}"

CUBA_SOFTWARE="star_cuba"               # Name of the CUBA Software application

# Report STAR-System version
echo "Building STAR-System v${MAJOR_VERSION:?}.${MINOR_VERSION:?}..."
echo

# Check the CPU type
echo "Checking CPU architecture..."
STAR_SYSTEM_CPU=`uname -m | sed "s/i.86/x86-32/" | sed "s/x86_64/x86-64/"`
echo "CPU = ${STAR_SYSTEM_CPU:?}"
echo


# Copy the lib files from/to the correct directories
echo "Installing the lib files..."
LIB32_EXISTS="[ -e /usr/lib32/ ] && [ ! -L /usr/lib32 ]"
LIB64_EXISTS="[ -e /usr/lib64/ ] && [ ! -L /usr/lib64 ]"
LIB_NOT_LINK="[ ! -L /usr/lib ]"
for libfile in ${LIB_FILE_LIST:?}; do

	if eval $LIB32_EXISTS; then
		if cp -f -v ${STAR_SYSTEM_LIB32_DIR:?}/${libfile:?} /usr/lib32/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
			ln -sf /usr/lib32/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} /usr/lib32/${libfile:?}
			echo "Copied the 32-bit lib file ${libfile:?} to /usr/lib32/"
		else
			echo "##ERROR## Couldn't copy the 32-bit lib file ${libfile:?} to /usr/lib32/"
			exit 1
		fi
	fi
	
	if eval $LIB64_EXISTS; then
		if cp -f -v ${STAR_SYSTEM_LIB64_DIR:?}/${libfile:?} /usr/lib64/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
			ln -sf /usr/lib64/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} /usr/lib64/${libfile:?}
			echo "Copied the 64-bit lib file ${libfile:?} to /usr/lib64/"
		else
			echo "##ERROR## Couldn't copy the 64-bit lib file ${libfile:?} to /usr/lib64/"
			exit 1
		fi
	fi
	
	if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ] && eval $LIB_NOT_LINK; then
		if cp -f -v ${STAR_SYSTEM_LIB32_DIR:?}/${libfile:?} /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
			ln -sf /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} /usr/lib/${libfile:?}
			echo "Copied the 32-bit lib file ${libfile:?} to /usr/lib/"
		else
			echo "##ERROR## Couldn't copy the 32-bit lib file ${libfile:?} to /usr/lib/"
			exit 1
		fi
	elif [ "${STAR_SYSTEM_CPU:?}" = "x86-64" ] && eval $LIB_NOT_LINK; then
		if eval $LIB64_EXISTS && ! eval $LIB32_EXISTS; then
			if cp -f -v ${STAR_SYSTEM_LIB32_DIR:?}/${libfile:?} /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
				ln -sf /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} /usr/lib/${libfile:?}
				echo "Copied the 32-bit lib file ${libfile:?} to /usr/lib/"
			else
				echo "##ERROR## Couldn't copy the 32-bit lib file ${libfile:?} to /usr/lib/"
				exit 1
			fi
		else
			if cp -f -v ${STAR_SYSTEM_LIB64_DIR:?}/${libfile:?} /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?}; then
				ln -sf /usr/lib/${libfile:?}.${MAJOR_VERSION:?}.${MINOR_VERSION:?} /usr/lib/${libfile:?}
				echo "Copied the 64-bit lib file ${libfile:?} to /usr/lib/"
			else
				echo "##ERROR## Couldn't copy the 64-bit lib file ${libfile:?} to /usr/lib/"
				exit 1
			fi
		fi
	fi
done
echo


# Create the links for the lib files
echo "Updating the run time bindings for the lib files..."
if eval $LIB32_EXISTS; then
	/sbin/ldconfig -n /usr/lib32
fi
if eval $LIB64_EXISTS; then
	/sbin/ldconfig -n /usr/lib64
fi
if eval $LIB_NOT_LINK; then
	/sbin/ldconfig -n /usr/lib
fi
echo


# Copy applications and examples
if mkdir -p "${PROGRAMS_DIR:?}"; then

	if [ ${INSTALL_EXAMPLES:?} = 1 ]; then

		if [ -d "${INCLUDES_DIR:?}" ]; then

			# Build the test programs
			echo "Building the test programs..."
			
			cd "${EXAMPLES_DIR:?}"
			
			cd "${STAR_SYSTEM_TEST_DIR:?}"
			make clean
			make
			if ! cp -f -v "${STAR_SYSTEM_TEST:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build STAR-System Test executable"
			fi
			
			cd "../${PERF_TESTER_DIR:?}"
			make clean
			make
			if ! cp -f -v "${PERF_TESTER:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Performance Tester executable"
			fi
			
			cd "../${TIME_CODE_TEST_DIR:?}"
			make clean
			make
			if ! cp -f -v "${TIME_CODE_TEST:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Time-code Test executable"
			fi
			
			cd "../${CONFIG_EXAMPLE_DIR:?}/${PCI_MK2_CONFIG_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${PCI_MK2_CONFIG_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build PCI Mk2 Device Configuration Example executable"
			fi
			
			cd "../${BRICK_MK2_CONFIG_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${BRICK_MK2_CONFIG_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Brick Mk2 Device Configuration Example executable"
			fi
			
			cd "../${ROUTER_MK2S_CONFIG_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${ROUTER_MK2S_CONFIG_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Router Mk2S Device Configuration Example executable"
			fi
			
			cd "../${MK2_CONFIG_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${MK2_CONFIG_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Mk2 Device Configuration Example executable"
			fi
			
			cd "../${PACKET_SUBSYSTEM_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${PACKET_SUBSYSTEM_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Packet Subsystem Example executable"
			fi
			
			cd "../${DEVICE_IDENTIFIER_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${DEVICE_IDENTIFIER_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Device Identifier Example executable"
			fi
			
			cd "../${PORT_CONTROL_STATUS_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${PORT_CONTROL_STATUS_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Port Control Status Example executable"
			fi
			
			cd "../${ROUTER_CONFIG_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${ROUTER_CONFIG_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Router Config Example executable"
			fi
			
			cd "../${ROUTING_TABLE_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${ROUTING_TABLE_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Routing Table Example executable"
			fi
			
			cd "../${USER_REGISTERS_EXAMPLE_DIR:?}"
			make clean
			make
			if ! cp -f -v "${USER_REGISTERS_EXAMPLE:?}" "../../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build User Registers Example executable"
			fi
			
			cd ../../${RMAP_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${RMAP_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build RMAP Packet Library Example executable"
			fi
			
			cd ../${LINK_EVENTS_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${LINK_EVENTS_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Link Events Example executable"
			fi

			cd ../${API_TEST_APP_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${API_TEST_APP_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build API Test App Example executable"
			fi

			cd ../${RMAP_TARGET_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${RMAP_TARGET_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build RMAP Target Example executable"
			fi

			cd ../${TIMESTAMP_TEST_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${TIMESTAMP_TEST_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Timestamp Example executable"
			fi

			cd ../${TRIGGERING_EXAMPLE_DIR:?}
			make clean
			make
			if ! cp -f -v "${TRIGGERING_EXAMPLE:?}" "../../${PROGRAMS_DIR:?}"; then
				echo "##WARNING## Couldn't build Triggering Example executable"
			fi
			
			cd ../../
		
		fi
	
	else

		rm -rf ${EXAMPLES_DIR:?}
	
	fi
	
	if [ ${INSTALL_QT4:?} = 1 ]; then
	
	    # Copying the Qt4 GUI applications
    	echo "Copying the Qt4 GUI applications..."

	    for GUI_APP in ${GUI_APP_LIST:?}; do
		    if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ]; then
			    if ! cp -f -v "${STAR_SYSTEM_LIB32_DIR:?}/${GUI_APP:?}" "${PROGRAMS_DIR:?}/"; then
				    echo "##WARNING## Unable to copy 32-bit $GUI_APP to destination directory"
			    fi
		    else
			    if ! cp -f -v "${STAR_SYSTEM_LIB64_DIR:?}/${GUI_APP:?}" "${PROGRAMS_DIR:?}/"; then
				    echo "##WARNING## Unable to copy 64-bit $GUI_APP to destination directory"
			    fi
		    fi
	    done
	
	fi
	
	if [ ${INSTALL_QT5:?} = 1 ]; then
	
	    # Copying the Qt5 GUI applications
    	echo "Copying the Qt5 GUI applications..."

	    for GUI_APP in ${QT5_GUI_APP_LIST:?}; do
		    if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ]; then
			    if ! cp -f -v "${STAR_SYSTEM_LIB32_QT5_DIR:?}/${GUI_APP:?}" "${PROGRAMS_DIR:?}/"; then
				    echo "##WARNING## Unable to copy 32-bit $GUI_APP to destination directory"
			    fi
		    else
			    if ! cp -f -v "${STAR_SYSTEM_LIB64_QT5_DIR:?}/${GUI_APP:?}" "${PROGRAMS_DIR:?}/"; then
				    echo "##WARNING## Unable to copy 64-bit $GUI_APP to destination directory"
			    fi
		    fi
	    done
	
	fi
	
	# Copying the CUBA Software
	echo "Copying the CUBA Software..."

    if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ]; then
        if ! cp -f -v "${STAR_SYSTEM_LIB32_DIR:?}/${CUBA_SOFTWARE:?}" "${PROGRAMS_DIR:?}/"; then
            echo "##WARNING## Unable to copy 32-bit $CUBA_SOFTWARE to destination directory"
        fi
    else
        if ! cp -f -v "${STAR_SYSTEM_LIB64_DIR:?}/${CUBA_SOFTWARE:?}" "${PROGRAMS_DIR:?}/"; then
            echo "##WARNING## Unable to copy 64-bit $CUBA_SOFTWARE to destination directory"
        fi
    fi
	
	# Shortcut paths
	SHORTCUTS_DIR=shortcuts
	USR_DESKTOP_DIRECTORIES="/usr/share/desktop-directories"
	USR_APPLICATIONS="/usr/share/applications"
	
	# Shortcuts only exist during initial run during installation
	SHORTCUTS_EXISTS="[ -e ${SHORTCUTS_DIR:?} ]"
	if eval $SHORTCUTS_EXISTS; then
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
		
		# Install shortcut resources
		mv "${SHORTCUTS_DIR:?}/STAR.png" .
		mv "${SHORTCUTS_DIR:?}/launch_application.sh" .
		mv "${SHORTCUTS_DIR:?}/launch_cpp_docs.sh" .
		mv "${SHORTCUTS_DIR:?}/launch_docs.sh" .
		mv "${SHORTCUTS_DIR:?}/launch_star_cuba.sh" .
		mv "${SHORTCUTS_DIR:?}/launch_star_performance_tester.sh" .
		mv "${SHORTCUTS_DIR:?}/launch_star-system_test.sh" .

		# Set shortcut resource permissions
		chmod 755 "launch_application.sh" \
				  "launch_cpp_docs.sh"\
				  "launch_docs.sh"\
				  "launch_star_cuba.sh"\
				  "launch_star_performance_tester.sh"\
				  "launch_star-system_test.sh"

		# Install desktop entries
		mv "${SHORTCUTS_DIR:?}/${STAR_DUNDEE_DIR:?}" "${USR_DESKTOP_DIRECTORIES:?}"
		mv "${SHORTCUTS_DIR:?}/${STAR_SYSTEM_DIR:?}" "${USR_DESKTOP_DIRECTORIES:?}"
		mv "${SHORTCUTS_DIR:?}/${CUBA_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${DEVICE_CONFIG_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${DEVICE_UPDATE_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${ERROR_INJECT_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${PERF_TESTER_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${RECEIVE_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${RMAP_TARGET_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${SINK_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${SOURCE_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
			mv "${SHORTCUTS_DIR:?}/${CPP_DOCS_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		fi
		if [ ${INSTALL_DOC:?} = 1 ]; then
			mv "${SHORTCUTS_DIR:?}/${DOCS_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		fi
		mv "${SHORTCUTS_DIR:?}/${STAR_SYSTEM_TEST_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${STAR_SYSTEM_UNINSTALL_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${TIME_CODE_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		mv "${SHORTCUTS_DIR:?}/${TRANSMIT_DESKTOP:?}" "${USR_APPLICATIONS:?}"
		rm -rf "${SHORTCUTS_DIR:?}/"

		# Set desktop entry permissions
		chmod 755 "${USR_STAR_DUNDEE_DIR:?}"\
				  "${USR_STAR_SYSTEM_DIR:?}"\
				  "${USR_CUBA_DESKTOP:?}"\
				  "${USR_DEVICE_CONFIG_DESKTOP:?}"\
				  "${USR_DEVICE_UPDATE_DESKTOP:?}"\
				  "${USR_ERROR_INJECT_DESKTOP:?}"\
				  "${USR_PERF_TESTER_DESKTOP:?}"\
				  "${USR_RECEIVE_DESKTOP:?}"\
				  "${USR_RMAP_TARGET_DESKTOP:?}"\
				  "${USR_SINK_DESKTOP:?}"\
				  "${USR_SOURCE_DESKTOP:?}"\
				  "${USR_STAR_SYSTEM_TEST_DESKTOP:?}"\
				  "${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?}"\
				  "${USR_TIME_CODE_DESKTOP:?}"\
				  "${USR_TRANSMIT_DESKTOP:?}"
		if [ ${INSTALL_DOC:?} = 1 ]; then
			chmod 755 "${USR_CPP_DOCS_DESKTOP:?}"
		fi
		if [ ${INSTALL_DOC:?} = 1 ]; then
			chmod 755 "${USR_DOCS_DESKTOP:?}"
		fi
		
		# Command for installing shortcuts
		SHORTCUT_INSTALL_CMD="xdg-desktop-menu install --novendor --noupdate"
				  
		# Install shortcuts
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_CUBA_DESKTOP:?}; then
			echo "Updated ${USR_CUBA_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_CUBA_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DEVICE_CONFIG_DESKTOP:?}; then
			echo "Updated ${USR_DEVICE_CONFIG_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_DEVICE_CONFIG_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DEVICE_UPDATE_DESKTOP:?}; then
			echo "Updated ${USR_DEVICE_UPDATE_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_DEVICE_UPDATE_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_ERROR_INJECT_DESKTOP:?}; then
			echo "Updated ${USR_ERROR_INJECT_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_ERROR_INJECT_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_PERF_TESTER_DESKTOP:?}; then
			echo "Updated ${USR_PERF_TESTER_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_PERF_TESTER_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_RECEIVE_DESKTOP:?}; then
			echo "Updated ${USR_RECEIVE_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_RECEIVE_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_RMAP_TARGET_DESKTOP:?}; then
			echo "Updated ${USR_RMAP_TARGET_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_RMAP_TARGET_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_SINK_DESKTOP:?}; then
			echo "Updated ${USR_SINK_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_SINK_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_SOURCE_DESKTOP:?}; then
			echo "Updated ${USR_SOURCE_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_SOURCE_DESKTOP:?} shortcut"
			exit 1
		fi
		if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
			if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_CPP_DOCS_DESKTOP:?}; then
				echo "Updated ${USR_CPP_DOCS_DESKTOP:?} shortcut"
			else
				echo "##ERROR## Couldn't update ${USR_CPP_DOCS_DESKTOP:?} shortcut"
				exit 1
			fi
		fi
		if [ ${INSTALL_DOC:?} = 1 ]; then
			if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_DOCS_DESKTOP:?}; then
				echo "Updated ${USR_DOCS_DESKTOP:?} shortcut"
			else
				echo "##ERROR## Couldn't update ${USR_DOCS_DESKTOP:?} shortcut"
				exit 1
			fi
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_STAR_SYSTEM_TEST_DESKTOP:?}; then
			echo "Updated ${USR_STAR_SYSTEM_TEST_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_STAR_SYSTEM_TEST_DESKTOP:?} shortcut"
			exit 1
		fi	
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?}; then
			echo "Updated ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_STAR_SYSTEM_UNINSTALL_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_TIME_CODE_DESKTOP:?}; then
			echo "Updated ${USR_TIME_CODE_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_TIME_CODE_DESKTOP:?} shortcut"
			exit 1
		fi
		if ${SHORTCUT_INSTALL_CMD:?} ${USR_STAR_DUNDEE_DIR:?} ${USR_STAR_SYSTEM_DIR:?} ${USR_TRANSMIT_DESKTOP:?}; then
			echo "Updated ${USR_TRANSMIT_DESKTOP:?} shortcut"
		else
			echo "##ERROR## Couldn't update ${USR_TRANSMIT_DESKTOP:?} shortcut"
			exit 1
		fi
		if xdg-desktop-menu forceupdate; then
			echo "Updated desktop shortcuts"
		else
			echo "##ERROR## Couldn't update desktop shortcuts"
			exit 1
		fi
	fi

else
	echo "##WARNING## Couldn't create ${PROGRAMS_DIR:?} directory"
fi
echo


# Check whether the kernel uses the regparm option
if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ]; then
	echo "Checking for regparm option in kernel..."
	mkdir -p "${STAR_SYSTEM_TMP_DRIVER:?}"
	cd "${STAR_SYSTEM_TMP_DRIVER:?}"
	echo "obj-m := test.o" > Makefile
	echo "#include <linux/module.h>" > test.c
	echo "#include <linux/kernel.h>" >> test.c
	echo "void init_module(void) { return 0; }" >> test.c
	echo "void cleanup_module(void) { }" >> test.c
	if make -n -C $KDIR SUBDIRS=$PWD modules | grep regparm=3 > /dev/null 2>/dev/null; then
		STAR_SYSTEM_REGPARM="/regparm"
	else
		STAR_SYSTEM_REGPARM="/noregparm"
	fi
	cd ..
	rm -rf "${STAR_SYSTEM_TMP_DRIVER:?}"
else
	STAR_SYSTEM_REGPARM=""
fi

echo "REGPARM = $STAR_SYSTEM_REGPARM"
echo


# Make the drivers

if [ -d "${DRIVERS_DIR:?}" ]; then

	cd "${DRIVERS_DIR:?}"

	if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then

		# Make the PCI driver
		echo "Building the PCI driver..."
		cd "${PCI_DRIVER_DIR:?}"
		make -f Kbuild clean
		for OBJECT_FILE in ${PCI_MODULE_FILES_LIST:?}; do
			if cp -f -v "objects/${STAR_SYSTEM_CPU:?}$STAR_SYSTEM_REGPARM/${OBJECT_FILE:?}.o_shipped" .; then
				echo "Copied the PCI Driver object file ${OBJECT_FILE:?} to the build directory"
			else
				echo "##ERROR## Couldn't copy the PCI Driver object file ${OBJECT_FILE:?} to the build directory"
				exit 1
			fi
		done
		make -f Kbuild
		if cp -f -v ${PCI_DRIVER:?}.ko ../; then
			echo "PCI driver built"
		else
			echo "ERROR: Couldn't build PCI driver"
			exit 1
		fi
		cd ..
		echo
		
	fi

	if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then

		# Make the USB driver
		echo "Building the USB driver..."
		cd "${USB_DRIVER_DIR:?}"
		make -f Kbuild clean
		for OBJECT_FILE in ${USB_MODULE_FILES_LIST:?}; do
			if cp -f -v "objects/${STAR_SYSTEM_CPU:?}$STAR_SYSTEM_REGPARM/${OBJECT_FILE:?}.o_shipped" .; then
				echo "Copied the USB Driver object file ${OBJECT_FILE:?} to the build directory"
			else
				echo "##ERROR## Couldn't copy the USB Driver object file ${OBJECT_FILE:?} to the build directory"
				exit 1
			fi
		done
		make -f Kbuild
		if cp -f -v ${USB_DRIVER:?}.ko ../; then
			echo "USB driver built"
		else
			echo "ERROR: Couldn't build USB driver"
			exit 1
		fi
		cd ..
		echo

	fi

	cd ..
	
fi

# Remove old PCI driver module
NEED_REMOVE=`/sbin/lsmod | grep -c ${PCI_DRIVER:?}`
if [ ${NEED_REMOVE:?} -ne 0 ]; then
	/sbin/rmmod ${PCI_DRIVER:?}
fi

# Remove old USB driver module
NEED_REMOVE=`/sbin/lsmod | grep -c ${USB_DRIVER:?}`
if [ ${NEED_REMOVE:?} -ne 0 ]; then
	/sbin/rmmod ${USB_DRIVER:?}
fi

# Rebuild dependency tree to include the driver modules
if /sbin/depmod; then
	echo "Rebuilt dependency tree"
else
	echo "##ERROR## Couldn't rebuild dependency tree"
	exit 1
fi


if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then

	# Load the PCI driver module
	if /sbin/modprobe ${PCI_DRIVER:?}; then
		echo "PCI driver module loaded"
	else
		echo "##ERROR## Couldn't load PCI driver module"
		exit 1
	fi

fi

if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then

	# Load the USB driver module
	if /sbin/modprobe ${USB_DRIVER:?}; then
		echo "USB driver module loaded"
	else
		echo "##ERROR## Couldn't load USB driver module"
		exit 1
	fi

fi


# Copy the Device Configuration Service to /sbin/
echo "Copying the Device Configuration Service and creating links..."
if [ "${STAR_SYSTEM_CPU:?}" = "x86-32" ]; then
	echo "${STAR_SYSTEM_LIB32_DIR:?}/${DEVICE_CONFIG_EXE:?}"
	if cp -f -v "${STAR_SYSTEM_LIB32_DIR:?}/${DEVICE_CONFIG_EXE:?}" "/sbin/"; then
		echo "Copied ${STAR_SYSTEM_LIB32_DIR:?}/${DEVICE_CONFIG_EXE:?} to /sbin/"
	else
		echo "##ERROR## Unable to copy ${STAR_SYSTEM_LIB32_DIR:?}/${DEVICE_CONFIG_EXE:?} to /sbin/"
		exit 1
	fi
else
	echo "${STAR_SYSTEM_LIB64_DIR:?}/${DEVICE_CONFIG_EXE:?}"
	if cp -f -v "${STAR_SYSTEM_LIB64_DIR:?}/${DEVICE_CONFIG_EXE:?}" "/sbin/"; then
		echo "Copied ${STAR_SYSTEM_LIB64_DIR:?}/${DEVICE_CONFIG_EXE:?} to /sbin/"
	else
		echo "##ERROR## Unable to copy ${STAR_SYSTEM_LIB64_DIR:?}/${DEVICE_CONFIG_EXE:?} to /sbin/"
		exit 1
	fi
fi


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

	# Copy the Device Configuration Service script into init.d
	if cp -f -v ${SCRIPTS_DIR:?}/${DEVICE_CONFIG_SCRIPT:?} "${INST_ETC:?}/init.d"; then
		echo "Copied Device Configuration Service script into ${INST_ETC:?}/init.d"
	else
		echo "##ERROR## Unable to copy Device Configuration Service script into ${INST_ETC:?}/init.d"
		exit 1
	fi

	# Create soft links from rc{2,3,5}.d/S99star_device_config to init.d/star_device_config.
	if update-rc.d ${DEVICE_CONFIG_SCRIPT:?} defaults; then
		echo "Created symlinks to ${DEVICE_CONFIG_SCRIPT:?}"
	else
		for RUN_LEVEL in 2 3 4 5; do
			if [ -e ${RC_ETC:?}/rc${RUN_LEVEL:?}.d ]; then
				if ln -sf "${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}" "${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S99${DEVICE_CONFIG_SCRIPT:?}"; then
					echo "Created symlink ${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S99${DEVICE_CONFIG_SCRIPT:?} to ${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}"
				else
					echo "##ERROR## Unable to create symlink ${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S9$${DEVICE_CONFIG_SCRIPT:?} to ${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}"
					exit 1
				fi
			else
				echo "##ERROR## No ${RC_ETC:?}/rc${RUN_LEVEL:?}.d directory, unable to create symlink ${RC_ETC:?}/rc${RUN_LEVEL:?}.d/S9$${DEVICE_CONFIG_SCRIPT:?} to ${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?}"
				exit 1
			fi
		done
	fi
else
	echo "##ERROR## Unable to find boot script directory ${INST_ETC:?}/init.d"
	exit 1
fi
echo


# Copy the file to set access permissions to the PCI and USB drivers
echo "Setting access permissions for the PCI and USB drivers"
if [ -e ${RULES_DIR:?} ]; then

	if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then

		if cp -f -v ${SCRIPTS_DIR:?}/${PCI_RULES_SCRIPT:?} ${RULES_DIR:?}; then
			echo "PCI driver rules created"
		else
			echo "##ERROR## Couldn't set PCI driver access permissions, couldn't copy to ${RULES_DIR:?}"
			exit 1
		fi
		
	fi

	if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then
	
		if cp -f -v ${SCRIPTS_DIR:?}/${USB_RULES_SCRIPT:?} ${RULES_DIR:?}; then
			echo "USB driver rules created"
		else
			echo "##ERROR## Couldn't set USB driver access permissions, couldn't copy to ${RULES_DIR:?}"
			exit 1
		fi
		
	fi
else
	echo "##ERROR## Couldn't set PCI and USB driver access permissions, ${RULES_DIR:?} doesn't exist"
	exit 1
fi


# Ensure the udev rules are picked up
echo "Updating udev rules..."
udevadm control --reload-rules
udevadm trigger --action=change --subsystem-match=star_spw_pci
udevadm trigger --action=change --subsystem-match=star_spw_usb
echo


# Start the Device Configuration Service
echo "Starting the Device Configuration Service..."
${INST_ETC:?}/init.d/${DEVICE_CONFIG_SCRIPT:?} start
echo

  
# Successfully completed build
echo "Completed STAR-System build"
echo
exit 0

