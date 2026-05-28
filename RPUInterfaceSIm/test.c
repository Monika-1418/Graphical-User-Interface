 
//cc -pthread -O3 -g -lm  -c test.c -o test.o
//cc -pthread -O3 -g -lm -Wall -I../../inc/star  test.o  -lstar_conf_api_generic -lstar_conf_api_brick_mk3 -lstar_conf_api_pxi -lstar_conf_api_pci_mk2 -lstar_conf_api_brick_mk2 -lstar_conf_api_mk2 -lstar_conf_api_router -lstar_conf_api_router_mk2s -lstar_conf_api_gbe_brick -lstar-api -lrt -lpthread -o test



#include "star-api.h"

STAR_SPACEWIRE_ADDRESS *getTransmitPathAddress(int link);

int main()
{
    U8 prompt[7] = {0x76, 0xFF, 0x77, 0x4, 0x7E, 0x0, 0x4A};
    U8 *pTxBuffer = (U8 *)calloc(7, sizeof(U8));
    STAR_SPACEWIRE_ADDRESS *pAddress;

    int transmitLink = 1;
    int sizeOFbuffer = 8;
    int n = 0;

    STAR_STREAM_ITEM *pTxStreamItem = NULL;

    /* Select the path to be added to the front of packets transmitted */
    pAddress = getTransmitPathAddress(transmitLink);

    /* Prepend the transmit buffer with linkNum */
    pTxBuffer[0] = transmitLink;

    /* Fill pTxBuffer */
    for(n=0;n<sizeOFbuffer-1;n++)
    {
       *(pTxBuffer+1+n)=prompt[n];
         printf("%02x ",prompt[n]);
    }

    /* Create the packet to be transmitted */
    pTxStreamItem = STAR_createPacket(pAddress, (U8 *)pTxBuffer, sizeOFbuffer, STAR_EOP_TYPE_EOP);

    return 0;
}


STAR_SPACEWIRE_ADDRESS *getTransmitPathAddress(int link)
{
    STAR_SPACEWIRE_ADDRESS *pAddress;
    unsigned char newPath[1];

    newPath[0]=link;

    /* Create a SpaceWire address from the path with pathlength-1 */
    pAddress = STAR_createAddress(newPath, 1);

    /* Return the completed address */
    return pAddress;
}
