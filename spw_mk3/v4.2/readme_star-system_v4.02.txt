STAR-System v4.02
=================

Contents
--------
 - Introduction
 - Supported Operating Systems
 - Installation Instructions
     - Installation Instructions for Windows
        - Windows XP USB Issue
        - Windows Vista USB Issue
        - Universal CRT
     - Installation Instructions for Linux
     - Installation Instructions for Linux with Secure Boot enabled
     - Installation Instructions for QNX
     - Installation Instructions for VxWorks
     - Installation Instructions for RTEMS
 - Known Issues
 - Change Log
     - Changes between STAR-System v4.01 and v4.02 - 13th March 2020
 - Support and Contact Information



Introduction
------------
STAR-System is the STAR-Dundee software suite system provided with all new and
future STAR-Dundee interface and router devices. It also provides the drivers
used by other STAR-System devices. A full list of supported devices is provided
in the STAR-System documentation. If you are using a device which is not simply
an interface or router, then it will include a separate user manual, focussing
on its unique capabilities. Some of the features of STAR-System are still
available for use with these devices and again, further information is included
in the STAR-System documentation.

The aims of STAR-System include:

 - To provide high bandwidth and low latency packet transmission and reception.
 - To provide additional test and debug capabilities to the user than were
   available in previous STAR-Dundee software packages.
 - To simplify migration between devices, operating systems and programming
   languages.

STAR-System was designed with the intention of not only allowing you to transmit
and receive packets at high data rates, but also to provide you with the tools
you're likely to need when testing or debugging a new device, for example. These
include transmitting packets terminated with an EEP or with no end of packet
marker, transmitting time-codes at a particular point in a packet, receiving
packets and time-codes in the order in which they were carried across the
SpaceWire link, etc.

STAR-System provides a consistent interface across multiple operating systems
(see Supported Operating Systems). It provides a consistent interface for
accessing numerous STAR-Dundee device types, with support for each new device
added as that device is completed. It also supports multiple programming and
scripting languages, with more to be added in the future, in a manner that is as
consistent as is possible across these languages.



Supported Operating Systems
---------------------------
STAR-System is provided for Windows and Linux operating systems. The following
versions of Windows are supported:

 - Windows Vista (32- and 64-bit)
 - Windows Server 2008 (32- and 64-bit)
 - Windows 7 (32- and 64-bit)
 - Windows 8 and 8.1 (32- and 64-bit)
 - Windows Server 2012 (64-bit)
 - Windows 10 (32- and 64-bit)

The Windows versions of STAR-System also support compiling and running
applications under Cygwin.

Note that as Microsoft has dropped support for Windows XP and Windows Server
2003, STAR-System has dropped support for Windows XP and Windows Server 2003
from version 4.02 onwards. Please contact us if you require an older version
with Windows XP support.

2.6, 3.x and 4.0 - 5.3.9 Linux kernels for i386 and x86-64 processors are
supported. An ARM beta release has been developed for Raspberry Pi and
BeagleBone devices and will be included in a future release. For further
information, or support for other ARM devices, please contact us.

QNX, VxWorks and RTEMS versions of STAR-System are available separately.

If you require support for other operating systems or other Linux kernels or
architectures, please contact us using the contact information below.



Installation Instructions
-------------------------

Installation Instructions for Windows
-------------------------------------
To install STAR-System on Windows, double-click the .msi file for the Windows
architecture you are installing on. For example, to install on a 64-bit version
of Windows, double-click star-system_win_x86-64_v4.02.msi. To install on a
32-bit version of Windows, double-click star-system_win_x86-32_v4.02.msi.

After double-clicking the appropriate .msi file, the STAR-System installer
should begin. Follow the instructions to install STAR-System, selecting the
features that you wish to install.

If you encounter issues installing STAR-System on Windows, please make sure that
you have the latest root certificates installed on your machine. These are
available from Windows Update. It is recommended that you have the latest
service pack and updates installed, particularly if using an older version of
Windows (see below).

Windows XP USB Issue
--------------------
There is a bug in Windows XP which can cause PCs to bug check (blue screen) when
transmitting and receiving over USB at the high rates possible using the
STAR-System USB Driver. This will cause your PC to crash and reboot. A fix is
available from Microsoft and it is recommended that this fix is installed (along
with all other important updates) when running on Windows XP.

