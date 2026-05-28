/**
 * \file config_port_errors.hpp
 *
 * \brief Holds information about errors that may be present on a device's
 *        configuration port.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about errors that may be present on a device's
 * configuration port.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_CONFIG_PORT_ERRORS_CPP__
#define __DEVICE_CONFIG_PORT_ERRORS_CPP__

#include "port_errors.hpp"
#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds information about errors that may be present on a device's configuration
* port. If a member of this object is set, then the error it represents is
* present.
*/
class ConfigPortErrors : public PortErrors
{
private:
     /** struct required by the STAR-API to hold config port errors */
     STAR_CFG_CONFIG_PORT_ERRORS configPortErrors;

     /**
     * Copy the contents of the given STAR_CFG_CONFIG_PORT_ERRORS struct into
     * the STAR_CFG_CONFIG_PORT_ERRORS held by this object.
     *
     * @param errorInfo A reference to a STAR_CFG_CONFIG_PORT_ERRORS struct to
     *                  copy from.
     */
     void CopyFromSTAR_CFG_CONFIG_PORT_ERRORS(STAR_CFG_CONFIG_PORT_ERRORS& errorInfo)
     {
        configPortErrors.cargoTooLarge = errorInfo.cargoTooLarge;
        configPortErrors.commandNotImplemented =
                                                errorInfo.commandNotImplemented;
        configPortErrors.earlyEEP = errorInfo.earlyEEP;
        configPortErrors.earlyEOP = errorInfo.earlyEOP;
        configPortErrors.errorCount = errorInfo.errorCount;
        configPortErrors.invalidDataCRC = errorInfo.invalidDataCRC;
        configPortErrors.invalidDataLength = errorInfo.invalidDataLength;
        configPortErrors.invalidDestinationKey =
                                                errorInfo.invalidDestinationKey;
        configPortErrors.invalidDestinationLogicalAddress =
                                     errorInfo.invalidDestinationLogicalAddress;
        configPortErrors.invalidHeaderCRC = errorInfo.invalidHeaderCRC;
        configPortErrors.invalidRegisterAddress =
                                              errorInfo.invalidRegisterAddress;
        configPortErrors.invalidRMWDataLength = errorInfo.invalidRMWDataLength;
        configPortErrors.lateEEP = errorInfo.lateEEP;
        configPortErrors.lateEOP = errorInfo.lateEOP;
        configPortErrors.portTimeoutError = errorInfo.portTimeoutError;
        configPortErrors.sourceLogicalAddressError =
                                            errorInfo.sourceLogicalAddressError;
        configPortErrors.sourcePathAddressError =
                                               errorInfo.sourcePathAddressError;
        configPortErrors.unsupportedProtocol = errorInfo.unsupportedProtocol;
        configPortErrors.unusedRMAPCommandOrPacketType =
                                        errorInfo.unusedRMAPCommandOrPacketType;
        configPortErrors.verifyBufferOverrun = errorInfo.verifyBufferOverrun;
     }

