/**
 * \file cpp_api_macros.hpp
 *
 * \brief Defines miscellaneous macros required by the C++ API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines miscellaneous macros required by the C++ API.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __CPP_API_PORT__
#define __CPP_API_PORT__

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

#define STAR_SPW_DEVICE_NAME_MAX_LENGTH             64
#define STAR_SPW_DEVICE_SERIAL_NUMBER_MAX_LENGTH    16

/* macro for unreferenced parameters */
#if !defined(_STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_)
    #define _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(a) ((void)(a))
#endif

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

