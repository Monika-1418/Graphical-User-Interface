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
INITIAL_INSTALL=1
INSTALL_DIR=0
INSTALL_QT4=0
INSTALL_QT5=0
INSTALL_EXAMPLES=0
INSTALL_INCLUDES=0
INSTALL_DOC=0
INSTALL_APP_NOTES=0
INSTALL_PCI_DRIVER=0
INSTALL_USB_DRIVER=0
INSTALL_ULTRA_PCIE_DRIVER=0
INSTALL_TCP_DRIVER=0
INSTALL_CPP_DOC=0
INSTALL_CPP_INC=0
INSTALL_CPP_EXAMPLES=0
INSTALL_PYTHON_DOC=0
INSTALL_PYTHON_SRC=0
INSTALL_PYTHON_EXAMPLES=0
TRY_LATEST_KERNEL=0
DEVELOPMENT=0
OVERWRITE=0
ACCEPT_LICENSE=0
DISPLAY_HELP=0

# Display help if no arguments specified
if [ -z "$1" ]
  then
    set -- "--help"
fi

# Handle arguments
ARG_COUNT=0
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
          --ultra-pcie-driver ) INSTALL_ULTRA_PCIE_DRIVER=1 ;;
          --ethernet-driver ) INSTALL_TCP_DRIVER=1 ;;
          --no-pci-driver ) INSTALL_PCI_DRIVER=0 ;;
          --no-usb-driver ) INSTALL_USB_DRIVER=0 ;;
          --no-ultra-pcie-driver ) INSTALL_ULTRA_PCIE_DRIVER=0 ;;
          --no-ethernet-driver ) INSTALL_TCP_DRIVER=0 ;;
          --cpp-doc ) INSTALL_CPP_DOC=1 ;;
          --cpp-inc ) INSTALL_CPP_INC=1 ;;
          --cpp-examples ) INSTALL_CPP_EXAMPLES=1 ;;
          --py-doc ) INSTALL_PYTHON_DOC=1 ;;
          --py-src ) INSTALL_PYTHON_SRC=1 ;;
          --py-examples ) INSTALL_PYTHON_EXAMPLES=1 ;;
          --try-latest-kernel ) TRY_LATEST_KERNEL=1 ;;
          --all )
            INSTALL_EXAMPLES=1
            INSTALL_QT4=1
            INSTALL_QT5=1
            INSTALL_INCLUDES=1
            INSTALL_DOC=1
            INSTALL_APP_NOTES=1
            INSTALL_PCI_DRIVER=1
            INSTALL_USB_DRIVER=1
            INSTALL_ULTRA_PCIE_DRIVER=1
            INSTALL_TCP_DRIVER=1
            INSTALL_CPP_DOC=1
            INSTALL_CPP_INC=1
            INSTALL_CPP_EXAMPLES=1
            INSTALL_PYTHON_DOC=1
            INSTALL_PYTHON_SRC=1
            INSTALL_PYTHON_EXAMPLES=1 ;;
          --development )
            INSTALL_EXAMPLES=1
            INSTALL_INCLUDES=1
            INSTALL_DOC=1
            INSTALL_APP_NOTES=1
            INSTALL_CPP_DOC=1
            INSTALL_CPP_INC=1
            INSTALL_CPP_EXAMPLES=1
            INSTALL_PYTHON_DOC=1
            INSTALL_PYTHON_SRC=1
            INSTALL_PYTHON_EXAMPLES=1
            DEVELOPMENT=1 ;;
          --overwrite ) OVERWRITE=1 ;;
          --accept-license ) ACCEPT_LICENSE=1 ;;
          --help ) DISPLAY_HELP=1 ;;
          * )
            echo "Unrecognised option: ${split_arg:?}"
            echo ""
            DISPLAY_HELP=1 ;;
        esac
        
        [ "$arg" = "$split_arg" ] && \
            arg='' || \
            arg="${arg#*,}"
            
        ARG_COUNT=$((ARG_COUNT+1))
    done
done

# Get location of script
script_dir=$(dirname "$0")

