/**
 * \file device_type_resolver.h
 *
 * \brief Defines class which provides methods to determine the type of a given
 *        device (or the type of device which a given Port is part of).
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines class which provides methods to determine the type of a given device
 * (or the type of device which a given Port is part of).
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 */

#include "star_device.hpp"
#include "port.hpp"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::deviceconfig;

#ifndef DEVICE_TYPE_RESOLVER_H
#define DEVICE_TYPE_RESOLVER_H

/**
 * Provides methods to determine the type of a device, e.g. whether it is a
 * Mk2 device or more specifically a BrickMk2, PCIMk2, RouterMk2S, etc.
 */
class DeviceTypeResolver
{
public:
    /**
     * Determines whether the given device is from the Mk2 family of devices.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a Mk2 device, otherwise false if it isn't
     *         a Mk2 device or if an error occurred determining the device type.
     *
     */
    static bool IsMk2Device(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Mk2 device */
        bool isMk2Device = false;
        switch (chipType)
        {
        case 7: /* a PCI Mk2 device, including cPCI Mk2 */
        case 8: /* a PCIe device */
        case 15: /* an SPLT device */
        case 16: /* a Router Mk2S device */
        case 17: /* a Brick Mk2 device */
        case 19: /* a Brick Mk3 device */
        case 21: /* a PXI 4 port interface device */
        case 22: /* a PXI 4 port interface/RMAP device */
        case 24: /* a PXI 12 port Router device */
        case 29: /* a PXI Mk2 4 port interface device */
        case 30: /* a PXI Mk2 4 port interface/RMAP device */
        case 31: /* a PXI Mk2 12 port Router device */
            isMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Mk2 device or not */
        return isMk2Device;
    }

    /**
     * Get the chip type of the specified device.
     *
     * @param pDevice The device to get the chip type for.
     *
     * @return The device chip type (as a numerical identifier) or zero.
     */
    static U8 GetDeviceChipType(STARDevice *pDevice)
    {
        DeviceConfiguration *pDeviceConfig;
        DeviceIdentifierInfo *pDeviceIdentifierInfo;
        U8 chipType;

        /* if no device */
        if (!pDevice)
        {
            /* can't get its chip type */
            return 0;
        }

        /* get the device configuration */
        pDeviceConfig = pDevice->GetDeviceConfiguration();

        /* if failed to obtain device configuration */
        if (!pDeviceConfig)
        {
            /* can't get its chip type */
            return 0;
        }

        /* get the device identification info */
        pDeviceIdentifierInfo = pDeviceConfig->GetDeviceIdentificationInfo();

        /* if failed to get the device identification info */
        if (!pDeviceIdentifierInfo)
        {
            /* can't get its chip type */

            /* free the device configuration */
            delete pDeviceConfig;

            /* return zero */
            return 0;
        }

        /* get the chip type from the device identification info */
        chipType = pDeviceIdentifierInfo->GetChipType();

        /* free the device identification info */
        delete pDeviceIdentifierInfo;

        /* free the device configuration */
        delete pDeviceConfig;

        /* return the chip type */
        return chipType;
    }

    /**
     * Determines whether the given device is a PCI Mk2 or cPCI Mk2 device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a PCI Mk2 device, otherwise false if it
     *         isn't a PCI Mk2 device or if an error occurred determining the
     *         device type.
     */
    static bool IsPCIMk2Device(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a PCI Mk2
         * device */
        bool isPCIMk2Device = false;
        switch (chipType)
        {
        case 7: /* a PCI Mk2 device, including cPCI Mk2 */
            isPCIMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a PCI Mk2 device or not */
        return isPCIMk2Device;
    }

    /**
     * Determines whether the given device is a PCIe or SPLT device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a PCIe or SPLT device, otherwise false if
     *         it isn't a PCIe or SPLT device or if an error occurred
     *         determining the device type.
     */
    static bool IsPCIeOrSPLTDevice(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a PCIe or SPLT
         * device */
        bool isPCIeOrSPLTDevice = false;
        switch (chipType)
        {
        case 8: /* a PCIe device */
        case 15: /* an SPLT device */
            isPCIeOrSPLTDevice = true;
            break;
        default:
            break;
        }

        /* return whether it is a PCIe or SPLT device or not */
        return isPCIeOrSPLTDevice;
    }

    /**
     * Determines whether the given device is a Brick Mk2 device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a Brick Mk2 device, otherwise false if it
     *         isn't a Brick Mk2 device or if an error occurred determining the
     *         device type.
     */
    static bool IsBrickMk2Device(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Brick Mk2
         * device */
        bool isBrickMk2Device = false;
        switch (chipType)
        {
        case 17: /* a Brick Mk2 device */
            isBrickMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Brick Mk2 device or not */
        return isBrickMk2Device;
    }

    /**
     * Determines whether the given device is a Router Mk2S device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a Router Mk2S device, otherwise false if
     *         it isn't a Router Mk2S device or if an error occurred
     *         determining the device type.
     */
    static bool IsRouterMk2SDevice(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Router Mk2S
         * device */
        bool isRouterMk2SDevice = false;
        switch (chipType)
        {
        case 16: /* a Router Mk2S device */
            isRouterMk2SDevice = true;
            break;
        default:
            break;
        }

        /* return whether it is a Router Mk2S device or not */
        return isRouterMk2SDevice;
    }

    /**
     * Determines whether the given device is a Brick Mk3 device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a Brick Mk3 device, otherwise false if it
     *         isn't a Brick Mk3 device or if an error occurred determining the
     *         device type.
     */
    static bool IsBrickMk3Device(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Brick Mk3
         * device */
        bool isBrickMk3Device = false;
        switch (chipType)
        {
        case 19: /* a Brick Mk3 device */
            isBrickMk3Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Brick Mk3 device or not */
        return isBrickMk3Device;
    }

    /**
     * Determines whether the given device is a PXI device.
     *
     * @param pDevice A pointer to a Device object representing the device to
     *                check.
     *
     * @return True if the device is a PXI device, otherwise false if it
     *         isn't a PXI device or if an error occurred determining the
     *         device type.
     */
    static bool IsPXIDevice(STARDevice *pDevice)
    {
        U8 chipType;

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a PXI device */
        bool isPXIDevice = false;
        switch (chipType)
        {
        case 21: /* a PXI 4 port interface device */
        case 22: /* a PXI 4 port interface/RMAP device */
        case 24: /* a PXI 12 port Router device */
        case 29: /* a PXI Mk2 4 port interface device */
        case 30: /* a PXI Mk2 4 port interface/RMAP device */
        case 31: /* a PXI Mk2 12 port Router device */
            isPXIDevice = true;
            break;
        default:
            break;
        }

        /* return whether it is a PXI device or not */
        return isPXIDevice;
    }

    /**
     * Determines whether the given Port is on a device from the Mk2 family of
     * devices.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is from a Mk2 device, otherwise false if it
     *         isn't from a Mk2 device or if an error occurred determining the
     *         device type.
     */
    static bool IsPortOnMk2Device(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Mk2 device */
        bool isMk2Device = false;
        switch (chipType)
        {
        case 7: /* a PCI Mk2 device, including cPCI Mk2 */
        case 8: /* a PCIe device */
        case 15: /* an SPLT device */
        case 16: /* a Router Mk2S device */
        case 17: /* a Brick Mk2 device */
        case 19: /* a Brick Mk3 device */
        case 29: /* a PXI Mk2 4 port interface device */
        case 30: /* a PXI Mk2 4 port interface/RMAP device */
        case 31: /* a PXI Mk2 12 port Router device */
            isMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Mk2 device or not */
        return isMk2Device;
    }

    /**
     * Determines whether the given Port is on a PCI Mk2 device.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is from a PCI Mk2 device, otherwise false if it
     *         isn't from a PCI Mk2 device or if an error occurred determining
     *         the device type.
     */
    static bool IsPortOnPCIMk2Device(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a PCI Mk2
         * device */
        bool isPCIMk2Device = false;
        switch (chipType)
        {
        case 7: /* a PCI Mk2 device, including cPCI Mk2 */
            isPCIMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a PCI Mk2 device or not */
        return isPCIMk2Device;
    }

    /**
     * Determines whether the given Port is on a Router Mk2S device.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is on a Router Mk2S device, otherwise false if
     *         it isn't or if an error occurred determining the device type.
     */
    static bool IsPortOnRouterMk2SDevice(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Router Mk2S
         * device */
        bool isRouterMk2SDevice = false;
        switch (chipType)
        {
        case 16: /* a Router Mk2S device */
            isRouterMk2SDevice = true;
            break;
        default:
            break;
        }

        /* return whether it is a Router Mk2S device or not */
        return isRouterMk2SDevice;
    }

    /**
     * Determines whether the given Port is on a Brick Mk2 device.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is on a Brick Mk2 device, otherwise false if it
     *         isn't or if an error occurred determining the device type.
     */
    static bool IsPortOnBrickMk2Device(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Brick Mk2
         * device */
        bool isBrickMk2Device = false;
        switch (chipType)
        {
        case 17: /* a Brick Mk2 device */
            isBrickMk2Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Brick Mk2 device or not */
        return isBrickMk2Device;
    }

    /**
     * Determines whether the given Port is on a Brick Mk3 device.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is on a Brick Mk3 device, otherwise false if it
     *         isn't or if an error occurred determining the device type.
     */
    static bool IsPortOnBrickMk3Device(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a Brick Mk2
         * device */
        bool isBrickMk3Device = false;
        switch (chipType)
        {
        case 19: /* a Brick Mk3 device */
            isBrickMk3Device = true;
            break;
        default:
            break;
        }

        /* return whether it is a Brick Mk3 device or not */
        return isBrickMk3Device;
    }

    /**
     * Determines whether the given Port is on a PCIe or SPLT device.
     *
     * @param pPort A pointer to a Port object from the device to check.
     *
     * @return True if the Port is on a PCIe or SPLT device, otherwise false if
     *         it isn't or if an error occurred determining the device type.
     */
    static bool IsPortOnPCIeOrSPLTDevice(Port *pPort, STARDevice *pDevice)
    {
        U8 chipType;

        /* if invalid port */
        if (!pPort)
        {
            /* return error */
            return false;
        }

        /* get the device chip type */
        chipType = DeviceTypeResolver::GetDeviceChipType(pDevice);

        /* examine the device's chip type to determine if it is a PCIe or
         * SPLT device */
        bool isPCIeOrSPLTDevice = false;
        switch (chipType)
        {
        case 8: /* a PCIe device */
        case 15: /* an SPLT device */
            isPCIeOrSPLTDevice = true;
            break;
        default:
            break;
        }

        /* return whether it is a PCIe/SPLT device or not */
        return isPCIeOrSPLTDevice;
    }

};

#endif /* DEVICE_TYPE_RESOLVER_H */

