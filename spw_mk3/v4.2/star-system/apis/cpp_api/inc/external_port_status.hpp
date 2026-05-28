/**
 * \file external_port_status.hpp
 *
 * \brief Holds information about the status of an external port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about the status of an external port on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_EXTERNAL_PORT_STATUS_CPP__
#define __DEVICE_EXTERNAL_PORT_STATUS_CPP__

#include "port_status.hpp"
#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds information about the status of an external port on a device.
*/
class ExternalPortStatus : public PortStatus
{
private:
     /** struct required by STAR-API to hold the external port's status */
     STAR_CFG_EXTERNAL_PORT_STATUS externalPortStatus;

     /**
     * Copy the contents of the given STAR_CFG_EXTERNAL_PORT_STATUS struct into
     * the STAR_CFG_EXTERNAL_PORT_STATUS held by this object.
     *
     * @param portStatus A reference to a STAR_CFG_EXTERNAL_PORT_STATUS struct
     *                   to copy from.
     */
     void CopyFromSTAR_CFG_EXTERNAL_PORT_STATUS(
         STAR_CFG_EXTERNAL_PORT_STATUS& portStatus)
     {
         externalPortStatus.inputBufferEmpty = portStatus.inputBufferEmpty;
         externalPortStatus.inputBufferFull = portStatus.inputBufferFull;
         externalPortStatus.outputBufferEmpty = portStatus.outputBufferEmpty;
         externalPortStatus.outputBufferFull = portStatus.outputBufferFull;
     }

     /**
     * Copy the values of the member variables of the given ExternalPortStatus
     * object into this object.
     *
     * @param portStatus A reference to an ExternalPortStatus object to copy
     *                   from.
     *
     */
     void CopyFromExternalPortStatus(const ExternalPortStatus& portStatus)
     {
         externalPortStatus.inputBufferEmpty =
             portStatus.externalPortStatus.inputBufferEmpty;
         externalPortStatus.inputBufferFull =
             portStatus.externalPortStatus.inputBufferFull;
         externalPortStatus.outputBufferEmpty =
             portStatus.externalPortStatus.outputBufferEmpty;
         externalPortStatus.outputBufferFull =
             portStatus.externalPortStatus.outputBufferFull;
     }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ExternalPortStatus() : PortStatus()
    {
         externalPortStatus.inputBufferEmpty = 0;
         externalPortStatus.inputBufferFull = 0;
         externalPortStatus.outputBufferEmpty = 0;
         externalPortStatus.outputBufferFull = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param portStatus A STAR_CFG_EXTERNAL_PORT_STATUS struct which contains
    *                   the external port status values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit ExternalPortStatus(STAR_CFG_EXTERNAL_PORT_STATUS& portStatus) :
        PortStatus()
    {
        /* copy the contents of the given struct into the struct held by
         * this object */
        CopyFromSTAR_CFG_EXTERNAL_PORT_STATUS(portStatus);
    }

    /**
    * Copy constructor - to allow copies of ExternalPortStatus objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    ExternalPortStatus(const ExternalPortStatus& portStatusToCopyFrom)
                      : PortStatus()
    {
        /* copy contents of object being copied into this object */
        CopyFromExternalPortStatus(portStatusToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of ExternalPortStatus
    * objects to have their contents assigned to each other.
    *
    */
    ExternalPortStatus& operator=(const ExternalPortStatus& portStatusBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &portStatusBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        CopyFromExternalPortStatus(portStatusBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~ExternalPortStatus()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether the input buffer of the external port is empty.
    *
    * \note The input buffer writes data to the SpaceWire router.
    *
    * @return 1 if empty, otherwise 0.
    *
    */
    char GetInputBufferEmpty()
    {
        return externalPortStatus.inputBufferEmpty;
    }

    /**
    * Get whether the input buffer of the external port is full.
    *
    * @return 1 if full, otherwise 0.
    *
    */
    char GetInputBufferFull()
    {
        return externalPortStatus.inputBufferFull;
    }

    /**
    * Get whether the output buffer of the external port is empty.
    *
    * \note The output buffer writes data to the external device connected to
    *       the external port.
    *
    * @return 1 if empty, otherwise 0.
    *
    */
    char GetOutputBufferEmpty()
    {
        return externalPortStatus.outputBufferEmpty;
    }

    /**
    * Get whether the output buffer of the external port is full.
    *
    * @return 1 if full, otherwise 0.
    *
    */
    char GetOutputBufferFull()
    {
        return externalPortStatus.outputBufferFull;
    }

};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

