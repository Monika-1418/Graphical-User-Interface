/**
 * \file driver_factory.hpp
 *
 * \brief Base factory object, for creating instances of Driver objects.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Base factory object, to create instances of Driver objects.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DRIVER_FACTORY_BASE_CPP__
#define __STAR_DRIVER_FACTORY_BASE_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/* forward declaration of Driver class */
class Driver;

/**
* Base class for Driver factory objects.
*/
class DriverFactory
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    DriverFactory()
    {
    }

    /**
    * Destructor. Made virtual to support correct destruction of derived
    * classes.
    */
    virtual ~DriverFactory()
    {
    }

    /**
    * Create and return an instance of a Driver object corresponding to the
    * given STAR_DRIVER_ID.
    *
    * @param[in] driverID The ID of driver to create.
    *
    * @return A pointer to a new Driver object, or NULL.
    *
    * \note This is defined as a pure virtual function. It must be overridden
    *       by child classes.
    *
    */
    virtual _Check_return_ Driver* CreateDriver(STAR_DRIVER_ID driverID) = 0;
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

