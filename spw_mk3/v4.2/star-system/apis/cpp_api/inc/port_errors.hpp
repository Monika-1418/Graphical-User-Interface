/**
 * \file port_errors.hpp
 *
 * \brief Parent class for objects which hold information about errors that may
 *        be present on the different types of port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for objects which hold information about errors that may be
 * present on the different types of port on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __PORT_ERRORS_CPP__
#define __PORT_ERRORS_CPP__

#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Parent class for objects which hold information about errors that may be
* present on the different types of port on a device.
*/
class PortErrors
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PortErrors()
    {
    }

    /**
    * Copy constructor - to allow copies of PortErrors objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    PortErrors(const PortErrors& errorsToCopyFrom)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(errorsToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of PortErrors
    * objects to have their contents assigned to each other.
    *
    */
    PortErrors& operator=(const PortErrors& errorsBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &errorsBeingAssigned)
            return *this;

        /* nothing to copy / assign */

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~PortErrors()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

