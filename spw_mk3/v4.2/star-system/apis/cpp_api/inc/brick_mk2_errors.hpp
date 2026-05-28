/**
 * \file brick_mk2_errors.hpp
 *
 * \brief Holds information about errors to inject into a Brick Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about errors to inject into a Brick Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK2_CONFIG
 */

#ifndef __BRICK_MK2_ERROR_INFO_CPP__
#define __BRICK_MK2_ERROR_INFO_CPP__

#include "cfg_api_brick_mk2_types.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace brickmk2 {

/**
* Holds information about errors to inject into a Brick Mk2 device.
*/
class BrickMk2Errors
{
private:
    /* struct required by 'c' API to hold error info */
    STAR_CFG_BRICK_MK2_ERRORS errorInfo;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk2Errors()
    {
        errorInfo.decrementCredit = 0;
        errorInfo.escapeError = 0;
        errorInfo.incrementCredit = 0;
        errorInfo.insertFCT = 0;
        errorInfo.parityError = 0;
        errorInfo.suppressFCT = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param eInfo A STAR_CFG_BRICK_MK2_ERRORS struct which contains
    *              the error info values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit BrickMk2Errors(STAR_CFG_BRICK_MK2_ERRORS eInfo)
    {
        errorInfo.decrementCredit = eInfo.decrementCredit;
        errorInfo.escapeError = eInfo.escapeError;
        errorInfo.incrementCredit = eInfo.incrementCredit;
        errorInfo.insertFCT = eInfo.insertFCT;
        errorInfo.parityError = eInfo.parityError;
        errorInfo.suppressFCT = eInfo.suppressFCT;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param decrementCredit Specifies whether credit should be decremented to
    *                        cause an error.
    * @param escapeError Specifies whether an escape error should be injected.
    * @param incrementCredit Specifies whether credit should be incremented to
    *                        cause an error.
    * @param insertFCT Specifies whether an extra FCT should be inserted to
    *                  cause an error.
    * @param parityError Specifies whether a parity error should be injected.
    * @param suppressFCT Specifies whether an FCT should be suppressed to cause
    *                    an error.
    *
    */
    BrickMk2Errors(char decrementCredit, char escapeError,
                      char incrementCredit, char insertFCT, char parityError,
                      char suppressFCT)
    {
        errorInfo.decrementCredit = decrementCredit;
        errorInfo.escapeError = escapeError;
        errorInfo.incrementCredit = incrementCredit;
        errorInfo.insertFCT = insertFCT;
        errorInfo.parityError = parityError;
        errorInfo.suppressFCT = suppressFCT;
    }

    /**
    * Copy constructor - to allow copies of BrickMk2Errors objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    BrickMk2Errors(const BrickMk2Errors& eInfo)
    {
        /* copy contents of object being copied into this object */
        errorInfo.decrementCredit = eInfo.errorInfo.decrementCredit;
        errorInfo.escapeError = eInfo.errorInfo.escapeError;
        errorInfo.incrementCredit = eInfo.errorInfo.incrementCredit;
        errorInfo.insertFCT = eInfo.errorInfo.insertFCT;
        errorInfo.parityError = eInfo.errorInfo.parityError;
        errorInfo.suppressFCT = eInfo.errorInfo.suppressFCT;
    }

    /**
    * Overloaded assignment operator - to allow instances of BrickMk2Errors
    * objects to have their contents assigned to each other.
    *
    */
    BrickMk2Errors& operator=(const BrickMk2Errors& eInfo)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &eInfo)
            return *this;

        /* copy contents of object being assigned into this object */
        errorInfo.decrementCredit = eInfo.errorInfo.decrementCredit;
        errorInfo.escapeError = eInfo.errorInfo.escapeError;
        errorInfo.incrementCredit = eInfo.errorInfo.incrementCredit;
        errorInfo.insertFCT = eInfo.errorInfo.insertFCT;
        errorInfo.parityError = eInfo.errorInfo.parityError;
        errorInfo.suppressFCT = eInfo.errorInfo.suppressFCT;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~BrickMk2Errors()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether credit should be decremented to cause an error.
    *
    * @return 1 if the decrement credit element is set, otherwise 0.
    *
    */
    char IsDecrementCreditSet()
    {
        return errorInfo.decrementCredit;
    }

    /**
    * Get whether an escape error should be injected.
    *
    * @return 1 if the escape error element is set, otherwise 0.
    *
    */
    char IsEscapeErrorSet()
    {
        return errorInfo.escapeError;
    }

    /**
    * Get whether credit should be incremented to cause an error.
    *
    * @return 1 if the increment credit element is set, otherwise 0.
    *
    */
    char IsIncrementCreditSet()
    {
        return errorInfo.incrementCredit;
    }

    /**
    * Get whether an extra FCT should be inserted to cause an error.
    *
    * @return 1 if the insert FCT element is set, otherwise 0.
    *
    */
    char IsInsertFCTSet()
    {
        return errorInfo.insertFCT;
    }

    /**
    * Get whether a parity error should be injected.
    *
    * @return 1 if the parity error element is set, otherwise 0.
    *
    */
    char IsParityErrorSet()
    {
        return errorInfo.parityError;
    }

    /**
    * Get whether an FCT should be suppressed to cause an error.
    *
    * @return 1 if the suppress FCT element is set, otherwise 0.
    *
    */
    char IsSuppressFCTSet()
    {
        return errorInfo.suppressFCT;
    }

    /**
    * Set whether credit should be decremented to cause an error.
    *
    * @param value The value to set for the decrement credit element.
    *
    */
    void SetDecrementCredit(char value)
    {
        errorInfo.decrementCredit = value;
    }

    /**
    * Set whether an escape error should be injected.
    *
    * @param value The value to set for the escape error element.
    *
    */
    void SetEscapeError(char value)
    {
        errorInfo.escapeError = value;
    }

    /**
    * Set whether credit should be incremented to cause an error.
    *
    * @param value The value to set for the increment credit element.
    *
    */
    void SetIncrementCredit(char value)
    {
        errorInfo.incrementCredit = value;
    }

    /**
    * Set whether an extra FCT should be inserted to cause an error.
    *
    * @param value The value to set for the insert FCT element.
    *
    */
    void SetInsertFCT(char value)
    {
        errorInfo.insertFCT = value;
    }

    /**
    * Set whether a parity error should be injected.
    *
    * @param value The value to set for the parity error element.
    *
    */
    void SetParityError(char value)
    {
        errorInfo.parityError = value;
    }

    /**
    * Set whether an FCT should be suppressed to cause an error.
    *
    * @param value The value to set for the suppress FCT element.
    *
    */
    void SetSuppressFCT(char value)
    {
        errorInfo.suppressFCT = value;
    }

    /**
    * Return the STAR_CFG_BRICK_MK2_ERRORS struct which is held internally by
    * this object.
    *
    * @return A pointer to the STAR_CFG_BRICK_MK2_ERRORS structure which is
    *         held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_BRICK_MK2_ERRORS* GetAsSTAR_CFG_BRICK_MK2_ERRORS()
    {
        /* return pointer to the STAR_CFG_BRICK_MK2_ERRORS struct held
         * internally by this object. */
        return &errorInfo;
    }
};

/* end namespace tags */
} /* brickmk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