# Check for empty install directory
if [ ${INSTALL_DIR:?} = 0 ]; then
    INSTALL_DIR=/usr/local/STAR-Dundee/STAR-System
else
    # Display help if only --install-dir is specified
    if [ ${ARG_COUNT:?} = 1 ]; then
        DISPLAY_HELP=1
    fi
fi

# The archive containing the STAR-System files
tar_file=star-system.tgz

# Display help if requested
if [ ${DISPLAY_HELP:?} = 1 ]; then
    echo "Available options:"
    echo "  --install-dir=[target]  Default /usr/local/STAR-Dundee/STAR-System"
    echo "                          Optional - must specify at least one other option:"
    echo ""
    echo "  --qt4                   Qt4 GUI applications"
    echo "  --qt5                   Qt5 GUI applications"
    echo "  --examples              C API example files"
    echo "  --includes              C API header files"
    echo "  --doc                   C API documentation"
    echo "  --app-notes             Application notes"
    echo "  --pci-driver            The PCI driver"
    echo "  --usb-driver            The USB driver"
    echo "  --ultra-pcie-driver     The Ultra PCIe driver"
    echo "  --ethernet-driver       The Ethernet driver"
    echo "  --cpp-doc               C++ API documentation"
    echo "  --cpp-inc               C++ API header files"
    echo "  --cpp-examples          C++ API example files"
    echo "  --py-doc                Python API documentation"
    echo "  --py-src                Python API source files"
    echo "  --py-examples           Python API examples"
    echo "  --accept-license        Accept the license agreement"
    echo "  --all                   Installs all optional features"
    echo "  --development           Installs files required for development only"
    echo "  --no-pci-driver         Avoids installing PCI driver when --all specified"
    echo "  --no-usb-driver         Avoids installing USB driver when --all specified"
    echo "  --no-ultra-pcie-driver  Avoids installing Ultra PCIe driver when --all specified"
    echo "  --no-ethernet-driver    Avoids installing Ethernet driver when --all specified"
    echo "  --try-latest-kernel     Suppresses the kernel version check when building the drivers"
    
    exit 0
fi

if [ ${ACCEPT_LICENSE:?} = 1 ]; then
    echo "License agreement accepted.\n"
else
    echo
    cat ${script_dir:?}/license.txt
    echo
    echo
    echo -n "Do you accept the above license agreement [y/n]? "
    read answer
    if [ -z ${answer} ]; then
        answer="n"
    fi
    if [ ! ${answer:?} = "y" ] && [ ! ${answer:?} = "Y" ]; then
        echo "You did not accept the license agreement and the installation will not proceed."
        exit 1
    fi
fi

# Check the kernel version
kernel_version_less_than_or_equal()
{
    # sort options:
    #     -V, --version-sort          natural sort of (version) numbers within text
    #     -C, --check=quiet, --check=silent  like -c, but do not report first bad line
    printf '%s\n' "$1" "$2" | sort -C -V

    # the exit status of "sort" in the line above becomes exit status of this function
}

if [ ! ${TRY_LATEST_KERNEL:?} = 1 ]; then
    # "Peek" the value of LATEST_TESTED_KERNEL from star-system.tgz/build_star-system.sh, using "tar -O", grep and sed.
    LATEST_TESTED_KERNEL=`tar -xOzf ${script_dir:?}/${tar_file:?} common_operations.sh | grep ^LATEST_TESTED_KERNEL | sed -e 's/.*=//g'`

    KVERSION=`uname -r`                     # Current kernel version
    CURRENT_KERNEL=$(echo $KVERSION | sed -e 's/-.*//')
    if ! kernel_version_less_than_or_equal $CURRENT_KERNEL $LATEST_TESTED_KERNEL
    then
        echo "##ERROR## Kernel ($KVERSION) is too recent"
        echo
        echo "This kernel ($KVERSION) is too recent, and not supported by the packaged drivers (up to $LATEST_TESTED_KERNEL)."
        echo "Please contact STAR-Dundee to check if a newer version of the driver is available."
        echo "You can also run with --try-latest-kernel to suppress this check."
        exit 1
    fi
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