     /**
     * Copy the values of the member variables of the given ConfigPortErrors
     * object into this object.
     *
     * @param errorInfo A reference to a ConfigPortErrors object to copy from.
     */
     void CopyFromConfigPortErrors(const ConfigPortErrors& errorInfo)
     {
        configPortErrors.cargoTooLarge =
                            errorInfo.configPortErrors.cargoTooLarge;
        configPortErrors.commandNotImplemented =
                            errorInfo.configPortErrors.commandNotImplemented;
        configPortErrors.earlyEEP = errorInfo.configPortErrors.earlyEEP;
        configPortErrors.earlyEOP = errorInfo.configPortErrors.earlyEOP;
        configPortErrors.errorCount = errorInfo.configPortErrors.errorCount;
        configPortErrors.invalidDataCRC =
                            errorInfo.configPortErrors.invalidDataCRC;
        configPortErrors.invalidDataLength =
                            errorInfo.configPortErrors.invalidDataLength;
        configPortErrors.invalidDestinationKey =
                            errorInfo.configPortErrors.invalidDestinationKey;
        configPortErrors.invalidDestinationLogicalAddress =
                    errorInfo.configPortErrors.invalidDestinationLogicalAddress;
        configPortErrors.invalidHeaderCRC =
                            errorInfo.configPortErrors.invalidHeaderCRC;
        configPortErrors.invalidRegisterAddress =
                            errorInfo.configPortErrors.invalidRegisterAddress;
        configPortErrors.invalidRMWDataLength =
                            errorInfo.configPortErrors.invalidRMWDataLength;
        configPortErrors.lateEEP = errorInfo.configPortErrors.lateEEP;
        configPortErrors.lateEOP = errorInfo.configPortErrors.lateEOP;
        configPortErrors.portTimeoutError =
                            errorInfo.configPortErrors.portTimeoutError;
        configPortErrors.sourceLogicalAddressError =
                        errorInfo.configPortErrors.sourceLogicalAddressError;
        configPortErrors.sourcePathAddressError =
                            errorInfo.configPortErrors.sourcePathAddressError;
        configPortErrors.unsupportedProtocol =
                            errorInfo.configPortErrors.unsupportedProtocol;
        configPortErrors.unusedRMAPCommandOrPacketType =
                    errorInfo.configPortErrors.unusedRMAPCommandOrPacketType;
        configPortErrors.verifyBufferOverrun =
                                errorInfo.configPortErrors.verifyBufferOverrun;
     }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ConfigPortErrors() : PortErrors()
    {
        configPortErrors.cargoTooLarge = 0;
        configPortErrors.commandNotImplemented = 0;
        configPortErrors.earlyEEP = 0;
        configPortErrors.earlyEOP = 0;
        configPortErrors.errorCount = 0;
        configPortErrors.invalidDataCRC = 0;
        configPortErrors.invalidDataLength = 0;
        configPortErrors.invalidDestinationKey = 0;
        configPortErrors.invalidDestinationLogicalAddress = 0;
        configPortErrors.invalidHeaderCRC = 0;
        configPortErrors.invalidRegisterAddress = 0;
        configPortErrors.invalidRMWDataLength = 0;
        configPortErrors.lateEEP = 0;
        configPortErrors.lateEOP = 0;
        configPortErrors.portTimeoutError = 0;
        configPortErrors.sourceLogicalAddressError = 0;
        configPortErrors.sourcePathAddressError = 0;
        configPortErrors.unsupportedProtocol = 0;
        configPortErrors.unusedRMAPCommandOrPacketType = 0;
        configPortErrors.verifyBufferOverrun = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param errorInfo A STAR_CFG_CONFIG_PORT_ERRORS struct which contains
    *              the config port error values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit ConfigPortErrors(STAR_CFG_CONFIG_PORT_ERRORS& errorInfo) :
        PortErrors()
    {
        /* copy the contents of the given struct into the struct held by
         * this object */
        CopyFromSTAR_CFG_CONFIG_PORT_ERRORS(errorInfo);
    }

    /**
    * Copy constructor - to allow copies of ConfigPortErrors objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    ConfigPortErrors(const ConfigPortErrors& configErrorsToCopyFrom)
                                                      : PortErrors()
    {
        /* copy contents of object being copied into this object */
        CopyFromConfigPortErrors(configErrorsToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of ConfigPortErrors
    * objects to have their contents assigned to each other.
    *
    */
    ConfigPortErrors& operator=(const ConfigPortErrors& errorsBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &errorsBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        CopyFromConfigPortErrors(errorsBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~ConfigPortErrors()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether an error was recorded to indicate that an RMAP command packet
    * was too large.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetCargoTooLarge()
    {
        return configPortErrors.cargoTooLarge;
    }

    /**
    * Get whether an error was recorded to indicate that a valid RMAP command
    * code was used, but is not currently supported by the device.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetCommandNotImplemented()
    {
        return configPortErrors.commandNotImplemented;
    }

    /**
    * Get whether an error was recorded to indicate that a command packet was
    * terminated before the end of the packet with an EEP.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetEarlyEEP()
    {
        return configPortErrors.earlyEEP;
    }

    /**
    * Get whether an error was recorded to indicate that a command packet was
    * terminated before the end of the packet with an EOP.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetEarlyEOP()
    {
        return configPortErrors.earlyEOP;
    }

    /**
    * Get the number of errors present on the configuration port.
    *
    * @return The number of errors.
    *
    */
    char GetErrorCount()
    {
        return configPortErrors.errorCount;
    }

    /**
    * Get whether an error was recorded to indicate that a packet's data was
    * corrupted, i.e. the CRC didn't match as expected.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidDataCRC()
    {
        return configPortErrors.invalidDataCRC;
    }

    /**
    * Get whether an error was recorded to indicate that the data length of a
    * packet was incorrect.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidDataLength()
    {
        return configPortErrors.invalidDataLength;
    }

    /**
    * Get whether an error was recorded to indicate the presence of an invalid
    * destination key in a command packet.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidDestinationKey()
    {
        return configPortErrors.invalidDestinationKey;
    }

    /**
    * Get whether an error was recorded to indicate that the destination logical
    * address in the command packet is not the default value of 1.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidDestinationLogicalAddress()
    {
        return configPortErrors.invalidDestinationLogicalAddress;
    }

    /**
    * Get whether an error was recorded to indicate the presence of an invalid
    * header CRC.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidHeaderCRC()
    {
        return configPortErrors.invalidHeaderCRC;
    }

    /**
    * Get whether an unknown register address was given in the command packet or
    * a write is attempted to a read only register.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidRegisterAddress()
    {
        return configPortErrors.invalidRegisterAddress;
    }

    /**
    * Get whether a read-modify-write command data length was invalid. The
    * expected length is 8.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetInvalidRMWDataLength()
    {
        return configPortErrors.invalidRMWDataLength;
    }

    /**
    * Get whether a command packet was not terminated correctly and trailing
    * bytes were detected before the end of the packet.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetLateEEP()
    {
        return configPortErrors.lateEEP;
    }

    /**
    * Get whether a command packet was not terminated correctly and trailing
    * bytes were detected before the end of the packet.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetLateEOP()
    {
        return configPortErrors.lateEOP;
    }

    /**
    * Get whether a timeout event was detected by the configuration port
    * routing logic.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetPortTimeoutError()
    {
        return configPortErrors.portTimeoutError;
    }

    /**
    * Get whether an invalid source logical address was received.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetSourceLogicalAddressError()
    {
        return configPortErrors.sourceLogicalAddressError;
    }

    /**
    * Get whether an invalid source address path was received.
    *
    * \note This error is not used for the 10X.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetSourcePathAddressError()
    {
        return configPortErrors.sourcePathAddressError;
    }

    /**
    * Get whether a command packet was received with a protocol identifier which
    * is not the RMAP protocol identifier (0x01).
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetUnsupportedProtocol()
    {
        return configPortErrors.unsupportedProtocol;
    }

    /**
    * Get whether an unused command code was used or if a packet type was invalid.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetUnusedRMAPCommandOrPacketType()
    {
        return configPortErrors.unusedRMAPCommandOrPacketType;
    }

    /**
    * Get whether a verified write command was performed and the data length
    * was not 4.
    *
    * @return 1 if the error was present, otherwise 0.
    *
    */
    char GetVerifyBufferOverrun()
    {
        return configPortErrors.verifyBufferOverrun;
    }

};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

