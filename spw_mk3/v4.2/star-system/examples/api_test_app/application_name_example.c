/**
 * \file application_name_example.c
 *
 * \brief Demonstrates the application name features.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Prints the initial application name, sets it to a new value and then prints
 * the new value.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "star-api.h"

void applicationNameExample()
{
    /* Get initial application name */
    char * applicationName = STAR_getApplicationName();

    if (applicationName != NULL)
    {
        /* Print initial application name */
        printf("Initial application name: %s\n", applicationName);

        /* Destroy application name */
        STAR_destroyString(applicationName);
    }

    /* Set application name */
    STAR_setApplicationName("Test STAR-System Application");

    /* Get application name */
    applicationName = STAR_getApplicationName();

    if (applicationName != NULL)
    {
        /* Print application name */
        printf("Application name: %s\n", applicationName);

        /* Destroy application name */
        STAR_destroyString(applicationName);
    }
}
