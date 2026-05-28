/**
* \file address.hpp
*
* \brief Represents a path or logical address, used to route a (SpaceWire)
*        packet to its intended destination.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Represents a path or logical address, used to route a (SpaceWire) packet to
* its intended destination.
*
* Copyright &copy; 2013 STAR-Dundee Ltd
*
* \ingroup CPP_STREAM_ITEMS
*/

#ifndef __STAR_SPACEWIRE_ADDRESS_CPP__
#define __STAR_SPACEWIRE_ADDRESS_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item_exceptions.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {


/**
* An address is used to route a packet through a SpaceWire network from a
* source node to a destination using the two main forms of addressing defined
* in the SpaceWire standard: path addressing and logical addressing.
*/
class Address
{
private:
    /** struct required by STAR-API to hold a packet address */
    STAR_SPACEWIRE_ADDRESS *pAddress;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Address() : pAddress(NULL)
    {
    }

    /**
    * Constructor - use given path and path length to automatically create
    * a SpaceWire address.
    *
    * @param[in] path The address path.
    * \note The contents of this buffer are copied into data structures managed
    *       by the STAR-System API. It is safe to dispose of this buffer after
    *       this function completes.
    * @param pathLen Length of the path buffer
    *
    * \throws AddressException If the Address fails to create.
    */
    Address(_In_count_(pathLen) unsigned char* path, U16 pathLen)
                                                 : pAddress(NULL)
    {
        /* create the address */
        int result = CreateAddress(path, pathLen);

        /* if error creating address */
        if (result == 0)
        {
            /* throw exception (can't return error code from constructor) */
            throw AddressException();
        }
    }

    /**
    * Overloaded constructor. Initialise the Address with the given
    * STAR_SPACEWIRE_ADDRESS struct.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * @param pSTAR_SPACEWIRE_ADDRESS A pointer to a STAR_SPACEWIRE_ADDRESS
    *                                struct.
    */
    explicit Address(STAR_SPACEWIRE_ADDRESS *pSTAR_SPACEWIRE_ADDRESS)
                         : pAddress(pSTAR_SPACEWIRE_ADDRESS)
    {
    }

    /**
    * Destructor. This will automatically destroy / dispose of the Address if
    * required, if it hasn't already been destroyed by the user (by calling
    * DestroyAddress()).
    */
    ~Address()
    {
        /* if spacewire address not destroyed */
        if (pAddress != NULL)
        {
            /* automatically destroy the address */
            DestroyAddress();
        }
    }

    /**
    * Copy constructor - invoked when a copy of a Address object is
    * made, e.g. such as through being a return value from a function or when
    * being passed into a function as a 'value' parameter.
    *
    * A new Address will be created which is a duplicate of the Address being
    * copied from.
    *
    * @param address The Address object which is being copied
    *
    * \throws AddressException If the Address fails to be copied.
    *
    */
    Address(const Address& address)
    {
        /* if the object being copied contains an initialised address struct */
        if (address.pAddress != NULL)
        {
            /* Perform deep copy of address data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create an address using the same data as the object
             * being copied.
             */
            int result = CreateAddress(address.GetPath(),
                address.GetPathLength());

            /* if error creating address */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw AddressException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pAddress = NULL;
        }
    }

    /**
    * Overloaded assignment operator - invoked when an attempt is made to assign
    * the contents of one Address object into another.
    *
    * Any existing Address will be destroyed and a new Address created which
    * is a duplicate of the Address being assigned.
    *
    * @param address The Address object whose contents are being
    *                assigned to 'this' object.
    *
    * \throws AddressException If the Address fails to be assigned / copied.
    */
    Address& operator=(const Address& address)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &address)
            return *this;

        /* delete contents of current address if any, they are about to replaced */
        DestroyAddress();

        /* copy contents of address being copied into 'this' */
        if (address.pAddress != NULL)
        {
            /* Perform deep copy of address data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create an address using the same data as the object
             * being copied.
             */
            int result = CreateAddress(address.GetPath(), address.GetPathLength());

            /* if error creating address */
            if (result == 0)
            {
                /* throw exception */
                throw AddressException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pAddress = NULL;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the length of the address
    *
    * @return The length of the address
    *
    */
    U16 GetPathLength() const
    {
        if (pAddress != NULL)
            return pAddress->pathLength;
        else
            return 0;
    }

    /**
    * Get the path
    *
    * @return An array containing the SpaceWire path address elements
    *
    */
    _Check_return_ U8* GetPath() const
    {
        if (pAddress != NULL)
            return pAddress->pPath;
        else
            return NULL;
    }

    /**
    * Creates a SpaceWire address
    *
    * @param[in] path The address path.
    * \note The contents of this buffer are copied into data structures managed
    *       by the STAR-System API. It is safe to dispose of this buffer after
    *       this function completes.
    * @param pathLen Length of the path buffer
    *
    * @return 1 if address created successfully, otherwise 0
    *
    */
    _Check_return_ int CreateAddress(_In_count_(pathLen) unsigned char* path,
                                     U16 pathLen)
    {
        /* if an address has already been created */
        if (pAddress != NULL)
        {
            /* delete the existing address */
            DestroyAddress();
        }

        /* call 'c' API to create a new spacewire address */
        pAddress = STAR_createAddress(path, pathLen);

        /* if address created successfully */
        if (pAddress != NULL)
        {
            /* return success */
            return 1;
        }
        else
        {
            /* return error code */
            return 0;
        }
    }

    /**
    * Free any resources held by the Address object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyAddress()
    {
        /* if valid address to destroy */
        if (pAddress != NULL)
        {
            /* call 'c' API to destroy the address */
            STAR_destroyAddress(pAddress);

            /* ensure address is set as empty */
            pAddress = NULL;
        }
    }

    /**
    * Gets the underlying STAR_SPACEWIRE_ADDRESS struct which is held internally
    * by the Address object.
    *
    * \note It is not usually necessary to use this function, it is
    *       used to support internal processing within the C++ API.
    *
    * @return A STAR_SPACEWIRE_ADDRESS object, or NULL if not available.
    */
    _Check_return_ STAR_SPACEWIRE_ADDRESS* GetAs_STAR_SPACEWIRE_ADDRESS()
    {
        return pAddress;
    }
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

