/**
 * \file remote_device_exception.hpp
 *
 * \brief Defines exceptions which may occur when creating and/or using
 *        remote device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines exceptions which may occur when creating and/or using remote
 * devices.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __REMOTE_DEVICE_EXCEPTION_CPP__
#define __REMOTE_DEVICE_EXCEPTION_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Signifies an exception which may occur when attempting to create or process
* remote devices.
*/
class RemoteDeviceException : public std::exception
{
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

