/**
 * \file api_version_example.c
 *
 * \brief Retreives the current API version and prints it.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Uses the STAR_getApiVersion() function to retreive the current API version
 * that is in use and prints it.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "utilities.h"

#include "star-api.h"

void apiVersionExample()
{
    /* Get API version info */
    STAR_VERSION_INFO * versionInfo = STAR_getApiVersion();

    /* Print version info */
    printVersionInfo(versionInfo);

    /* Destroy version info */
    STAR_destroyVersionInfo(versionInfo);
}
