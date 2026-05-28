/**
* \file external_trigger_configuration.hpp
*
* \brief Provides functions for configuring external triggering on a device.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for configuring external triggering on a device.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_CONF
*/

#ifndef __EXTERNAL_TRIGGER_CONFIGURATION_CPP__
#define __EXTERNAL_TRIGGER_CONFIGURATION_CPP__

#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting external trigger
* configuration parameters.
*/
class ExternalTriggerConfiguration
{
public:
    /**
    * Disables output for a specific external trigger.
    * When disabled, the external trigger's output action does not cause the
    * trigger output signal to be pulsed.
    *
    * @param extTrigger External trigger to disable output on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableExtTriggerOutput(U32 extTrigger) = 0;

    /**
    * Enables output for a specific external trigger.
    * When enabled, the external trigger's output action causes the trigger
    * output signal to be pulsed for a duration specified by the extend
    * duration value.
    *
    * @param extTrigger External trigger to enable output on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableExtTriggerOutput(U32 extTrigger) = 0;

    /**
    * Disables invert for a specific external trigger.
    * When disabled, the input trigger signal is not modified before the
    * detection mechanism.
    *
    * @param extTrigger External trigger to disable invert on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableExtTriggerInvert(U32 extTrigger) = 0;

    /**
    * Enables edge detect mode for a specific external trigger.
    * When enabled, the trigger will be set on the rising edge of the input
    * trigger signal.
    *
    * @param extTrigger External trigger to enable edge detect mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableExtTriggerEdgeDetectMode(U32 extTrigger) = 0;

    /**
    * Enables invert for a specific external trigger.
    * When enabled, the input trigger signal is inverted before the detection
    * mechanism.
    *
    * @param extTrigger External trigger to enable invert on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableExtTriggerInvert(U32 extTrigger) = 0;

    /**
    * Sets the extend duration for a specific external trigger.
    * The extend duration is the duration, in cycles, of the trigger output
    * signal pulse when a trigger output action occurs.
    *
    * @param extTrigger External trigger to set extend duration on.
    * @param extend Extend duration in cycles.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetExtTriggerExtend(U32 extTrigger, U32 extend) = 0;

    /**
    * Gets the extend duration for a specific external trigger.
    * The extend duration is the duration, in cycles, of the trigger output
    * signal pulse when a trigger output action occurs.
    *
    * @param extTrigger External trigger to get the extend duration for.
    * @param pExtend Point to value to be updated with the extend duration in
    *                cycles.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetExtTriggerExtend(U32 extTrigger, U32 *pExtend) = 0;

    /**
    * Disables edge detect mode for a specific external trigger.
    * When disabled, the trigger will always be set when the input trigger is
    * high.
    *
    * @param extTrigger External trigger to disable edge detect mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableExtTriggerEdgeDetectMode(U32 extTrigger) = 0;

    /**
    * Gets whether invert is enabled for a specific external trigger.
    *
    * @param extTrigger External trigger to check.
    * @param pEnabled User supplied value that will be updated to 1 if invert
    *                 is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetExtTriggerInvertEnabled(U32 extTrigger, U32 *pEnabled) = 0;

    /**
    * Gets whether output is enabled for a specific external trigger.
    *
    * @param extTrigger External trigger to check.
    * @param pEnabled User supplied value that will be updated to 1 if output
    *                 is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetExtTriggerOutputEnabled(U32 extTrigger, U32 *pEnabled) = 0;

    /**
    * Gets whether edge detect mode is enabled for a specific external trigger.
    *
    * @param extTrigger External trigger to check.
    * @param pEnabled User supplied value that will be updated to 1 if edge
    *                 detect mode is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetExtTriggerEdgeDetectModeEnabled(U32 extTrigger,
        U32 *pEnabled) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

