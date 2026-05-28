/**
 * \file version_information.hpp
 *
 * \brief Holds version information for a device, driver, or API module.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds version information for a device, driver, or API module.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_SYSTEM_CPP_VERSION_INFORMATION__
#define __STAR_SYSTEM_CPP_VERSION_INFORMATION__

#include "version.h"
#include "star-api.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Holds version information for a device, driver, or API module.
*/
class VersionInformation
{
public:
    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of version name strings */
        STR_VERSION_NAME_LEN = STAR_STR_MAX_LEN,
        /** maxmimum expected length of version author strings */
        STR_VERSION_AUTHOR_LEN = STAR_STR_MAX_LEN
    };

private:
    /* the version number, split into the following form:
     * 'majorNumber.minorNumber (editNumber) pPatchNumber'
     */
    U16 m_major;
    U16 m_minor;
    U16 m_edit;
    U16 m_patch;

    /* the version's name */
    char m_name[STR_VERSION_NAME_LEN];

    /* the version's author */
    char m_author[STR_VERSION_AUTHOR_LEN];

    /**
    * Private member function. Populate the contents of a VersionInformation
    * object using the contents of a given STAR_VERSION_INFO structure.
    *
    * @param[in] versionInfo The STAR_VERSION_INFO structure used to populate
    *                        the VersionInformation object.
    */
    void PopulateFrom_STAR_VERSION_INFO(STAR_VERSION_INFO* versionInfo)
    {
        /* if version info structure is valid */
        if (versionInfo != NULL)
        {
            /* copy version data to object */
            CopyString(m_name, STR_VERSION_NAME_LEN, versionInfo->name);
            CopyString(m_author, STR_VERSION_AUTHOR_LEN, versionInfo->author);
            m_major = versionInfo->major;
            m_minor = versionInfo->minor;
            m_edit = versionInfo->edit;
            m_patch = versionInfo->patch;
        }
        else
        {
            /* reset contents to empty */
            m_major = m_minor = m_edit = m_patch = 0;
            m_name[0] = '\0';
            m_author[0] = '\0';
        }
    }

    /**
    * Copy the contents of the source string into the destination buffer with
    * the given buffer size.
    *
    * @param dest A pointer to the destination buffer.
    * @param destSize The size of the destination buffer.
    * @param source A pointer to the source string.
    *
    */
    void CopyString(char *dest, unsigned int destSize, const char *source)
    {
        /* set destination to empty string */
        dest[0] = '\0';

        /* if no valid source */
        if (!source)
        {
            /* end */
            return;
        }

        /* if destination buffer has zero size */
        if (destSize < 1)
        {
            /* end */
            return;
        }

        /* get length of source string */
        size_t sourceLen = strlen(source);

        /* if source string empty */
        if (sourceLen == 0)
        {
            /* end */
            return;
        }

        /* if length of source string exceeds desination buffer size */
        if (sourceLen > (destSize-1))
        {
            /* truncate length of source string */
            sourceLen = destSize - 1;
        }

        /* copy the contents of the source string to the destination buffer */
        for (unsigned int i=0; i<sourceLen; i++)
            dest[i] = source[i];

        /* append a null terminator */
        dest[sourceLen] = '\0';
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    VersionInformation() : m_major(0), m_minor(0), m_edit (0), m_patch(0)
    {
        m_name[0] = '\0';
        m_author[0] = '\0';
    }

    /**
    * Constructor - initialise object members to the contents of a specified
    * STAR_VERSION_INFO structure.
    *
    */
    explicit VersionInformation(STAR_VERSION_INFO *vInfo) :
        m_major(0), m_minor(0),
        m_edit (0), m_patch(0)
    {
        m_name[0] = '\0';
        m_author[0] = '\0';

        /* populate contents of object using struct provided */
        PopulateFrom_STAR_VERSION_INFO(vInfo);
    }

    /**
    * Copy constructor - to allow copies of version information objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    */
    VersionInformation(const VersionInformation& versionInfo)
    {
        m_name[0] = '\0';
        m_author[0] = '\0';

        /* copy contents of versionInfo provided into 'this' version object */
        CopyString(m_name, STR_VERSION_NAME_LEN, versionInfo.m_name);
        CopyString(m_author, STR_VERSION_AUTHOR_LEN, versionInfo.m_author);
        m_major = versionInfo.m_major;
        m_minor = versionInfo.m_minor;
        m_edit = versionInfo.m_edit;
        m_patch = versionInfo.m_patch;
    }

    /**
    * Overloaded assignment operator - to allow instances of version information
    * objects to have their contents assigned to each other.
    */
    VersionInformation& operator=(const VersionInformation& versionInfo)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &versionInfo)
            return *this;

        /* copy contents of versionInfo provided into 'this' version object */
        CopyString(m_name, STR_VERSION_NAME_LEN, versionInfo.m_name);
        CopyString(m_author, STR_VERSION_AUTHOR_LEN, versionInfo.m_author);
        m_major = versionInfo.m_major;
        m_minor = versionInfo.m_minor;
        m_edit = versionInfo.m_edit;
        m_patch = versionInfo.m_patch;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Return name of module
    */
    char* GetName()
    {
        return m_name;
    }

    /**
    * Set the version name
    *
    * @param[in] _name A null-terminated string containing the version name.
    *
    * \note If the length of the string provided exceeds
    *       VersionInformation::STR_VERSION_NAME_LEN it will be truncated.
    */
    void SetName(_In_z_count_c_(STR_VERSION_NAME_LEN) const char *_name)
    {
        CopyString(m_name, STR_VERSION_NAME_LEN, _name);
    }

    /**
    * Return author of module
    */
    char* GetAuthor()
    {
        return m_author;
    }

    /**
    * Set author of module
    *
    * @param[in] _author A null-terminated string containing the version author.
    *
    * \note If the length of the string provided exceeds
    *       VersionInformation::STR_VERSION_AUTHOR_LEN it will be truncated.
    */
    void SetAuthor(_In_z_count_c_(STR_VERSION_NAME_LEN) const char *_author)
    {
        CopyString(m_author, STR_VERSION_AUTHOR_LEN, _author);
    }

    /**
    * Return major version number of module
    */
    U16 GetMajor()
    {
        return m_major;
    }

    /**
    * Set major version number of module
    *
    * @param[in] m The major version number to set
    */
    void SetMajor(U16 m)
    {
        m_major = m;
    }

    /**
    * Return major version number of module
    */
    U16 GetMinor()
    {
        return m_minor;
    }

    /**
    * Set minor version number of module
    *
    * @param[in] m The minor version number to set
    */
    void SetMinor(U16 m)
    {
        m_minor = m;
    }

    /**
    * Return the edit number of the module.
    */
    U16 GetEdit()
    {
        return m_edit;
    }

    /**
    * Return the patch number of the module.
    *
    * \note Edit will be 0 if patch is non zero.
    */
    U16 GetPatch()
    {
        return m_patch;
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

