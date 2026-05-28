/**
 * \file deprecation_exceptions.hpp
 *
 * \brief Defines exceptions which may occur when trying to use a function that 
 *        has been deprecated.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines exceptions which may occur when trying to use a function that has 
 * been deprecated.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __DEPRECATION_EXCEPTIONS_CPP__
#define __DEPRECATION_EXCEPTIONS_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Signifies an exception which may occur when attempting to use a method that 
* has been deprecated.
*/
class MethodDeprecatedException : public std::exception
{
};

/**
* Signifies an exception which may occur when attempting to use a constructor 
* that has been deprecated.
*/
class ConstructorDeprecatedException : public std::exception
{
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

