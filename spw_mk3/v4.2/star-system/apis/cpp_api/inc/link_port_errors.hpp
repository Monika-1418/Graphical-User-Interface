/**
 * \file link_port_errors.hpp
 *
 * \brief Holds information about errors which may be present on one of the link
 *        ports on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about errors which may be present on one of the link ports
 * on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_LINK_PORT_ERRORS_CPP__
#define __DEVICE_LINK_PORT_ERRORS_CPP__

#include "port_errors.hpp"
#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds information about errors which may be present on one of the link ports
* on a device. If a member of this object is set, then the error it
* represents is present.
*/
class LinkPortErrors : public PortErrors
{
private:
     /** struct required by STAR-API to hold link port errors */
     STAR_CFG_SPW_LINK_ERRORS linkPortErrors;

     /**
     * Copy the contents of the given STAR_CFG_SPW_LINK_ERRORS struct into
     * the STAR_CFG_SPW_LINK_ERRORS held by this object.
     *
     * @param errorInfo A reference to a STAR_CFG_SPW_LINK_ERRORS struct to
     *                  copy from.
     */
     void CopyFromSTAR_CFG_SPW_LINK_ERRORS(STAR_CFG_SPW_LINK_ERRORS& errorInfo)
     {
         linkPortErrors.characterSequence = errorInfo.characterSequence;
         linkPortErrors.credit = errorInfo.credit;
         linkPortErrors.disconnect = errorInfo.disconnect;
         linkPortErrors.errorCount = errorInfo.errorCount;
         linkPortErrors.escape = errorInfo.escape;
         linkPortErrors.packetAddress = errorInfo.packetAddress;
         linkPortErrors.parity = errorInfo.parity;
         linkPortErrors.portTimeout = errorInfo.portTimeout;
     }

     /**
     * Copy the values of the member variables of the given LinkPortErrors
     * object into this object.
     *
     * @param errorInfo A reference to a LinkPortErrors object to copy from.
     *
     */
     void CopyFromLinkPortErrors(const LinkPortErrors& errorInfo)
     {
         linkPortErrors.characterSequence =
             errorInfo.linkPortErrors.characterSequence;
         linkPortErrors.credit = errorInfo.linkPortErrors.credit;
         linkPortErrors.disconnect = errorInfo.linkPortErrors.disconnect;
         linkPortErrors.errorCount = errorInfo.linkPortErrors.errorCount;
         linkPortErrors.escape = errorInfo.linkPortErrors.escape;
         linkPortErrors.packetAddress = errorInfo.linkPortErrors.packetAddress;
         linkPortErrors.parity = errorInfo.linkPortErrors.parity;
         linkPortErrors.portTimeout = errorInfo.linkPortErrors.portTimeout;
     }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    LinkPortErrors() : PortErrors()
    {
         linkPortErrors.characterSequence = 0;
         linkPortErrors.credit = 0;
         linkPortErrors.disconnect = 0;
         linkPortErrors.errorCount = 0;
         linkPortErrors.escape = 0;
         linkPortErrors.packetAddress = 0;
         linkPortErrors.parity = 0;
         linkPortErrors.portTimeout = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param errorInfo A STAR_CFG_SPW_LINK_ERRORS struct which contains
    *              the link port error values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit LinkPortErrors(STAR_CFG_SPW_LINK_ERRORS& errorInfo) :
        PortErrors()
    {
        /* copy the contents of the given struct into the struct held by
         * this object */
        CopyFromSTAR_CFG_SPW_LINK_ERRORS(errorInfo);
    }

    /**
    * Copy constructor - to allow copies of LinkPortErrors objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    LinkPortErrors(const LinkPortErrors& linkErrorsToCopyFrom) : PortErrors()
    {
        /* copy contents of object being copied into this object */
        CopyFromLinkPortErrors(linkErrorsToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of LinkPortErrors
    * objects to have their contents assigned to each other.
    *
    */
    LinkPortErrors& operator=(const LinkPortErrors& linkErrorsBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &linkErrorsBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        CopyFromLinkPortErrors(linkErrorsBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~LinkPortErrors()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether a character sequence error occurred on the link - a time code
    * or data character was received before the first FCT.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetCharacterSequence()
    {
        return linkPortErrors.characterSequence;
    }

    /**
    * Get whether a credit error occurred on the link.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetCredit()
    {
        return linkPortErrors.credit;
    }

    /**
    * Get whether a disconnect error occurred on the link - no activity occurred
    * on the link for the disconnect timeout period.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetDisconnect()
    {
        return linkPortErrors.disconnect;
    }

    /**
    * Get the number of errors present on the link port.
    *
    * @return The number of errors.
    *
    */
    char GetErrorCount()
    {
        return linkPortErrors.errorCount;
    }

    /**
    * Get whether an invalid escape code was received on the link (ESC-ESC,
    * ESC-EOP, or ESC-EEP).
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetEscape()
    {
        return linkPortErrors.escape;
    }

    /**
    * Get whether a packet with an invalid address was received on the link,
    * either due to an unknown port or an invalid logical address.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetPacketAddress()
    {
        return linkPortErrors.packetAddress;
    }

    /**
    * Get whether a parity was detected on a character parity bit.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetParity()
    {
        return linkPortErrors.parity;
    }

    /**
    * Get whether a port timeout has occurred, e.g. the port has become blocked
    * for a period of time and a packet could not be routed to a destination
    * port before the port timeout occurred.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetPortTimeout()
    {
        return linkPortErrors.portTimeout;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

