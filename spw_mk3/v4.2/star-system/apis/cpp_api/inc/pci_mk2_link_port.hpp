/**
 * \file pci_mk2_link_port.hpp
 *
 * \brief Represents a Link Port on a PCI Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a PCI Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCI_MK2_CONFIG
 */

#ifndef __DEVICE_PCI_MK2_LINK_PORT_CPP__
#define __DEVICE_PCI_MK2_LINK_PORT_CPP__

#include "mk2_link_port.hpp"

#include "not_supported_exception.hpp"
#include "cfg_api_pci_mk2.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace pcimk2 {

/**
* Represents a Link Port on a PCI Mk2 device.
*/
class PCIMk2LinkPort : public Mk2LinkPort
{
public:
    /** Frequencies that a link may run at.
      * These are divided by a divider set by SetLinkRateDivider() to generate
      * the desired link speed. */
    enum LinkFrequency
    {
        /** 120 MHz */
        LINK_FREQUENCY_120MHZ = STAR_CFG_PCIMK2_LINK_FREQ_120,
        /** 128 MHz */
        LINK_FREQUENCY_128MHZ = STAR_CFG_PCIMK2_LINK_FREQ_128,
        /** 140 MHz */
        LINK_FREQUENCY_140MHZ = STAR_CFG_PCIMK2_LINK_FREQ_140,
        /** 150 MHz */
        LINK_FREQUENCY_150MHZ = STAR_CFG_PCIMK2_LINK_FREQ_150,
        /** 160 MHz */
        LINK_FREQUENCY_160MHZ = STAR_CFG_PCIMK2_LINK_FREQ_160,
        /** 180 MHz */
        LINK_FREQUENCY_180MHZ = STAR_CFG_PCIMK2_LINK_FREQ_180,
        /** 200 MHz */
        LINK_FREQUENCY_200MHZ = STAR_CFG_PCIMK2_LINK_FREQ_200
    };

    /**
    * Default constructor. Initialise object to default state.
    */
    PCIMk2LinkPort() : Mk2LinkPort()
    {
    }

    /**
    * Overloaded constructor. Initialise object to given value(s).
    *
    * @param idOfOwningDevice The ID of the device which this port forms a part
    *                         of.
    * @param type The type of port.
    * @param portNum The port number.
    * @param statusControl The port's status / control register value.
    *
    */
    PCIMk2LinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Mk2LinkPort(
                                                     idOfOwningDevice,
                                                     type,
                                                     portNum,
                                                     statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of PCIMk2LinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    PCIMk2LinkPort(const PCIMk2LinkPort& portToCopyFrom) : Mk2LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of PCIMk2LinkPort
    * objects to have their contents assigned to each other.
    *
    */
    PCIMk2LinkPort& operator=(const PCIMk2LinkPort& portBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &portBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        owningDeviceForPort = portBeingAssigned.owningDeviceForPort;
        portType = portBeingAssigned.portType;
        portNumber = portBeingAssigned.portNumber;
        portStatusControl = portBeingAssigned.portStatusControl;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~PCIMk2LinkPort()
    {
        /* no resources to free at present */
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int EnableStateChangeEvents()
    {
        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetStateChangeEventsEnabled(_Out_ int *pEnabled)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pEnabled);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int DisableStateChangeEvents()
    {
        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int EnableSpeedChangeEvents()
    {
        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetSpeedChangeEventsEnabled(_Out_ int *pEnabled)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pEnabled);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int DisableSpeedChangeEvents()
    {
        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetMeasuredLinkSpeed(_Out_ U16 *pLinkSpeed)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pLinkSpeed);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * Get the clock frequency for the link.
    *
    * @param[out] pLinkFreq Pointer to a value that will be updated with the
    *                       link's clock frequency.
    *
    * @return 1 clock frequency successfully obtained, otherwise 0.
    *
    */
    int GetLinkClockFrequency(_Out_ LinkFrequency *pLinkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        STAR_CFG_PCIMK2_LINK_FREQ linkFreq;
        int result = CFG_PCIMK2_getLinkClockFrequency(owningDeviceForPort,
                                                      portNumber, &linkFreq);

        /* store frequency in input variable provided */
        (*pLinkFreq) = (LinkFrequency)linkFreq;

        /* return result */
        return result;
    }

    /**
    * Set the clock frequency for the link.
    *
    * @param[out] linkFreq Link clock frequency to be set.
    *
    * @return 1 clock frequency successfully set, otherwise 0.
    *
    */
    int SetLinkClockFrequency(LinkFrequency linkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        return CFG_PCIMK2_setLinkClockFrequency(owningDeviceForPort, portNumber,
                                           (STAR_CFG_PCIMK2_LINK_FREQ)linkFreq);
    }
};

/* end namespace tags */
} /* pcimk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

