/**
 * \file external_port_errors.hpp
 *
 * \brief Holds information about errors which may be present on one of the
 *        external ports of a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about errors which may be present on one of the external
 * ports of a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_EXTERNAL_PORT_ERRORS_CPP__
#define __DEVICE_EXTERNAL_PORT_ERRORS_CPP__

#include "port_errors.hpp"
#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds information about errors which may be present on one of the external
* ports on a device. If a member of this object is set, then the error it
* represents is present.
*/
class ExternalPortErrors : public PortErrors
{
private:
     /** struct required by STAR-API to hold external port errors */
     STAR_CFG_EXTERNAL_PORT_ERRORS externalPortErrors;

     /**
     * Copy the contents of the given STAR_CFG_EXTERNAL_PORT_ERRORS struct into
     * the STAR_CFG_EXTERNAL_PORT_ERRORS held by this object.
     *
     * @param errorInfo A reference to a STAR_CFG_EXTERNAL_PORT_ERRORS struct to
     *                  copy from.
     */
     void CopyFromSTAR_CFG_EXTERNAL_PORT_ERRORS(
         STAR_CFG_EXTERNAL_PORT_ERRORS& errorInfo)
     {
         externalPortErrors.errorCount = errorInfo.errorCount;
         externalPortErrors.packetAddress = errorInfo.packetAddress;
         externalPortErrors.portTimeout = errorInfo.portTimeout;
     }

     /**
     * Copy the values of the member variables of the given ExternalPortErrors
     * object into this object.
     *
     * @param errorInfo A reference to an ExternalPortErrors object to copy
     *                  from.
     *
     */
     void CopyFromExternalPortErrors(const ExternalPortErrors& errorInfo)
     {
         externalPortErrors.errorCount =
             errorInfo.externalPortErrors.errorCount;
         externalPortErrors.packetAddress =
             errorInfo.externalPortErrors.packetAddress;
         externalPortErrors.portTimeout =
             errorInfo.externalPortErrors.portTimeout;
     }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ExternalPortErrors() : PortErrors()
    {
         externalPortErrors.errorCount = 0;
         externalPortErrors.packetAddress = 0;
         externalPortErrors.portTimeout = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param errorInfo A STAR_CFG_EXTERNAL_PORT_ERRORS struct which contains
    *              the external port error values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit ExternalPortErrors(STAR_CFG_EXTERNAL_PORT_ERRORS& errorInfo) :
        PortErrors()
    {
        /* copy the contents of the given struct into the struct held by
         * this object */
        CopyFromSTAR_CFG_EXTERNAL_PORT_ERRORS(errorInfo);
    }

    /**
    * Copy constructor - to allow copies of ExternalPortErrors objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    ExternalPortErrors(const ExternalPortErrors& externalErrorsToCopyFrom)
                                                            : PortErrors()
    {
        /* copy contents of object being copied into this object */
        CopyFromExternalPortErrors(externalErrorsToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of ExternalPortErrors
    * objects to have their contents assigned to each other.
    *
    */
    ExternalPortErrors& operator=(const ExternalPortErrors& externalErrorsBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &externalErrorsBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        CopyFromExternalPortErrors(externalErrorsBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~ExternalPortErrors()
    {
        /* no resources to free at present */
    }

    /**
    * Get the number of errors present on the external port.
    *
    * @return The number of errors.
    *
    */
    char GetErrorCount()
    {
        return externalPortErrors.errorCount;
    }

    /**
    * Get whether an error occurred because a packet was received with an
    * invalid address either due to an unknown port or an invalid logical
    * address. This error also occurs when an empty packet is sent to the
    * external port.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetPacketAddress()
    {
        return externalPortErrors.packetAddress;
    }

    /**
    * Get whether an error was recorded to indicate that a port has become
    * blocked for a period of time and a packet could not be routed to a destination
    * port before the port timeout occurred.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetPortTimeout()
    {
        return externalPortErrors.portTimeout;
    }

};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

