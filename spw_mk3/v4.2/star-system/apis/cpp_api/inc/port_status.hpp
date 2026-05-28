/**
 * \file port_status.hpp
 *
 * \brief  Parent class for objects which hold information about the status of
 *         a port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for objects which hold information about the status of a port
 * on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_PORT_STATUS_CPP__
#define __DEVICE_PORT_STATUS_CPP__

#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
 * Parent class for objects which hold information about the status of a port
 * on a device.
*/
class PortStatus
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PortStatus()
    {
    }

    /**
    * Copy constructor - to allow copies of PortStatus objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    PortStatus(const PortStatus& portStatusToCopyFrom)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portStatusToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of PortStatus
    * objects to have their contents assigned to each other.
    *
    */
    PortStatus& operator=(const PortStatus& portStatusBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &portStatusBeingAssigned)
            return *this;

        /* nothing to copy */

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~PortStatus()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