The fix can be obtained from: http://support.microsoft.com/kb/969238.

Windows Vista USB Issue
-----------------------
There is a bug in Windows Vista prior to Service Pack 2 which can cause USB
traffic to be transferred out of sequence when transmitting and receiving over
USB at the high rates possible using the STAR-System USB Driver. This can cause
SpaceWire packets to be corrupted or received out of order. A fix for this issue
was included in Service Pack 2 and it is highly recommended that the latest
service pack is installed (along with all other important updates) when running
on Windows Vista.

Universal CRT
-------------
The GUI applications require components from the Universal CRT which is built
into Windows 10 and older versions (Vista onwards) that have had updates
applied. If you try to run the GUI applications on an older version of Windows
that has not been updated then you may see an error stating that
"api-ms-win-crt-runtime-l1-1-0.dll" is missing. To resolve this error you must
either install the latest Windows updates or the KB2999226
(https://support.microsoft.com/en-us/help/2999226/update-for-universal-c-runtime-in-windows)
package.


Installation Instructions for Linux
-----------------------------------
Prior to installing STAR-System on Linux, you should ensure that the gcc
compiler, make and the kernel headers are installed on the target machine. These
are required to build the drivers for your system.

On some systems, e.g. RedHat, it may also be necessary to install the
"linux-kernel-devel" and/or "build-essential" packages in order to build the
driver modules. On some versions of CentOS it may be necessary to install
"kernel-devel-$(uname -r)" and/or "kernel-headers-$(uname -r)".

To use the GUI applications you will also need to install Qt. Qt may be
installed by default, or will be available to download using your Linux
distribution's package manager. The packages required are libqtcore4 and
libqtgui4. Qt5 may require qt5-default or qt5-qtbase and
libqt5multimedia5-plugins or qt5-qtmultimedia depending on the distribution. The
STAR-System applications have been successfully tested on Qt versions 4.7, 4.8,
and 5.5 and above.

To install STAR-System on Linux, first extract the .tgz archive, then
double-click the installation wizard for the Linux architecture you are
installing on, or run the installation from the command line. For example, to
install on a 64-bit x86 Linux kernel, run star-system_linux_x86-64_v4.02. To
install on a 32-bit x86 kernel, run star-system_linux_x86-32_v4.02.

After running the appropriate install script, the STAR-System installer GUI
should begin. Follow the instructions to install STAR-System, selecting the
features that you wish to install.

STAR-System requires root access during the installation process to install the
drivers and associated files. Although on some Linux distributions the installer
will ask for the appropriate password, note that on some distributions the
installer will instead fail if the Linux distribution does not ask for the
appropriate permissions. On these distributions you will need to execute the
script as root. This can be done at the command line using su or sudo, depending
on your distribution. For example, on Fedora:

    $ su -
    $ ./star-system_linux_x86-32_v4.02

On Ubuntu, the following command can be used:

    $ sudo ./star-system_linux_x86-32_v4.02

The installer builds drivers specifically for the current kernel. If you upgrade
your kernel, or wish to use STAR-System on another kernel, please run the
following build script to rebuild the drivers:

    $ ./build_star-system.sh

Note that as with the installer you will need to be root or use sudo to run this
script.

We also provide a command line installation script, star-system_install_v4.02.sh
that includes a variety of options to allow feature selection. All features can
be installed with the following command:

    $ ./star-system_install_v4.02.sh --all

The help argument can be used to display the available options:

    $ ./star-system_install_v4.02.sh --help

Note again that as with the installer you will need to be root or use sudo to
run this script.


Installation Instructions for Linux with Secure Boot Enabled
------------------------------------------------------------
Some Linux kernels require that kernel modules, such as the STAR-System drivers,
be cryptographically signed by a trusted key in order to be loaded. This can
often be the case for kernels running on UEFI systems with Secure Boot enabled.

If an error message such as:

    Modprobe: ERROR: could not insert '<driver name>': Required key not available

is encountered during installation, this may indicate that Secure Boot is
enabled.

If it is possible to disable Secure Boot via the UEFI or BIOS settings, this
should allow the drivers to be loaded.

If this is not possible, another method is to use the 'mokutil' tool to disable
Secure Boot. You may have to install this tool using your Linux distribution's
package manager. To disable Secure Boot, use the command:

    sudo mokutil --disable-validation

You will be asked to provide a password, which will be required after rebooting.

On rebooting a prompt will be displayed allowing Secure Boot to be disabled. You
will either be asked for the password you entered earlier, or for specific
characters from the password.

If you cannot disable Secure Boot, or would prefer to retain it, the following
steps describe the procedure required to sign and install the driver modules.

Firstly, create a directory for the signing keys:

    mkdir ~/.ssl
    cd ~/.ssl

Next, create the required keys:

    openssl req -new -x509 -newkey rsa:2048 -keyout star.priv -outform DER \
        -out star.der -nodes -days 36500 -subj "/CN=STAR-Dundee/"

NOTE : it is important to keep your private key secure, as it could be used to
compromise any system which has your public key enrolled.

The next step is to sign the STAR-Dundee USB and PCI drivers:

    sudo /usr/src/kernels/$(uname -r)/scripts/sign-file sha256 star.priv \
        star.der $(modinfo -n star_spw_usb)

    sudo /usr/src/kernels/$(uname -r)/scripts/sign-file sha256 star.priv \
        star.der $(modinfo -n star_spw_pci)

Finally, the key must be "enrolled" to allow it to be recognised by the Secure
Boot system:

    sudo mokutil --import star.der

You will be asked to provide a password, which will be required after rebooting.

Reboot your PC, and select "Enroll MOK" from the displayed menu. The new key
should be shown as a numeric entry, and the details can be checked by selecting
the "View" option. Once the identity of the key has been confirmed, select
"Continue" from the menu, then "Yes" when asked to confirm enrolling the key.

You will then be asked for the password you entered when enrolling the key,
either as the full password, or specific characters from it.

The system will reboot again, and the signed drivers should now be available.

Note that key creation and enrolling only needs to be done once, and the same
keys can be used for future updates or installations of STAR-System.


Installation Instructions for QNX
---------------------------------
To install STAR-System on QNX, first extract the .tgz to the directory you wish
to install the software in, then double-click the install script, or run the
install script from the command line. For example, run:

    $ ./install_star-system.sh

Reboot the system and STAR-System should now be installed and ready to use.


Installation Instructions for VxWorks
-------------------------------------
Installation instructions for VxWorks are available in a separate document,
provided with the VxWorks release.


Installation Instructions for RTEMS
-----------------------------------
The STAR-System RTEMS release is provided as a .tgz file which should be
extracted to a location that can be accessed from your RTEMS build environment.
The example applications can then be built in the build environment using the
following command:

    $ make OPERATING_SYSTEM=RTEMS

Further information on using the RTEMS release is provided in the full
STAR-System documentation, included with the .tgz.


Known Issues
------------
The following list contains all known issues with this release of STAR-System:

 - The STAR_setPacketData() function has been removed from this release of
   STAR-API as setting the data of a packet which had already been submitted to
   be transmitted over a channel would cause an error.
 - As a result of the STAR_setPacketData() issue, STAR Performance Tester's
   random data test does not use a different length for each packet sent, but
   instead picks some random lengths which it uses repeatedly.
 - Resetting the device, e.g. by calling STAR_resetDevice(), while a packet is
   being transmitted and/or received on the device, can cause the device to get
   in to a bad state.

If you encounter any problems not included in this list, please contact us using
the contact information below.


Change Log
----------

A full list of the changes introduced in each version is available in the
STAR-System documentation.

Changes between STAR-System v4.01 and v4.02 - 11th March 2020
-------------------------------------------------------------

 - Fixed bug in STAR-System Test that caused incorrect link speed calculation
   for receive tests.
 - Added support for STAR-Ultra PCIe devices.
 - Linux kernel compatibility tested up to v5.5.8.
 - Various performance improvements.


Support and Contact Information
-------------------------------
For software updates and access to further technical information, please
register your STAR-Dundee devices at
https://www.star-dundee.com/product-registration (or click the Product
Registration link on our website: https://www.star-dundee.com). After
registering your devices, you will be given an account with permissions to
download updates for those devices. You can also register to receive e-mail
notifications when new updates are available.

If you wish to contact STAR-Dundee with a support enquiry, please e-mail
support@star-dundee.com. For general enquiries, please contact
enquiries@star-dundee.com. Our full address is as follows:

    STAR-Dundee Ltd.
    STAR House
    166 Nethergate
    Dundee
    DD1 4EE
    Scotland, UK
