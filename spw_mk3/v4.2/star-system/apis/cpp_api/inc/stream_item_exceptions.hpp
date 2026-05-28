/**
 * \file stream_item_exceptions.hpp
 *
 * \brief Defines exceptions which may occur when creating and/or using
 *        different forms of stream item, e.g. Packets. TimeCodes, etc.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines exceptions which may occur when creating and/or using different
 * forms of stream item, e.g. Packets. TimeCodes, etc.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STREAM_ITEM_EXCEPTIONS_CPP__
#define __STREAM_ITEM_EXCEPTIONS_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Signifies an exception which may occur when attempting to create or process
* Addresses
*/
class AddressException : public std::exception
{
};


/**
* Signifies an exception which may occur when attempting to create or process
* packets.
*/
class PacketException : public std::exception
{
};

/**
* Signifies an exception which may occur when attempting to create or process
* Data Chunks.
*/
class DataChunkException : public std::exception
{
};

/**
* Signifies an exception which may occur when attempting to create or process
* time-codes.
*/
class TimeCodeException : public std::exception
{
};

/**
* Signifies an exception which may occur when attempting to create or process
* a general stream item.
*/
class StreamItemException : public std::exception
{
};

/**
* Signifies an exception which may occur when attempting to create or process
* an error in data stream item.
*/
class ErrorInDataException : public std::exception
{
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