# Populate core files to be extracted depending on selected features
CORE_FILES="build_star-system.sh build_kernel_modules.sh common_operations.sh remove_star-system.sh lib Licenses"
if [ ${DEVELOPMENT:?} = 0 ]; then
    CORE_FILES="${CORE_FILES:?} scripts/star_device_config scripts/star-system.service"
fi

# Define a helper function to unpack a given path from the tarball
unpack_from_tarball() {
    description=${1:?}
    shift
    # Note: remaining arguments are required to be non-empty (dereferenced using "${@:?}" below)
    # Note: the arguments may take several forms, as permitted by the "tar" command, for example:
    #       - dir
    #       - dir/file
    #       - dir1 dir2 dir3/file
    #       - ... --exclude-file=specific.so

        if tar xvfz ${script_dir:?}/${tar_file:?} -C ${INSTALL_DIR:?} ${@:?}
        then
                echo "${description:?} extracted"
        else
                echo "##ERROR## Failed to unpack ${description:?}"
                exit 1
        fi
}

# Extract core files
# Check if INSTALL_TCP_DRIVER is set to 0
if [ ${INSTALL_TCP_DRIVER:?} = 0 ]; then
    unpack_from_tarball "Core files" ${CORE_FILES:?} --exclude='libstar_tcp_driver.so'
else 
    unpack_from_tarball "Core files" ${CORE_FILES:?}
fi

# Extract optional files
if [ ${INSTALL_EXAMPLES:?} = 1 ]; then
    unpack_from_tarball "C API examples" "examples"
fi
if [ ${INSTALL_INCLUDES:?} = 1 ]; then
    unpack_from_tarball "C API includes" "inc"
fi
if [ ${INSTALL_DOC:?} = 1 ]; then
    unpack_from_tarball "C API documentation" "doc"
fi
if [ ${INSTALL_APP_NOTES:?} = 1 ]; then
    unpack_from_tarball "Application notes" "application_notes"
fi

if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then
    unpack_from_tarball "PCI driver" "driver_src/pci_driver"
    unpack_from_tarball "PCI driver rules" "scripts/99-star_spw_pci.rules"
fi
if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then
    unpack_from_tarball "USB driver" "driver_src/usb_driver"
    unpack_from_tarball "USB driver rules" "scripts/99-star_spw_usb.rules"
fi
if [ ${INSTALL_ULTRA_PCIE_DRIVER:?} = 1 ]; then
    unpack_from_tarball "Ultra PCIe driver" "driver_src/ultra_pcie_driver"
    unpack_from_tarball "Ultra PCIe driver rules" "scripts/99-star_ultra_pcie.rules"
fi

if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
    unpack_from_tarball "C++ API documentation" "apis/cpp_api/doc"
fi
if [ ${INSTALL_CPP_INC:?} = 1 ]; then
    unpack_from_tarball "C++ API includes" "apis/cpp_api/inc"
fi
if [ ${INSTALL_CPP_EXAMPLES:?} = 1 ]; then
    unpack_from_tarball "C++ API examples" "apis/cpp_api/examples"
fi

if [ ${INSTALL_PYTHON_DOC:?} = 1 ]; then
    unpack_from_tarball "Python API documentation" "apis/python_api/Documentation"
fi
if [ ${INSTALL_PYTHON_SRC:?} = 1 ]; then
    unpack_from_tarball "Python API source files" "apis/python_api/STAR_system"
fi
if [ ${INSTALL_PYTHON_EXAMPLES:?} = 1 ]; then
    unpack_from_tarball "Python API examples" "apis/python_api/Examples"
fi

# Extract shortcut files
if [ ${DEVELOPMENT:?} = 0 ]; then
    unpack_from_tarball "Desktop shortcuts" "shortcuts"
fi

# Extract GUI examples if GUIs have been selected in install
if [ ${INSTALL_QT4:?} = 1 ] || [ ${INSTALL_QT5:?} = 1 ]; then
    unpack_from_tarball "GUI Configuration Examples" "gui_config_examples"
