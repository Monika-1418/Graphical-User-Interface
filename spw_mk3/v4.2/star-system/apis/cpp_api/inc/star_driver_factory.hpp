/**
 * \file star_driver_factory.hpp
 *
 * \brief Factory, for creating instances of Driver objects.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Factory, to create instances of Driver objects.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DRIVER_FACTORY_CPP__
#define __STAR_DRIVER_FACTORY_CPP__

#include "star-dundee_annotations.h"
#include "driver_factory.hpp"
#include "driver.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Factory, for creating instances of Driver objects.
*/
class STARDriverFactory : public DriverFactory
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    STARDriverFactory()
    {
    }

    /**
    * Destructor. Made virtual to support correct destruction of derived
    * classes.
    */
    virtual ~STARDriverFactory()
    {
    }

    /**
    * Create and return an instance of a Driver object corresponding to
    * the given driver ID.
    *
    * @param[in] driverID The ID of the driver to create an instance of a
    *                     Driver object for.
    *
    * @return A pointer to a new Driver object, or NULL.
    *
    */
    _Check_return_ Driver* CreateDriver(STAR_DRIVER_ID driverID)
    {
        /* if invalid ID, e.g. zero */
        if (driverID == 0)
        {
            return NULL;
        }

        /* create null Driver */
        Driver *newStarDriver = NULL;

        /* try to create instance of new Driver based on the ID provided */
        try
        {
            newStarDriver = new Driver(driverID);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            newStarDriver = NULL;

        }

        /* return the new Driver, may be NULL */
        return newStarDriver;
    }

    /**
    * Create and return an instance of a Driver object corresponding to
    * the given driver ID.
    *
    * @param[in] driverID The ID of the driver to create an instance of a
    *                     Driver object for.
    *
    * @return A pointer to a new Driver object, or NULL.
    *
    */
    static _Check_return_ Driver* CreateDriverFromID(STAR_DRIVER_ID driverID)
    {
        /* if invalid ID, e.g. zero */
        if (driverID == 0)
        {
            return NULL;
        }

        /* create null Driver */
        Driver *newStarDriver = NULL;

        /* try to create instance of new Driver based on the ID provided */
        try
        {
            newStarDriver = new Driver(driverID);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            newStarDriver = NULL;

        }

        /* return the new Driver, may be NULL */
        return newStarDriver;
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

