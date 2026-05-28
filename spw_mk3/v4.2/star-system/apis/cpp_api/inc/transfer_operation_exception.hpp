/**
 * \file transfer_operation_exception.hpp
 *
 * \brief Defines exceptions which may occur when creating and/or using
 *        transfer operations.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines exceptions which may occur when creating and/or using transfer
 * operation.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __TRANSFER_OPERATION_EXCEPTION_CPP__
#define __TRANSFER_OPERATION_EXCEPTION_CPP__

#include <exception>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace transferoperations {

/**
* Signifies an exception which may occur when attempting to create or process
* transfer operations.
*/
class TransferOperationException : public std::exception
{
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