fi

# Helper function to determine presence of Qt libraries
checkForQtLibraries()
{
    # Note: in the event there is a "permssion denied" or similar during the find,
    #       then the exit code will be 1, not 0
    # Therefore, we use a simple test on the stdout to determine the return value
    RESULT=`find /usr/lib /usr/lib32 /usr/lib64 -name "libQt*.so.$1" 2>/dev/null`
    if [ -z "$RESULT" ]; then
        echo 0
    else
        echo 1
    fi
}

# If command line specifies BOTH --qt4 and --qt5, or --all, then
# perform a check for the version(s) of libraries available, and install as approriate
if [ ${INSTALL_QT4:?} = 1 ] && [ ${INSTALL_QT5:?} = 1 ]; then
    if [ "$(checkForQtLibraries 4)" = "1" ] && [ "$(checkForQtLibraries 5)" = "0" ]; then
        # specifically require Qt4 applications
        echo "Detected Qt4, falling back to Qt4 GUI applications"
        INSTALL_QT4=1
        INSTALL_QT5=0
    else
        # default to installing Qt5 applications
        INSTALL_QT4=0
        INSTALL_QT5=1
    fi
fi


# Construct build arguments
build_args=
if [ ${INSTALL_EXAMPLES:?} = 1 ]; then
    build_args="${build_args} --examples"
fi
if [ ${INSTALL_QT4:?} = 1 ]; then
    build_args="${build_args} --qt4"
fi
if [ ${INSTALL_QT5:?} = 1 ]; then
    build_args="${build_args} --qt5"
fi
if [ ${INSTALL_DOC:?} = 1 ]; then
    build_args="${build_args} --doc"
fi
if [ ${INSTALL_TCP_DRIVER:?} = 0 ]; then
    build_args="${build_args} --no-ethernet-driver"
fi
if [ ${INSTALL_CPP_DOC:?} = 1 ]; then
    build_args="${build_args} --cpp-doc"
fi
if [ ${INSTALL_PYTHON_DOC:?} = 1 ]; then
    build_args="${build_args} --py-doc"
fi
if [ ${INSTALL_APP_NOTES:?} = 1 ]; then
    build_args="${build_args} --app-notes"
fi
if [ ${TRY_LATEST_KERNEL:?} = 1 ]; then
    build_args="${build_args} --try-latest-kernel"
fi

if [ ${DEVELOPMENT:?} = 1 ]; then
    build_args="${build_args} --development --no-pci-driver --no-usb-driver --no-ultra-pcie-driver"

    if [ ${INSTALL_PCI_DRIVER:?} = 1 ]; then
        echo "##WARNING## Suppressing PCI Driver (\"--development\" has been specified)"
    fi

    if [ ${INSTALL_USB_DRIVER:?} = 1 ]; then
        echo "##WARNING## Suppressing USB Driver (\"--development\" has been specified)"
    fi

    if [ ${INSTALL_ULTRA_PCIE_DRIVER:?} = 1 ]; then
        echo "##WARNING## Suppressing Ultra PCIe Driver (\"--development\" has been specified)"
    fi
else
    if [ ${INSTALL_PCI_DRIVER:?} = 0 ]; then
        build_args="${build_args} --no-pci-driver"
    fi
    if [ ${INSTALL_USB_DRIVER:?} = 0 ]; then
        build_args="${build_args} --no-usb-driver"
    fi
    if [ ${INSTALL_ULTRA_PCIE_DRIVER:?} = 0 ]; then
        build_args="${build_args} --no-ultra-pcie-driver"
    fi
fi

# Build STAR-System
cd ${INSTALL_DIR:?}
./build_star-system.sh ${build_args}

# Check build status
retval=$?
if [ ${retval:?} -eq 0 ]; then
    echo "Installation completed to ${INSTALL_DIR:?}"
else
    echo "Errors during installation to ${INSTALL_DIR:?}"
    exit ${retval:?}
fi
