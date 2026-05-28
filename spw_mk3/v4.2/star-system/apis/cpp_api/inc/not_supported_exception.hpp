/**
 * \file not_supported_exception.hpp
 *
 * \brief Defines exception which can occur if trying to use an unsupported
 *        function.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines exception which can occur when if trying to use a device
 * configuration function on an unsupported device.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __NOT_SUPPORTED_EXCEPTION_CPP__
#define __NOT_SUPPORTED_EXCEPTION_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Signifies an exception which can occur when attempting to call a device
* configuration function on an unsupported device.
*/
class NotSupportedException : public std::exception
{
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

