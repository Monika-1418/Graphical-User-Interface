/**
 * \file rmap_packet_exception.hpp
 *
 * \brief Represents an exception which may occur when creating and/or using
 *        RMAP packets.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an exception which may occur when creating and/or using
 * RMAP packets.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_PACKET_EXCEPTION_CPP__
#define __RMAP_PACKET_EXCEPTION_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an exception which may occur when creating and/or using
 * RMAP packets.
*/
class RMAPPacketException : public std::exception
{
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

