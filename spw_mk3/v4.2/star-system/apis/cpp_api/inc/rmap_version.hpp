/**
 * \file rmap_version.hpp
 *
 * \brief Holds version information for the RMAP packet library.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds version information for the RMAP packet library.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_PACKET_LIBRARY__VERSION_CPP__
#define __RMAP_PACKET_LIBRARY__VERSION_CPP__

#include "rmap_packet_library.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Holds version information for the RMAP packet library.
*/
class RMAPVersion
{
private:
    /* the version information for the RMAP Packet Library */
    U32 version;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPVersion()
    {
        version = 0;
    }

    /**
    * Overloaded constructor. Initialise object to given value(s).
    *
    * @param v Version information to initialise object with.
    */
    explicit RMAPVersion(U32 v)
    {
        version = v;
    }

    /**
    * Copy constructor - to allow copies of RMAPVersion objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RMAPVersion(const RMAPVersion& versionObjToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        version = versionObjToCopyFrom.version;
    }

    /**
    * Overloaded assignment operator - to allow instances of RMAPVersion
    * objects to have their contents assigned to each other.
    *
    */
    RMAPVersion& operator=(const RMAPVersion& versionObjBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &versionObjBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        version = versionObjBeingAssigned.version;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~RMAPVersion()
    {
        /* no resources to free at present */
    }

    /**
    * Get the major version number for the RMAP packet library.
    *
    * @return The major version number.
    */
    U8 GetMajor()
    {
        return RMAP_GET_VERSION_MAJOR(version);
    }

    /**
    * Get the minor version number for the RMAP packet library.
    *
    * @return The minor version number.
    */
    U8 GetMinor()
    {
        return (U8)RMAP_GET_VERSION_MINOR(version);
    }

    /**
    * Get the edit number for the RMAP packet library version.
    *
    * @return The edit number for the version.
    */
    U16 GetEdit()
    {
        return RMAP_GET_VERSION_EDIT(version);
    }

    /**
    * Get the patch level for the RMAP packet library version.
    *
    * The patch level should be 0 in a release version of the RMAP packet
    * library.
    *
    * @return The patch level for the version.
    */
    U8 GetPatch()
    {
        return RMAP_GET_VERSION_PATCH(version);
    }

    /**
    * Get the version as a 32-bit number.
    *
    * The most significant 8 bits will contain the major version number, the
    * next significant 8 bits the minor version number, the next significant 10
    * bits the edit number, and the least significant 6 bits the patch level. In
    * a release version of the library the patch level should be 0.
    *
    * @return The version as a 32-bit number.
    */
    U32 GetVersion()
    {
        return version;
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

