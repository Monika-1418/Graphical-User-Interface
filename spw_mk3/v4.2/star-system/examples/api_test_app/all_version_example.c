/**
 * \file all_version_example.c
 *
 * \brief Retreives and prints version information for all modules.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Uses the STAR_getAllVersions() function to retreive a list of versions of the
 * modules that are in use and prints the version information.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "utilities.h"

#include "star-api.h"

void allVersionExample()
{
    /* Define number of versions */
    U32 numberOfVersions = 0;

    /* Define counter for loop */
    unsigned int index;

    /* Get all versions */
    STAR_VERSION_INFO *versionInfos = STAR_getAllVersions(&numberOfVersions);

    /* For number of versions */
    for(index = 0; (versionInfos != NULL) && (index < numberOfVersions);
        index++)
    {
        /* Print version information string */
        printVersionInfo(&versionInfos[index]);

        /* If not the last version */
        if (index < (numberOfVersions - 1))
        {
            /* Print blank line to separate */
            printf("\n");
        }
    }

    /* Destroy version info string */
    STAR_destroyVersionInfoList(versionInfos);
}
