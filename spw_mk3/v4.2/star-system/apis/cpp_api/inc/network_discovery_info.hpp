/**
 * \file network_discovery_info.hpp
 *
 * \brief Holds network discovery information for a device, e.g. the number of
 *        ports it has, which ports are running.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds network discovery information for a device, e.g. the number of ports
 * it has, which ports are running..
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __NETWORK_DISCOVERY_INFO_CPP__
#define __NETWORK_DISCOVERY_INFO_CPP__

#include "cfg_api_router_types.h"
#include "cfg_api_router.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds network discovery information for a device, e.g. the number of ports it
* has, which ports are running.
*/
class NetworkDiscoveryInfo
{
public:
    /** device types permitted within the network discovery register */
    enum DeviceType
    {
        /** router device */
        DEVICE_TYPE_ROUTER = STAR_CFG_DEVICE_TYPE_ROUTER,
        /** unknown device type */
        DEVICE_TYPE_UNKNOWN = STAR_CFG_DEVICE_TYPE_UNKNOWN,
        /** invalid device type */
        DEVICE_TYPE_INVALID = STAR_CFG_DEVICE_TYPE_INVALID
    };

private:
    /** struct required by 'c' API to hold network discovery info */
    STAR_CFG_NETWORK_DISCOVERY_INFO networkInfo;

    /** id of device which this network discovery information relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    NetworkDiscoveryInfo() : idOfOwningDevice(0)
    {
        networkInfo.portCount = 0;
        networkInfo.returnPort = 0;
        networkInfo.runningPortsCount = 0;
        networkInfo.runningPortsMask = 0;
        networkInfo.deviceType = STAR_CFG_DEVICE_TYPE_UNKNOWN;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param id The id of the device which the network discovery information
    *           has been retreived for.
    * @param nInfo A STAR_CFG_NETWORK_DISCOVERY_INFO struct which contains the
    *              network discovery values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    NetworkDiscoveryInfo(STAR_DEVICE_ID id,
                         STAR_CFG_NETWORK_DISCOVERY_INFO nInfo)
                         : idOfOwningDevice(id)
    {
        networkInfo.portCount = nInfo.portCount;
        networkInfo.returnPort = nInfo.returnPort;
        networkInfo.runningPortsCount = nInfo.runningPortsCount;
        networkInfo.runningPortsMask = nInfo.runningPortsMask;
        networkInfo.deviceType = nInfo.deviceType;
    }

    /**
    * Copy constructor - to allow copies of NetworkDiscoveryInfo objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    NetworkDiscoveryInfo(const NetworkDiscoveryInfo& networkInfoToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        networkInfo.portCount = networkInfoToCopyFrom.networkInfo.portCount;
        networkInfo.returnPort = networkInfoToCopyFrom.networkInfo.returnPort;
        networkInfo.runningPortsCount =
            networkInfoToCopyFrom.networkInfo.runningPortsCount;
        networkInfo.runningPortsMask =
            networkInfoToCopyFrom.networkInfo.runningPortsMask;
        networkInfo.deviceType = networkInfoToCopyFrom.networkInfo.deviceType;
        idOfOwningDevice = networkInfoToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * NetworkDiscoveryInfo objects to have their contents assigned to each
    * other.
    *
    */
    NetworkDiscoveryInfo& operator=(
        const NetworkDiscoveryInfo& networkInfoBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &networkInfoBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        networkInfo.portCount = networkInfoBeingAssigned.networkInfo.portCount;
        networkInfo.returnPort =
            networkInfoBeingAssigned.networkInfo.returnPort;
        networkInfo.runningPortsCount =
            networkInfoBeingAssigned.networkInfo.runningPortsCount;
        networkInfo.runningPortsMask =
            networkInfoBeingAssigned.networkInfo.runningPortsMask;
        networkInfo.deviceType =
            networkInfoBeingAssigned.networkInfo.deviceType;
        idOfOwningDevice = networkInfoBeingAssigned.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~NetworkDiscoveryInfo()
    {
        /* no resources to free at present */
    }

    /**
    * Get the device type.
    *
    * @return The device type.
    *
    */
    DeviceType GetDeviceType()
    {
        return (DeviceType)networkInfo.deviceType;
    }

    /**
    * Get the number of ports the device has.
    *
    * @return The number of ports.
    *
    */
    U8 GetPortCount()
    {
        return networkInfo.portCount;
    }

    /**
    * Get the input port number which was used to access the network discovery
    * register.
    *
    * @return The number of the return port.
    *
    * \note The port number returned can be provided to the \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration::GetPort() DeviceConfiguration::GetPort() \endlink
    *       method on the DeviceConfiguration of the Device which owns the port
    *       in order to obtain an instance of a Port object (to access the
    *       Port's properties).
    *
    */
    U8 GetReturnPort()
    {
        return networkInfo.returnPort;
    }

    /**
    * Get the count of running ports on the device.
    *
    * @return The running ports count.
    *
    */
    U8 GetRunningPortsCount()
    {
        return networkInfo.runningPortsCount;
    }

    /**
    * Get which ports on the device are currently running.
    *
    * @return A bitmask of ports which are in the run state, where Bit 1
    *         corresponds to port 1.
    *
    */
    U32 GetRunningPortsMask()
    {
        return networkInfo.runningPortsMask;
    }

    /**
    * Refresh the network discovery information held by this object to reflect
    * any changes in state on the device it relates to, e.g. if there have been
    * changes in the number of running ports due to links being started or
    * stopped since the network discovery information was previously requested.
    * The network discovery properties held by this object will be updated to
    * the current known state.
    *
    * @return 1 if the network discovery information was successfully refreshed,
    *         otherwise 0 if there is no valid device ID available to retreive
    *         the current network discovery information from or if the request
    *         to retreive the network discovery information for the device
    *         failed.
    *
    */
    int Refresh()
    {
        /* if valid device id held */
        if (idOfOwningDevice)
        {
            /* re-request network discovery information for the device */
            return CFG_ROUTER_getNetworkDiscoveryInfo(idOfOwningDevice,
                                                      &networkInfo);

        }
        else
        {
            /* don't have a device to request network discovery information
             * for */
            return 0;
        }
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

