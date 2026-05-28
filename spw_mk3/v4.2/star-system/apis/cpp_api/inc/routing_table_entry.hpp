/**
 * \file routing_table_entry.hpp
 *
 * \brief Holds the content of a routing table entry.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds the content of a routing table entry.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_ROUTING_TABLE_ENTRY_CPP__
#define __DEVICE_ROUTING_TABLE_ENTRY_CPP__

#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds the content of a routing table entry.
*/
class RoutingTableEntry
{
private:
    /** struct required by STAR-API to hold routing table entry info */
    STAR_CFG_GAR_ENTRY routingTableEntry;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RoutingTableEntry()
    {
        routingTableEntry.deleteHeader = 0;
        routingTableEntry.invalidAddress = 0;
        routingTableEntry.portMask = 0;
        routingTableEntry.priority = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * @param tEntry A STAR_CFG_GAR_ENTRY struct which contains the routing
    *               table entry values to assign to this object.
    */
    explicit RoutingTableEntry(STAR_CFG_GAR_ENTRY tEntry)
    {
        routingTableEntry.deleteHeader = tEntry.deleteHeader;
        routingTableEntry.invalidAddress = tEntry.invalidAddress;
        routingTableEntry.portMask = tEntry.portMask;
        routingTableEntry.priority = tEntry.priority;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deleteHeader The 'delete header' status to set.
    * @param invalidAddress The 'invalid address' status to set.
    * @param portMask The bitmask of output ports to arbitrate for.
    * @param priority The 'priority' status to set.
    *
    */
    RoutingTableEntry(char deleteHeader, char invalidAddress, U32 portMask,
                      char priority)
    {
        routingTableEntry.deleteHeader = deleteHeader;
        routingTableEntry.invalidAddress = invalidAddress;
        routingTableEntry.portMask = portMask;
        routingTableEntry.priority = priority;
    }

    /**
    * Copy constructor - to allow copies of RoutingTableEntry objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RoutingTableEntry(const RoutingTableEntry& tEntryToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        routingTableEntry.deleteHeader =
            tEntryToCopyFrom.routingTableEntry.deleteHeader;
        routingTableEntry.invalidAddress =
            tEntryToCopyFrom.routingTableEntry.invalidAddress;
        routingTableEntry.portMask =
            tEntryToCopyFrom.routingTableEntry.portMask;
        routingTableEntry.priority =
            tEntryToCopyFrom.routingTableEntry.priority;
    }

    /**
    * Overloaded assignment operator - to allow instances of RoutingTableEntry
    * objects to have their contents assigned to each other.
    *
    */
    RoutingTableEntry& operator=(const RoutingTableEntry& tEntryBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &tEntryBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        routingTableEntry.deleteHeader = tEntryBeingAssigned.routingTableEntry.deleteHeader;
        routingTableEntry.invalidAddress = tEntryBeingAssigned.routingTableEntry.invalidAddress;
        routingTableEntry.portMask = tEntryBeingAssigned.routingTableEntry.portMask;
        routingTableEntry.priority = tEntryBeingAssigned.routingTableEntry.priority;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~RoutingTableEntry()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether this routing table entry is set to remove the leading header
    * byte of the input packet before it is transferred to the output port.
    *
    * @return Whether the 'delete header' status is set.
    *
    */
    char IsDeleteHeaderSet()
    {
        return routingTableEntry.deleteHeader;
    }

    /**
    * Set whether this routing table entry should remove the leading header
    * byte of the input packet before it is transferred to the output port.
    *
    * @param deleteHeader 1 to set 'delete header' status, otherwise 0.
    *
    */
    void SetDeleteHeader(char deleteHeader)
    {
        routingTableEntry.deleteHeader = deleteHeader;
    }

    /**
    * Get whether the the corresponding logical address for this routing table
    * entry is invalid. In this case, any packets arriving at the router with an
    * invalid address are split and an address error is reported in the port
    * status register.
    *
    * @return Whether the 'invalid address' status is set.
    *
    */
    char IsInvalidAddressSet()
    {
        return routingTableEntry.invalidAddress;
    }

    /**
    * Set whether the the corresponding logical address for this routing table
    * entry is invalid. In this case, any packets arriving at the router with an
    * invalid address are split and an address error is reported in the port
    * status register.
    *
    * @param invalidAddress 1 to set 'invalid address' status, otherwise 0.
    *
    */
    void SetInvalidAddress(char invalidAddress)
    {
        routingTableEntry.invalidAddress = invalidAddress;
    }

    /**
    * Get a bitmask of the output ports that this routing table entry / logical
    * address will arbitrate for. Valid bits set are 1 through 28.
    *
    * \note Bit 1 corresponds to port 1
    *       It is not possible to access the configuration port (0) through
    *       logical addresses.
    *
    * @return A bitmask of the output ports that this routing table entry /
    *         logical address will arbitrate for.
    *
    */
    U32 GetPortMask()
    {
        return routingTableEntry.portMask;
    }

    /**
    * Set a bitmask of the output ports that this routing table entry / logical
    * address will arbitrate for. Valid bits set are 1 through 28.
    *
    * \note Bit 1 corresponds to port 1
    *       It is not possible to access the configuration port (0) through
    *       logical addresses.
    *
    * @param portMask A bitmask of the output ports that this routing table
    *                 entry / logical address will arbitrate for.
    *
    */
    void SetPortMask(U32 portMask)
    {
        routingTableEntry.portMask = portMask;
    }

    /**
    * Get whether the priority status of this routing table entry is set. In
    * this case, packets with the logical address for this entry with the
    * priority bit set will be granted access to a particular output port in
    * preference to packets whose logical addresses in the routing table have
    * their priority bit set to zero.
    *
    * @return Whether the 'priority' status is set.
    *
    */
    char IsPrioritySet()
    {
        return routingTableEntry.priority;
    }

    /**
    * Set the priority status of this routing table entry. In
    * this case, packets with the logical address for this entry with the
    * priority bit set will be granted access to a particular output port in
    * preference to packets whose logical addresses in the routing table have
    * their priority bit set to zero.
    *
    * @param priority 1 to set the 'priority' status, otherwise 0.
    *
    */
    void SetPriority(char priority)
    {
        routingTableEntry.priority = priority;
    }

    /**
    * Return the STAR_CFG_GAR_ENTRY struct which is held
    * internally by this object.
    *
    * @return A pointer to the STAR_CFG_GAR_ENTRY structure which
    *         is held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_GAR_ENTRY *GetAsSTAR_CFG_GAR_ENTRY()
    {
        /* return pointer to the STAR_CFG_GAR_ENTRY struct held
         * internally by this object. */
        return &routingTableEntry;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

