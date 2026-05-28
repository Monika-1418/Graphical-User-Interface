/**
 * \file clock_rate_params.hpp
 *
 * \brief Specifies a link's clock rate parameters.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Specifies a link's clock rate parameters.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCIe_CONFIG
 */

#ifndef __BASE_CLOCK_RATE_PARAMS_CPP__
#define __BASE_CLOCK_RATE_PARAMS_CPP__

#include "star-api.h"
#include "cfg_api_mk2_types.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace pcie {

/**
* Specifies a link's clock rate parameters.
*/
class ClockRateParams
{
private:
    /* struct required by 'c' API to hold base transmit clock rate parameters */
    STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ClockRateParams() : clockRateParams()
    {
        clockRateParams.divisor = 0;
        clockRateParams.multiplier = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param params A STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct which contains the
    *               clock params to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit ClockRateParams(STAR_CFG_MK2_BASE_TRANSMIT_CLOCK params) :
        clockRateParams()
    {
        clockRateParams.divisor = params.divisor;
        clockRateParams.multiplier = params.multiplier;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param divisor The divisor value. Valid range 1:256 inclusive.
    * @param multiplier The multiplier value. Valid range 2:256 inclusive.
    *
    */
    ClockRateParams(U16 divisor, U16 multiplier) : clockRateParams()
    {
        clockRateParams.divisor = divisor;
        clockRateParams.multiplier = multiplier;
    }

    /**
    * Copy constructor - to allow copies of ClockRateParams objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    ClockRateParams(const ClockRateParams& paramsToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        clockRateParams.divisor = paramsToCopyFrom.clockRateParams.divisor;
        clockRateParams.multiplier =
            paramsToCopyFrom.clockRateParams.multiplier;
    }

    /**
    * Overloaded assignment operator - to allow instances of ClockRateParams
    * objects to have their contents assigned to each other.
    *
    */
    ClockRateParams& operator=(const ClockRateParams& paramsBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &paramsBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        clockRateParams.divisor = paramsBeingAssigned.clockRateParams.divisor;
        clockRateParams.multiplier = paramsBeingAssigned.clockRateParams.multiplier;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~ClockRateParams()
    {
        /* no resources to free at present */
    }

    /**
    * Get the divisor value. Valid range 1:256 inclusive.
    *
    * @return The divisor value.
    *
    */
    U16 GetDivisor()
    {
        return clockRateParams.divisor;
    }

    /**
    * Get the multiplier value. Valid range 2:256 inclusive.
    *
    * @return The multiplier value.
    *
    */
    U16 GetMultiplier()
    {
        return clockRateParams.multiplier;
    }

    /**
    * Set the divisor value. Valid range 1:256 inclusive.
    *
    * @param divisor The divisor value to set.
    *
    */
    void SetDivisor(U16 divisor)
    {
        clockRateParams.divisor = divisor;
    }

    /**
    * Set the multiplier value. Valid range 2:256 inclusive.
    *
    * @param multiplier The multiplier value to set
    *
    */
    void SetMultiplier(U16 multiplier)
    {
        clockRateParams.multiplier = multiplier;
    }

    /**
    * Return the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct which is held
    * internally by this object.
    *
    * @return A pointer to the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK structure which
    *         is held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK()
    {
        /* return pointer to the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct held
         * internally by this object. */
        return &clockRateParams;
    }
};

/* end namespace tags */
} /* pcie */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

