/**
 * \file cfg_api_pci_mk2_types.h
 *
 * \brief Types used with the STAR-Dundee PCI Mk2 Configuration API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */
#ifndef CFG_API_PCIMK2_TYPES_H
#define CFG_API_PCIMK2_TYPES_H


/**
 * \ingroup links
 *
 * Frequencies a link may run at. These are divided by
 * a divider set by CFG_PCIMK2_setLinkRateDivider() to
 * generate the desired link speed.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum pci_mk2_link_freq
{
    /** 120MHz */
    STAR_CFG_PCIMK2_LINK_FREQ_120 = 0,

    /**
     * 128MHz
     *
     * \versionChanged \ref changes_v2_0
     */
    STAR_CFG_PCIMK2_LINK_FREQ_128 = 5,

    /** 140MHz */
    STAR_CFG_PCIMK2_LINK_FREQ_140 = 1,

    /**
     * 150MHz
     *
     * \versionChanged \ref changes_v2_0
     */
    STAR_CFG_PCIMK2_LINK_FREQ_150 = 6,

    /** 160MHz */
    STAR_CFG_PCIMK2_LINK_FREQ_160 = 2,

    /** 180MHz */
    STAR_CFG_PCIMK2_LINK_FREQ_180 = 3,

    /** 200MHz */
    STAR_CFG_PCIMK2_LINK_FREQ_200 = 4
}
STAR_CFG_PCIMK2_LINK_FREQ;

#endif /*CFG_API_PCIMK2_TYPES_H */
