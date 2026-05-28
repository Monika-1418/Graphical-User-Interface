//--===========================================================================--
//-- File Name           : RS_PDS_spwtest_MKII.c
//-- Project Name    	 : Proximity Docking Sensor (PDS), SPADEX
//-- Design Name         : PDS
//-- Dependant Library   : stdio, stdlib, string, time and math
//-- Description         : C program that uses SpaceWire Dundee MK-II brick APIs for testing PDS
//-- Revision Number     : 1.0
//-- Revision Date       : ---
//-- Revision Descrpn.   : ---
//-- Author              : ---
//-- Verified By         : ---
//--===========================================================================--

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include<signal.h>
 
#include "star-api.h"
#include "cfg_api_router.h"
#include "cfg_api_mk2.h"
#include "utility.h"
#include "star-dundee_types.h"
 
 
 /* Menu option codes */
#define MENU_SEND				1
#define	MENU_EXIT				0
#define MENU_RESET				2
#define MENU_CHU_RESET                          3


#define RX_BUFFER_SIZE 				50000000
#define TX_BUFFER_SIZE 				900


/* Global Variables */
U8 g_usbport;	        /* The number of the device's USB port */
char	g_headermode;	/* Whether or not header mode was enabled when the device was opened */
char	g_networkmode;	/* Whether or not network mode was enabled when the device was opened */
int cmnd_index;		/* Command index*/
int sizeOFbuffer;	/* Length of command buffer */
int pix_width;	/* Whether Image data is 8 bit or 16 bit*/
U8 hk;			/* Whether HK data is requested*/
int flines;		/* No. of lines requested in Full Frame mode*/
int wsizeg,wnum;	/* Window size and no. of windows*/
int wbytes=0;		/* No. of bytes expected in window mode*/
int fbytes=0;		/* No. of bytes expected in Full Frame mode*/
int nframes;		/* No. of frames*/
int delay;
int flag_err;		/* Error in a frame*/
int stline,stpix;
float intgtim;
U8 *tempBuffer;
U8 *pTxBuffer;		/*Tranmit Buffer*/
U8 *pRxBuffer;          /*Receive Buffer*/
U8 *threshData;		
U8 prompt[1000];	/*Command Array*/
unsigned long  byteSize = 0U ;
STAR_CHANNEL_ID rxChannelId = 0U, txChannelId = 0U;
char ***ptrToArg=NULL;
FILE *logFile;
FILE *fsumI;
int  framelength;
 
void terminate(int a);
void ResetSPWUSB();
void ResetPDS();
void DisplayMenu();
void Test();
U8 *AllocateMemChar(unsigned long size);
void Initialization();
void FileInitialization();
void TransmitAndReceive();	  
void FormatData(); 
 
int getData();
 
void convert(int val);
int FFdefaultValues();
int WMdefaultValues();
int FFPrompt();
int WMPrompt();
int HKdefaultValues();
void ConvertU16ToBytes(int num, int arr[2]);
 
int pass_cntr=0, n=0,mode,z,errorCount=0,flag_errCount=0,last=0;
int transmitLink=1;
U8 thresh_display[1024][1024];
int ff_write[1024][1024];
U8 ff8_write[1024][1024];
int twrite[1048576];
int hkdata[16];

int winData[15][60][60],a=0,intensity=0,linenum=0,pixelnum=0,win_data=0,win_id=0;
int line[60],pixel[60],winData_id=0;
int bytesReceived = 0;
int rcvdata_index = 0;
int max, xl,xp;
int last_line_read,fifo_full_flag;
int file_out = 0;
int win_err = 0;
int testNum = 0;
unsigned int rxPacketCount=0;


STAR_TRANSFER_OPERATION *pTxTransferOp = NULL, *pRxTransferOp = NULL;
STAR_STREAM_ITEM *pTxStreamItem = NULL;
STAR_DEVICE_ID deviceID;
STAR_TRANSFER_STATUS rxStatus, txStatus;
STAR_SPACEWIRE_ADDRESS *pAddress;
STAR_STREAM_ITEM *pRxStreamItem;
FILE *testfile, *txtfile, *centfile;
double  duration;
float pivotL[8],pivotP[8];
char file_pre[200] = "images/";
char user_file[200];
char command_str[200];
char nframes_char[5];
char test_filename[40],img_filename[200],txt_filename[200],temp_filename[200];
 
float avgside = 0;
float avgbgnd = 0;
float avgcpi = 0;
float avgsumi = 0;
float avgcentL = 0;
float avgcentP = 0;
float avgfratio = 0;
float minL = 1024.0; 
float minP = 1024.0;
float maxL = -1.0; 
float maxP = -1.0;
int maxLIdx = 1;
int minLIdx = 1;
int maxPIdx = 1;
int minPIdx = 1;
float maxSum = -1.0; 
float minSum = 100000.0;

//CA
/*{0x4E, 0x54, 0x71},//RS QM

{0x4F, 0x62, 0x71}    ,  {0x4F, 0x68, 0x70} ---REPEATED //PDS FSM
{0XE8, OXE4, OXDE}  // RS FM1 
{0x47, 0x43, 0x43} // RS FM2 
{0x5F, 0x6A, 0x6E}   //PDS QM
{0x2E, 0X32, 0X41}  // PDS FM1
{0x65, 0X61, 0X5f}  // PDS FM2   {0x5C, 0X5C, 0X64} ----REPEATED

//PGA
{0x04, 0xF5, 0xD6}, //RS QM
{0X00, OXF1, OXD2}  // RS FM1 
{0xFC, 0xED, 0xD2} // RS FM2 
{0x08, 0xFD, 0xEA} , {0x04, 0xF5, 0XE2} ---REPEATED //PDS FSM
{0x00, 0xF1, 0xD6}  //PDS QM
{0XFC, 0XE9, 0XCA}  // PDS FM1
{0XFC, 0XED, 0XDA}  // PDS FM2  {0XFC, 0XF1, 0XDA} --- REPEATED
*/

int ColAmp[2][3]= {{0x4F, 0x68, 0x70},
                   {0x4F, 0x68, 0x70}}; 

int PGA[2][3] = {{0x04, 0xF5, 0XE2},
                {0x04, 0xF5, 0XE2}};

U8 in_colamp, in_pga;
int winsize=10;

clock_t t_start, t_tx, t_rx;
double tx_time, rx_time;

typedef union 
{
  unsigned short int num;
  
  struct BYTES
  {
    unsigned char arg[2];
    
  }byte;
  
}BYTE_TO_WORD;

	
/******************************************************************************/
/*                           SPACEWIRE_BRICK                                  */
/*  Tests the driver's ability to send packets. Fills buffer with CHU commands*/
/*  and transmits it out of a link and receives data from CHU.                */
/******************************************************************************/

int main(int argc, char *argv[])
{
	int menuSelect;
	 
	char bExit = 0;
	char command[200];
        int gain,selectCam = 0;
        
        printf("Select 0 for RS, 1 for PDS\n");
        scanf("%d",&selectCam);
	ptrToArg=&argv;



 
        char logFilePath[50]="logsheet.txt";
	logFile = fopen(logFilePath,"w"); 

	fsumI = fopen("fsumI.txt","w");
		
	if (logFile == NULL)
	{
	  printf("Error: Trouble opening log file. No logs will be created. \n");
	  return 0;
	}
	
	/* Print out Driver, API and Test Program Versions */
	printf("\n\n+++++++++++++++++++++++++++++++++++++++Welcome to PDS Testing++++++++++++++++++++++++++++++++++++++++++++\n\n");
	printf("\n\tABOUT : This Program aids the testing of Proximity Docking Sensor using STAR Dundee SpaceWire Brick\n\n");
	printf("\n\n=======================================SpaceWire Brick Details===========================================\n");
	clock_t start;
	start = GET_TIME();
	fprintf(logFile, "%d %s", start,"\n");
	
	menuSelect=atoi(ptrToArg[0][1]);
	mode=atoi(ptrToArg[0][3]);

        //System Call to iniate Spw service
	snprintf(command,sizeof(command),"service star_device_config restart");
	system(command);

        mode=atoi(ptrToArg[0][3]);

        intgtim = 0;

        if(mode != 5)
         {

           printf("\nEnter Integration Time(microsec): ");
           scanf ("%f",&intgtim);

           printf("\nEnter Gain(1/2/4): ");
           scanf ("%d",&gain);
         

       if ( gain == 1)
        {
          in_colamp = ColAmp[selectCam][0];
          in_pga = PGA[selectCam][0];
        }
       else if ( gain == 2)
        {
          in_colamp = ColAmp[selectCam][1];
          in_pga = PGA[selectCam][1];
        }
       else if ( gain == 4)
        {
          in_colamp = ColAmp[selectCam][2];
          in_pga = PGA[selectCam][2];
        }
       else
        {
          printf("Wrong gain selection.\nExiting...\n");
          return;
        }
       }

     if ( mode == 4)
        {
           printf("\nEnter Window Size(10/20/60): ");
           scanf ("%d",&winsize);
        }

        printf("\nEnter File Name: ");
        scanf("%s",user_file);
 
 
  
        
        signal(SIGINT, terminate);
        signal(SIGSEGV, terminate);
	
	DisplayInformation();
	
	/* Try to open the SpaceWire USB device */
        SetConfiguration(atoi(ptrToArg[0][2]));
        
     

		
		/* Loop until exit option is chosen */
		while (!bExit)
		{
			printf("\n\n\n");
		  
			switch(menuSelect)
			{
				case MENU_SEND:
					Test();
					menuSelect=0;
					break;
				
				case MENU_RESET:
					ResetSPWUSB();
					SetConfiguration(atoi(ptrToArg[0][2]));
					menuSelect=0;
					break;
	 
				case MENU_CHU_RESET:
					ResetPDS();
					menuSelect=0;
					break;
	
				case MENU_EXIT:
					printf("\nExiting SpaceWire USB test program\n");
					bExit = 1;
					break;
	
				default:
					printf("\nERROR: Incorrect menu option\n");
					break;
			}
			 
		}
		
        
        

	fclose(logFile);
        logFile = NULL;

	fclose(fsumI);
        fsumI = NULL;
 
	printf("Exiting...\n\n");
	return 0;
}




/******************************************************************************/
/*  Displays the options menu.                                                */
/******************************************************************************/
void DisplayMenu()
{
	printf("------------TEST BEGINS -----------------------:\n");
	printf("SpaceWire USB Test Program. Select Option:\n");
	printf("(1) Send Commands and Receive Data\n");
	printf("(2) Reset Brick\n");
        printf("(3) Reset PDS\n");
	printf("(0) Exit\n\n");
	printf("Please Select Menu Option: ");
}

/******************************************************************************/
/*  Outputs the status to screen                                              */
/******************************************************************************/
void OutputStatus(STAR_TRANSFER_STATUS status)
{
	fprintf(logFile,"%s","Status = ");
	switch (status)
	{
		case STAR_TRANSFER_STATUS_NOT_STARTED:
		   
			fprintf(logFile,"%s","Not yet started.\n");
			break;
	
		case STAR_TRANSFER_STATUS_STARTED :
		        
		        fprintf(logFile,"%s","Transfer has begun.\n");
			 
			break;

		case STAR_TRANSFER_STATUS_COMPLETE :
			fprintf(logFile,"%s","Transfer has completed.\n");
			 
			break;

		case STAR_TRANSFER_STATUS_CANCELLED :
			fprintf(logFile,"%s","Transfer was cancelled.\n");
			 
			break;

		case STAR_TRANSFER_STATUS_ERROR :
			fprintf(logFile,"%s","An error occurred while processing the transfer.\n");
			 
			break;

		default:
			fprintf(logFile,"%s","Unknown Result state!\n");
			 
			break;
	}
}



void DisposeAll( STAR_CHANNEL_ID *txChannelId,  STAR_CHANNEL_ID *rxChannelId, 
	STAR_STREAM_ITEM **pTxStreamItem, STAR_TRANSFER_OPERATION **pTxTransferOp, 
	STAR_TRANSFER_OPERATION **pRxTransferOp,  STAR_SPACEWIRE_ADDRESS **pAddress)
{
  

  
	 
      /* Dispose of the transfer operations */
   if (*pTxTransferOp != NULL)
    {
	STAR_disposeTransferOperation(*pTxTransferOp);
	fprintf(logFile,"%s","***Successfully disposed transmit transfer operation.\n");
	 
    }
     if (*pRxTransferOp != NULL)
    {
        fprintf(logFile,"%s","Successfully disposed receive transfer operation.\n");
	 
	STAR_disposeTransferOperation(*pRxTransferOp);
    }

    /* Destroy the packet transmitted */
    if (*pTxStreamItem != NULL)
    {
        fprintf(logFile,"%s","Successfully disposed transmit stream.\n");
	 
	STAR_destroyStreamItem(*pTxStreamItem);
    }

 

    /* Free the address path */
     if (*pAddress != NULL)
    {
        fprintf(logFile,"%s","Successfully disposed transmit address.\n");
	 
	STAR_destroyAddress(*pAddress);
    }
 
    /* Close the channels */
     if (*rxChannelId != 0U )
    {
        fprintf(logFile,"%s","Successfully disposed receive channel ID.\n");
	 
	STAR_closeChannel(*rxChannelId);
    }
     if (*txChannelId != 0U )
    {
        fprintf(logFile,"%s","Successfully disposed transmit channel ID.***\n");
        
	STAR_closeChannel(*txChannelId);
    }
}

void Initialization()
{
  
        nframes = 0;
	 
	nframes=atoi(ptrToArg[0][5]);
	
	
			
	/* Initiate receive, send the commands through transmit link and receive through the same link */
	  fprintf(logFile,"%s","Initiating transmit and receive via SpaceWire Brick Mk-II.\n");
       
	  printf("=====Initiating transmit and receive via SpaceWire Brick Mk-II=====\n\n"); 	
	
	/* Select the transmit device and channel to be used */
	if (chooseDeviceAndChannelID("transmit", &txChannelId, STAR_CHANNEL_DIRECTION_OUT,&deviceID) == 0 )
	{	   
	  return;
	}

         
	/* Get transmit link and check for validity */
        transmitLink=atoi(ptrToArg[0][2]);
	 
	rxPacketCount=0;
	if ((transmitLink < 1) || (transmitLink > 2))
	  {
	        fprintf(logFile,"%s","ERROR: Incorrect link number.");
		printf("ERROR: Incorrect link number\n");
		return;
	  } 
	else
	  printf("Transmit Link is-%d\n",transmitLink);
	
		
	/* Select the receive device and channel to be used */
	if (chooseDeviceAndChannelID("receive", &rxChannelId, STAR_CHANNEL_DIRECTION_IN, &deviceID) == 0)
	  {
	  DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	  return;
	  }
	
 	/*Fill the command array with commands and return operating mode*/
	if (getData()!=1)   
	  { 
	    DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    fprintf(logFile,"%s","ERROR: While filling command buffer.\n");
	    return;
	  }
	  
	  
	printf("\n\nNOTE- Previous files at the destined path will be overwritten if filename is same.\n");
	
        /* Allocate memory for transmit buffer */
	pTxBuffer = (unsigned char *)AllocateMemChar(TX_BUFFER_SIZE);
              
	if (pTxBuffer == NULL)
	  {
	       fprintf(logFile,"%s","ERROR: Unable to allocate memory for buffer.\n");
	       return;
	  }
	     
	 /*Fill Transmit Buffer */
	  pTxBuffer[0] = transmitLink;
	  printf("\nCommands in transmit buffer are--\n");
	 
	for (n=0;n<(sizeOFbuffer-1);n++)
	    { 
	     *(pTxBuffer+1+n)=prompt[n];	    
	      printf("%02x  ",prompt[n]);           
	    }
	
	printf("\n"); 
	 
	 
	 /* Create receive operation to receive 1 packet */
	pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
	
	if (pRxTransferOp == NULL)
	  {
	    fprintf(logFile,"%s","ERROR: Unable to create receive operation.\n");
	    DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    return;
	  }
	  
	 /* Create the packet to be transmitted */
	pTxStreamItem = STAR_createPacket(pAddress, (U8 *)pTxBuffer, sizeOFbuffer, STAR_EOP_TYPE_EOP);
	  
	if (pTxStreamItem == NULL)
	  {
	    fprintf(logFile,"%s","ERROR: Unable to create the packet to be transmitted.\n");
	    DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    return;
	  }
	  
	  
	/* Create the transmit transfer operation for the packet */
	pTxTransferOp = STAR_createTxOperation(&pTxStreamItem, 1);
	  
	if (pTxTransferOp == NULL)
	   {
	     fprintf(logFile,"%s","ERROR: Unable to create the transfer operation to be transmitted.\n");
	     DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	     return;
	   }
          
          centfile = NULL;
  	
	  centfile = fopen("cent.txt","w");	/* .txt file containing centroids */

}

void FileInitialization()
{

          char file_pre_temp[200]= "images/multi/";

 

          if (nframes > 1)
          {   
              
           //  strcat(file_pre_temp,user_file);

 
             strcpy(file_pre, file_pre_temp);
          //    puts(file_pre_temp);

           
              
          }
 




          printf("\n\nTest Frame No. %d...\n", testNum+1);
	  fprintf(logFile,"%s %d %s","Test Frame No.",testNum+1,"\n");
 
          //sprintf(temp_filename,"%s_%d",file_pre,testNum);
          sprintf(temp_filename,"%s%s_%d",file_pre,user_file,testNum);
          puts(temp_filename);
 

          sprintf(test_filename,"%s.test",temp_filename);
          sprintf(img_filename,"%s.pgm",temp_filename);
          sprintf(txt_filename,"%s.txt",temp_filename);
          puts(test_filename);
          puts(img_filename);
          puts(txt_filename);

          
          testfile = NULL;
          txtfile = NULL;
          
	  
	  testfile = fopen(test_filename,"w");	/* .test file required to generate pgm image */
	  txtfile = fopen(txt_filename,"w");	/* .txt file containing pixel intensities */
	   
  
}

/******************************************************************************/


/*  Tests the SpaceWire USB device. Fills buffer with user data,              */
/*  transmits it out of one link. Writes received data to file.               */
/*  If Image mode, image is generated for 8-bit mode and pixel intensities    */
/*  are stored if 16-bit. If Threshold and Sobel mode, image is displayed     */
/*  for 8-bit mode and pixel intensities are stored for 16-bit mode.          */
/*  If window mode, window data is stored.                                    */
/******************************************************************************/
void Test()
{
	
 
   int i,j,k,l;  
   

   Initialization();	
   	
   
   t_start = GET_TIME();
   printf("start time = %d\n",t_start);	
   
   for (testNum=0;testNum<nframes;testNum++)
       {	  
	  FileInitialization();

          pRxBuffer = NULL;
	  
	  t_tx = GET_TIME()-t_start;
	  //tx_time = ((double)t_tx)/CLOCKS_PER_SEC;
	  printf("Tx start time = %f\n",t_tx/1000000.0);
          TransmitAndReceive();	  
	  //t_rx = clock()-t_start;
	  //rx_time = ((double)t_rx)/CLOCKS_PER_SEC;
	  //printf("Rx end time = %d\n",rx_time);
	  FormatData(); 
  
	    
	  if ((testfile != NULL) && (ferror(testfile)) )	      
	       printf("%s %d %s","Write error in file_write. After writing ",bytesReceived," bytes\n");
 	    
	   /* Destroy the packet received */
	  if (bytesReceived >0 && pRxStreamItem != NULL)
	   {
	     STAR_destroyStreamItem(pRxStreamItem);
	   }
         
          if (pRxBuffer !=NULL)
	     free(pRxBuffer);

	    fprintf(logFile,"%s %d %s","***Receive buffer free for frame ",testNum+1," ***\n");
	    bytesReceived=0;
 	    
	    
	   
	    
	}
printf("\n\nFocussing: avgcpi-%f avgside-%f avgbgnd-%f, avgfocusratio-%f, maxLtominL-%f, maxPtominP-%f, avgcentL-%f, avgcentP-%f, maxLIdx-%d,minLIdx-%d,maxPIdx-%d,minPIdx-%d\n\n",avgcpi/nframes,avgside/nframes,avgbgnd/nframes,avgcpi/avgside,maxL- minL, maxP-minP, avgcentL/nframes, avgcentP/nframes, maxLIdx,minLIdx,maxPIdx,minPIdx);

fprintf(txtfile, "\n\nFocussing: avgcpi-%f avgside-%f avgbgnd-%f, avgfocusratio-%f, maxLtominL-%f, maxPtominP-%f, avgcentL-%f, avgcentP-%f,maxLIdx-%d,minLIdx-%d,maxPIdx-%d,minPIdx-%d\n",avgcpi/nframes,avgside/nframes,avgbgnd/nframes,avgcpi/avgside,maxL- minL, maxP-minP, avgcentL/nframes, avgcentP/nframes, maxLIdx,minLIdx,maxPIdx,minPIdx);

        if (testfile != NULL)
	  {
             fclose(testfile); 
             testfile =NULL;
          }

        if (txtfile != NULL)
	 {
             fclose(txtfile); 
             txtfile =NULL;
          }

        if (centfile != NULL)
	 {
             fclose(centfile); 
             centfile =NULL;
          }
	
 
        if (pTxBuffer !=NULL)
	{
	    free(pTxBuffer);
	    fprintf(logFile,"%s","Transmit buffer free.\n");
	}
 
	printf("Pass Tests : %d\n",pass_cntr);
	printf("Failed Tests : %d \n", errorCount);
	printf("Tests with Data mismatch : %d \n",flag_errCount);
        printf("Tests with Window error : %d",win_err);
	
	if ((errorCount == 0) && (flag_errCount==0))
	{
	  printf("\n\n++++++++TEST SUCCESSFULL!!!!+++++++++\n");
	  fprintf(logFile,"%s","TEST SUCCESSFULL!\n");
	}
	else
	{	
	  printf("\n\n--------TEST FAILED!!!!-----------\n");
	  fprintf(logFile,"%s","TEST FAILED!\n");
	     
	}
	
	
	DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
}


void TransmitAndReceive()
{
     int l;
    /* Allocate memory for receive buffer */
	  pRxBuffer = (unsigned char *)AllocateMemChar(RX_BUFFER_SIZE);
	  
	  
         /*Dont Create receive Operation again if only 1 frame is tested*/
         if (nframes > 1)
	  {
	      pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
	      
	      if (pRxTransferOp == NULL)
	      {
		  fprintf(logFile,"%s","ERROR: Unable to create receive operation.\n");
		  DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
		  return;
	      }
 
	  }
   	  
	  /* Submit the receive operation */
	 if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
	  {
	    printf("\nERROR: occurred during receive submit.  Test %d failed.\n", testNum + 1);
	    fprintf(logFile,"%s %d %s","ERROR: Occurred during receive submit. Test ",testNum+1," failed.\n");
	     
	  }
	  else
	  { 
	   fprintf(logFile,"%s","Receive operation transferred...\n");
	    
	  }
	  
	  max =0;
          xl=0;
          xp =0;
 
	    /* Submit the transmit operation */
	  if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
	    {
	       printf("\nERROR: occurred during transmit submit.  Test %d failed.\n", testNum + 1);
	       fprintf(logFile,"%s %d %s","ERROR: Occurred during transmit submit. Test ",testNum+1," failed.\n");
	        
	    }
	  else
	    {
	       fprintf(logFile,"%s","Transmit operation transferred...\n");
	      
	    }
	    
	     /*Wait on the transmit operation to complete*/
	  txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp, 1000);
	    
	  if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
	    {
	       printf("\nERROR: Couldn't transmit commands.  Test %d failed.\n", testNum + 1);
	       fprintf(logFile,"%s %d %s","ERROR: Couldn't transmit commands. Test ",testNum+1," failed.\n");
	       //errorCount++;
	       OutputStatus(txStatus);
            }
	  else
	    {
	      printf("=====Transmitted commands successfully=====\n\n");
	      fprintf(logFile,"%s","Transmitted commands successfully.\n"); 
	      
	      if(mode !=5)	//Other than HK mode
	      {
		     PerformTickIn(deviceID);  
	      }
	      
	    }
	    
	  printf("\n.\n.\n.\nWaiting to receive....\n\n"); 
	  
	  /* Wait on the receive operation completing */
	  rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp, 2000);
	  bytesReceived = 0;
	    
	  if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
	    {
	        printf("\nERROR: Couldn't receive data from PDS.  Test %d failed.\n", testNum + 1);
	        fprintf(logFile,"%s %d %s","ERROR: Couldn't receive data from PDS. Test ",testNum+1," failed.\n");
	        errorCount++;
	        OutputStatus(rxStatus);	       
	    }
	  else
 	    {
	      printf("\n========Received data successfully=======\n");
	      fprintf(logFile,"%s","Received data successfully.\n");
	           
	    }	
	    
	    
	  /*Get Received Data */
          rxPacketCount=0;
	  if (rxStatus == STAR_TRANSFER_STATUS_COMPLETE)
              rxPacketCount = STAR_getTransferItemCount(pRxTransferOp);

	  if (rxPacketCount != 1)
	     {
	       fprintf(logFile,"%s %u %s","ERROR: expected to receive 1 packet but received ",rxPacketCount,".\n");
	      
	     }
	  else
	     {
	      /* For each traffic item received */
	      for (l = 0U; l < rxPacketCount; l++)
	      {
		 pRxStreamItem = STAR_getTransferItem(pRxTransferOp, l);
		 pRxBuffer = (char *)STAR_getPacketData((STAR_SPACEWIRE_PACKET *)pRxStreamItem->item, &bytesReceived);

		
	      }
	    }
 	   
	  flag_err=0;
	    
	  printf("\nSize of data received : %d bytes.\n",bytesReceived);
	  fprintf(logFile,"%s %d %s","Size of data received : ",bytesReceived," bytes.\n");
  
  
  
  
}

void FormatData ()
{
  
  float  sumval, sumsq, temp1, temp2,avgsum,totalpix;
  float dist,linemoment,pixmoment,sumi,centL,centP,fratio;
  float winavgbgnd[15],winsigma[15],sigmathresh[15],winsumi[15],wincentL[15],wincentP[15];
  float colsum[60],lineintarr[60],pixintarr[60];
  int indx,flagcenterr,wincpi[15] , j,k,i,ktr, ktr2; 
    
   int a1, b1, c1, d1;
	  if (((mode == 5) && (bytesReceived != 16)) || ((mode == 4) && (bytesReceived != wbytes)) || ((mode == 1) && (bytesReceived != fbytes))) /* HK mode || window mode || FF image mode */
	    {    
	   
	       fprintf(logFile,"%s","ERROR: Mismatch in expected and received bytes.\n");
               flag_err = 1;
	       flag_errCount++;
	         
	    }
	  else
	    {
	       pass_cntr++;
	       fprintf(logFile,"%s %d %s","Test ",testNum+1," Passed.\n");   
	      
	    } 
	    
	    
	    
	  if ((mode == 5)  && (flag_err != 1))  //HK data
	    {
	      
	      for(j=0;j<bytesReceived;j++)
               {  
                 printf("%02x ",pRxBuffer[j]);
               }
	      
	         printf("\n\nHK data saved at images/frame_%d.txt\n",testNum);
	         j=0;
	    }
	    
	  //Start: Image mode    
	  if ((mode == 1)  && (flag_err != 1))  //Image mode
	    {  
	      if (pix_width == 0) //save 16 bit data as pixels (generates .txt file)
	      {
		 for (j=0;j<=flines;j++)
		    {
		       for (k=0;k<framelength;k++)
		         {
		              intensity = pRxBuffer[2*(k+j*(framelength+1))]*256;

		              ff_write[j][k]=intensity + pRxBuffer[(2*(k+j*(framelength+1)))+1]; 
                          
		              if (ff_write[j][k] >= max)
		                {
                                   max = ff_write[j][k];
			           xl = j;
			           xp = k;

		                }
                               
                               fprintf(txtfile,"%d ",ff_write[j][k]);
		           }
		           
		          fprintf(txtfile,"\n");
		        }
			printf("HK in Full frame\n");
                      for(j=bytesReceived-16;j<bytesReceived;j++) 
                         printf("%02x ",pRxBuffer[j]);
                     
		        
	      }
             else if (pix_width == 1) //save 8 bit data as pixels and generate image (.txt,.test,.pgm)
	        {
		   
                      tempBuffer = (unsigned char *)AllocateMemChar(RX_BUFFER_SIZE);
                     

		      for(j=0;j<=flines;j++)  
		         {
		            for(k=0;k<(framelength+1);k++)
		               {
		                 ff_write[j][k]=pRxBuffer[k+j*(framelength+1)];   /*initialize array with zeroes*/
                                 fprintf(txtfile,"%04d ",ff_write[j][k]);

		                 if (ff_write[j][k] > max)
		                    {
                                       max = ff_write[j][k];
                                       xl = j;
			               xp = k;

		                     }
		               }
		               fprintf(txtfile,"\n");
    		          }
                
            		if(framelength == 1023)
		           fwrite(pRxBuffer, 1, 1024*1024, testfile); 
                        else
                          {
                             ktr = 0; ktr2=0;
                             for(j=0; j<1024; j++) 
                              {
                                for(k=0; k<1024; k++) 
                                {
                                   if ((j>=stline) && (j <= stline+flines) && (k >= stpix) && (k <= stpix+framelength)) 
                                     {
                                        tempBuffer[ktr2] = pRxBuffer[ktr]; 
                                        ktr = ktr + 1;
                                        
                                     }
                                   else
                                     { 
                                        tempBuffer[ktr2] = 0;  
                                     }
                                  
                                     ktr2 = ktr2 + 1;

                                   
                                } 
                    
                                
                             }

                            fwrite(tempBuffer, 1, 1024*1024, testfile); 

                         }
                    
                         if (tempBuffer !=NULL)
                   	     free(tempBuffer);
 
		         snprintf(command_str,sizeof(command_str),"cat apsdmp.hdr %s > %s",test_filename,img_filename);
		         system(command_str);

		         printf("\n\nNOTE- Full frame image can be seen at %s\n",img_filename);
			
			//printf("HK in Full frame\n");			
                      //for(j=bytesReceived-8;j<bytesReceived;j++) 
                         //printf("%02x ",pRxBuffer[j]);
		         
	        } 
	      

	      printf("\n\nNOTE- Full frame data written at %s\n",txt_filename);
	      printf("\n\nMax...l-%d , p- %d, i-%d.txt\n",xl,xp,max);

               
	     
	    }//mode == 1
	    //Start: Image mode    
	    
	    
	    //Start: Threshold mode    
	     if( (mode == 2)  && (flag_err != 1) ) //Threshold
	    {   
	        threshData = (unsigned char *)AllocateMemChar(RX_BUFFER_SIZE);
		
 	        if (hk==1)
		    a=(bytesReceived-16)/6;
		else
		    a=(bytesReceived)/6;
		
		 /*Extracting line number and pixel number and intensity from received buffer into twrite[] */
		 for (j=0;j<a;j++)//Should be changed to 'a'
		     {		
		        linenum=(pRxBuffer[6*j]&0x0F)*256;
		        twrite[3*j]=linenum|pRxBuffer[6*j+1];
		        pixelnum=(pRxBuffer[6*j+2]&0x0F)*256;
		        twrite[3*j+1]=pixelnum|pRxBuffer[6*j+3];
		        intensity=(pRxBuffer[6*j+4]&0x0F)*256;
		        twrite[3*j+2]=intensity|pRxBuffer[6*j+5];
		 
		        threshData[(twrite[3*j]*1024) + twrite[3*j+1] ] = pRxBuffer[6*j+5];
		  
		       fprintf(txtfile,"%d\t\t%d\t\t%d\n",twrite[3*j],twrite[3*j+1],twrite[3*j+2]); 
		    
		     }

	 
		    if (a > 0)
		      {  
		        fprintf(logFile, "%s", "Pixel Data and HK bytes received.\n");
		      }
		    else
		     { 
                        if (bytesReceived == 16)
		          fprintf(logFile, "%s", "Only HK bytes received.\n");
		     }
		
   
            printf("\n\nNOTE- Full frame thresholded data written at %s\n",txt_filename);
	      
	      
	    }
	    //End: Threshold mode    
	    
	    
	    
	    //Start: Sobel mode    
	    if ((mode == 3)  && (flag_err != 1)) //Sobel Modes
	    {   
                j=0;
	        i=0;
		
		threshData = (unsigned char *)AllocateMemChar(RX_BUFFER_SIZE);
		last_line_read = (pRxBuffer[bytesReceived-2]*256) | pRxBuffer[bytesReceived-1];
		fifo_full_flag = (pRxBuffer[bytesReceived-4]*256) | pRxBuffer[bytesReceived-3];
		printf("\nLast line read = %04d     FIFO full flag = %04d\n",last_line_read,fifo_full_flag);
		
		/*Extracting line number and pixel number and intensity from received buffer into twrite[] */
		for (j=0;j<(bytesReceived-16);)
		    {
		       fprintf(txtfile,"%02x%02x\t %d\n",pRxBuffer[j],pRxBuffer[j+1],j);
		       j = j+2;		  
		      if (((pRxBuffer[j]& 0xFF)>>4) == 1)
		       { 		    
		         linenum = (pRxBuffer[j]&0x0F)*256 |  pRxBuffer[j+1];
		         last = 1;
		         j = j+2;
		       }
		     else if(((pRxBuffer[j]& 0xF0)>>4) == 2)
		      { 
	            pixelnum = (pRxBuffer[j]&0x0F)*256 | pRxBuffer[j+1];
		        last = 2;
		        j = j+2; 
		      }
                    else if(((pRxBuffer[j]& 0xF0)>>4) == 3)
		      { 		   
		         if (last == 3 )
		         {
		      
		           twrite[i++]=linenum;
		           pixelnum = pixelnum + 1;
		           twrite[i++] = pixelnum;
		           intensity = (pRxBuffer[j]&0x0F)*256;
		           twrite[i++] = intensity | pRxBuffer[j+1];
		      
		           threshData[ (linenum*1024) + pixelnum ] = pRxBuffer[j+1];
		      
		           j=j+2;
		           fprintf(txtfile,"%d\t\t%d\t\t%d\n",linenum,pixelnum,twrite[i-1]);
  
		        }
                      else if(last == 2)
		        {
		           twrite[i++] = linenum;
		           twrite[i++] = pixelnum-2;
		           intensity = (pRxBuffer[j]&0x0F)*256;
		           twrite[i++] = intensity | pRxBuffer[j+1];
		           fprintf(txtfile,"%d\t\t%d\t\t%d\n",linenum,pixelnum-2,twrite[i-1]);
		      
                           twrite[i++] = linenum;
		           twrite[i++] = pixelnum-1;
		           intensity = (pRxBuffer[j+2]&0x0F)*256;
		           twrite[i++] = intensity | pRxBuffer[j+3];
		           fprintf(txtfile,"%d\t\t%d\t\t%d\n",linenum,pixelnum-1,twrite[i-1]);
		      
		           twrite[i++] = linenum;
		           twrite[i++] = pixelnum;
		           intensity = (pRxBuffer[j+4]&0x0F)*256;
		           twrite[i++] = intensity | pRxBuffer[j+5];
		           fprintf(txtfile,"%d\t\t%d\t\t%d\n",linenum,pixelnum,twrite[i-1]);
		      
		          threshData[ (linenum*1024) + pixelnum-2 ] = pRxBuffer[j+1];
		          threshData[ (linenum*1024) + pixelnum-1 ] = pRxBuffer[j+3];
		          threshData[ (linenum*1024) + pixelnum ] = pRxBuffer[j+5];
		  	    
		          j=j+6;
		      
		         }
		    
		        last = 3;		   
		    
                     }	//else if(((pRxBuffer[j]& 0xF0)>>4) == 3)   
		  
		   }//for loop
		
		    // Create Image
                    // fwrite(threshData, 1, 1024*1024, testfile); 
                    // snprintf(command_str,sizeof(command_str),"cat apsdmp.hdr %s > %s",test_filename,img_filename);
                    // system(command_str);

		    if (a > 0)
		      {  
		        fprintf(logFile, "%s", "Pixel Data and HK bytes received.\n");
		      }
		    else
		     { 
                        if (bytesReceived == 16)
		          fprintf(logFile, "%s", "Only HK bytes received.\n");
		     }    
	      
	    }//if mode == 3
           //End: Sobel mode    
   
  
          //Start: Window mode    
          if (mode==4 && flag_err != 1) //Window mode
	    {
        
		for(z=0;z<15;z++)
		  for(j=0;j<60;j++)
		    for(k=0;k<60;k++)
		      winData[z][j][k]=0;		//Filling winData[][][] with zeroes
		      
		
		for(j=0;j<60;j++)
		{
		  line[j]=0;
		  pixel[j]=0;
		}
        
	      win_data = wnum*wsizeg*wsizeg;
		 
	      for (z=0; z < win_data; z++)
	          {
		      win_id = floor(pRxBuffer[2*z]/16);
		  
		      j = line[win_id];
		      k = pixel[win_id];
		      a = pRxBuffer[2*z] - (win_id*16);
		      winData[win_id][j][k]= (a*256)|pRxBuffer[2*z+1];

		     //winData_id= ((pRxBuffer[2*z]*256)|pRxBuffer[2*z+1])-winData[win_id][j][k];
		     //printf("\nwinData_id = %04x",winData_id);
		     if (k == wsizeg-1)
		     {
		       pixel[win_id] = 0;
		       line[win_id]=line[win_id]+1;
		     }
		     else
		     {
		        pixel[win_id] = pixel[win_id]+1;
		     }
	         }
	         
	      printf("\n\nNOTE- Window mode data written at %s\n",txt_filename);
              
	   
        
	  printf("HK in Win mode\n");
                      for(j=bytesReceived-16;j<bytesReceived;j++) 
                         {
                            printf("%02x ",pRxBuffer[j]);
                            hkdata[j-(bytesReceived-16)] = pRxBuffer[j];
                         }

     
	  
	    
	    
	    for(z=0;z<wnum;z++)
	      {
		sumsq = 0;
		avgsum = 0;
     
		  /*Sigma Theshold */
		for(j=0;j<wsizeg;j++)
		{
		  temp1 = winData[z][j][0];
		  temp2 = winData[z][j][wsizeg-1];
		  avgsum = avgsum + temp1;
		  avgsum = avgsum + temp2;
		  sumsq = sumsq + temp1*temp1;
		  sumsq = sumsq + temp2*temp2;
		  
		}
		  
		for(k=1;k<wsizeg-1;k++)
		{
		  temp1 = winData[z][0][k];
		  temp2 = winData[z][wsizeg-1][k];
		  avgsum = avgsum + temp1;
		  avgsum = avgsum + temp2;
		  sumsq = sumsq + temp1*temp1;
		  sumsq = sumsq + temp2*temp2;
		  
		}
		
		totalpix = (4*(wsizeg-1));
		winavgbgnd[z] = avgsum/totalpix;
		
		temp2 =  (sumsq/totalpix)-winavgbgnd[z]*winavgbgnd[z];		
		winsigma[z] = sqrt(temp2);	
		
		sigmathresh[z] = 4.5*winsigma[z];		//3.5*winsigma[z];		
		
		// Capture LineSum, Rowsum
		
		for(indx = 0; indx < wsizeg; indx++)
		{
		  colsum[indx] = 0;
		}
		
                lineintarr[0] = 0.0;
                lineintarr[wsizeg-1] = 0.0;
		
		for(j=1;j<wsizeg-1;j++)
		{
		  indx = 1;
		  sumval = 0;
		  
		  for(k=1;k<wsizeg-1;k++)
		    {
		      
		      temp1 = winData[z][j][k]-winavgbgnd[z];
		      
		      if (temp1 > sigmathresh[z])
		      {
			sumval = sumval + temp1;
			colsum[indx] = colsum[indx] + temp1;
			
		      } 
                      
                      indx = indx + 1;
		      
			
		    }
		    
		    lineintarr[j] = sumval;		  
		}
		
		for (k=0;k<wsizeg;k++)
		  pixintarr[k] = colsum[k];
		
		
		
		//Centroiding 
		//pivotL = 455.0; pivotP = 451.0;
		dist = pivotL[z]+2;
		linemoment = 0;
		pixmoment = 0;
		sumi = 0.0;
		centL = 0;
		centP = 0;
		flagcenterr = 0;
		
		for (j=1;j<wsizeg-1;j++)
		{
		  if(lineintarr[j] >0)
		  {
		    linemoment = linemoment + lineintarr[j]*dist;
		    sumi = sumi + lineintarr[j];
                    //printf("\n\nWindow-%d...sumi-%f\n",testNum,sumi);
		  }
		  
		  dist = dist + 1;
		  
		}
		if (sumi >0)
		{
		  centL = (linemoment/sumi) - 0.5;
		  flagcenterr = 0;		  
		  
		}
		else
		  flagcenterr = 1;
		
		//Pixel Centroiding
		sumi = 0.0;
		dist = pivotP[z]+2;
		
		if (flagcenterr == 0)
		{
		  
		  for (k=1;k<wsizeg-1;k++)
		   {
		      if (pixintarr[k] > 0)
		       {
		          pixmoment = pixmoment + pixintarr[k]*dist;
		          sumi = sumi + pixintarr[k];
		       }
		  
		       dist = dist + 1;
		  
		    }
		   
		   if (sumi >0)
		     {
		         centP = (pixmoment/sumi) - 0.5;
		         flagcenterr = 0;
		  
		     }
		   else
		         flagcenterr = 1;
		    
		}
		
		winsumi[z] = sumi;
		wincentL[z] = centL;
		wincentP[z] = centP;
		 
          avgbgnd = avgbgnd + winavgbgnd[z];
          avgsumi = avgsumi + winsumi[z];
          avgcentL = avgcentL + wincentL[z];
          avgcentP = avgcentP + wincentP[z];
		    
	      }//z loop
	      
         
             for(z=0;z<wnum;z++)
	      {  
                  max = 0;
                 
		  for(j=0;j<wsizeg;j++)
		  {
		    for(k=0;k<wsizeg;k++)
		    {
		      fprintf(txtfile,"%04d ",winData[z][j][k]); 

                	if (winData[z][j][k] > max)
			{
			   max = winData[z][j][k];
			   xl = j;
			   xp = k;

			}
		    }
		    fprintf(txtfile,"\n");
		  }				
	
           wincpi[z] = max-winavgbgnd[z];
           avgcpi = avgcpi + wincpi[z];
           
           
           //Focus Ratio
            a1 = winData[z][xl-1][xp];
            b1 = winData[z][xl+1][xp];
            c1 = winData[z][xl][xp-1];
            d1 = winData[z][xl][xp+1];
            avgside = avgside + (a1+b1+c1+d1)/4.0-winavgbgnd[z];
            temp1 = (((a1+b1+c1+d1)/4.0) -  winavgbgnd[z]);
            fratio = (wincpi[z])/temp1;

            if(wincentL[0] > maxL)
               {
                  maxL = wincentL[0];
                  maxLIdx = testNum;
               }

            if(wincentL[0] < minL)
             {
                 minL = wincentL[0];
                 minLIdx = testNum;
               }

            if(wincentP[0] > maxP)
              {
                maxP = wincentP[0];
                maxPIdx = testNum;
               }

            if(wincentP[0] < minP)
              { 
                minP = wincentP[0];
                minPIdx = testNum;
               }

           if(winsumi[0] > maxSum)
              {
                maxSum = winsumi[0];
                
               }

            if(winsumi[0] < minSum)
              { 
                minSum = winsumi[0];
              
               }
            
          fprintf(txtfile,"\n");

          printf("\n\nMax...l-%d , p- %d, i-%d.txt\n",xl,xp,max);
              
 		  printf("\n\nWindow-%d...centl-%f , centp- %f, cpi-%d, fratio-%f, avgside-%f,sumi-%f, peaktopeakL-%f, peaktopeakP-%f, peaktopeakSumI-%f, err-%d,maxLIdx-%d,minLIdx-%d,maxPIdx-%d,minPIdx-%d\n",testNum,wincentL[z],wincentP[z],wincpi[z],fratio,temp1,winsumi[z],maxL-minL, maxP-minP, maxSum-minSum, flagcenterr,maxLIdx,minLIdx,maxPIdx,minPIdx);
		
         fprintf(txtfile, "\n\nWindow-%d...centl-%f , centp- %f, cpi-%d, fratio-%f, avgside-%f,sumi-%f, err-%d, peakl-%d, peakp-%d\n\n\n",testNum,wincentL[z],wincentP[z],wincpi[z],fratio,temp1,winsumi[z],flagcenterr,xl,xp);
		 fprintf(txtfile,"pivotL-%d pivotP-%d\n",(int)pivotL[z],(int)pivotP[z]);
fprintf(centfile, "%f %f %d\n",wincentL[z],wincentP[z],wincpi[z]);

	fprintf(fsumI, "\n\nWindow-%d...centl-%f , centp- %f, cpi-%d, fratio-%f, avgside-%f,sumi-%f, err-%d, peakl-%d, peakp-%d, avgbgnd-%f, hk1-%d, hk2-%d\n\n\n",testNum,wincentL[z],wincentP[z],wincpi[z],fratio,temp1,winsumi[z],flagcenterr,xl,xp,winavgbgnd[z],hkdata[0],hkdata[1]);
		 fprintf(fsumI,"pivotL-%d pivotP-%d\n",(int)pivotL[z],(int)pivotP[z]);
fprintf(centfile, "%f %f %d\n",wincentL[z],wincentP[z],wincpi[z]);
          
           
	      }
	      
	      

       
	    }
  
}


/******************************************************************************/
/*  Allocates memory for U8 (char) type. Returns 1 if OK, 0 if not            */
/******************************************************************************/
U8 *AllocateMemChar(unsigned long size)
{
	U8* buffer;
	buffer = (U8 *)calloc(size, sizeof(U8));
	if (buffer == NULL)
	{
		printf( "ERROR: Can't allocate memory!!!!!!!\n" );
		fprintf(logFile,"%s","ERROR: Can't allocate memory!\n");
		getchar();
		free(buffer);
		return 0;
	}
	return buffer;
}


/******************************************************************************/
/* Resets the Spacewire Brick.                                                */
/******************************************************************************/
void ResetSPWUSB()
{
  
    STAR_DEVICE_ID deviceId;

    deviceId = chooseDevice("", STAR_DEVICE_ALL);
    if (deviceId == STAR_DEVICE_UNKNOWN)
    {
        puts("ERROR: Unknown device.\n");  
      return;
    }

    if (STAR_resetDevice(deviceId) == 0)
    
      puts("ERROR: Unable to reset device.\n");
     
    else
      
	printf("Device successfully reset\n");
    
 
    
    
	
}

/******************************************************************************/
/* Resets the Camera Head Unit.                                               */
/******************************************************************************/
void ResetPDS()
{       int transmitLink=1;
  	STAR_CHANNEL_ID rxChannelId = 0U, txChannelId = 0U;
        STAR_TRANSFER_OPERATION *pTxTransferOp = NULL, *pRxTransferOp = NULL;	
  	STAR_DEVICE_ID deviceID;
	STAR_STREAM_ITEM *pTxStreamItem;
	STAR_SPACEWIRE_ADDRESS *pAddress;
	STAR_TRANSFER_STATUS txStatus;
	
	
	/* Initiate receive, send the commands through transmit link and receive through the same link */
	 
	/* Select the transmit device and channel to be used */
	if (chooseDeviceAndChannelID("transmit", &txChannelId, STAR_CHANNEL_DIRECTION_OUT,&deviceID) == 0 )
	{
	  
	  return;
	}

	 
	
	/* Get transmit link and check for validity */
	transmitLink=atoi(ptrToArg[0][2]);
	
	if ((transmitLink < 1) || (transmitLink > 2))
	{
		printf("ERROR: Incorrect link number\n");
		return;
	}
	else
	{   /* Select the path to be added to the front of packets transmitted */
		//pAddress = getTransmitPathAddress(transmitLink);
		fprintf(logFile,"%s","Path Address obtained to transmit.\n");
	  
	}
 
	 pTxBuffer = (char *)calloc(1U, 2);
	     if (pTxBuffer == NULL)
	     {
	       puts("\nERROR: Unable to allocate memory for transmit buffer");
	       fprintf(logFile,"%s","ERROR: Unable to allocate memory for transmit buffer.\n");
	       return;
	     }
	
	pTxBuffer[0] = transmitLink;
	pTxBuffer[1] = 0x49;
 
	  /* Create the packet to be transmitted */
	pTxStreamItem = STAR_createPacket(pAddress, (U8 *)pTxBuffer, sizeOFbuffer, STAR_EOP_TYPE_EOP);
    
	if (pTxStreamItem == NULL)
	 {
	    
	    fprintf(logFile,"%s","ERROR: Unable to create the packet to be transmitted.\n");
	    DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    return;
	 }
       
       
	  /* Create the transmit transfer operation for the packet */
	  pTxTransferOp = STAR_createTxOperation(&pTxStreamItem, 1);
       
	if (pTxTransferOp == NULL)
	 {
	      fprintf(logFile,"%s","ERROR: Unable to create the transfer operation to be transmitted.\n");
	      DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	      return;
	  }
	  
 
	
	    /* Submit the transmit operation */
	    if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
	    {
	      printf("\nERROR: occurred during transmit submit.");
	      fprintf(logFile,"%s","ERROR: occurred during transmit submit.\n");
	      DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    }
 
	    
	    
	     /*Wait on the transmit operation to complete*/
	    txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp, 1000);
	    
	    if(txStatus != STAR_TRANSFER_STATUS_COMPLETE)
	    {
	      printf("\nERROR: Couldn't transmit command.\n");
	      fprintf(logFile,"%s","ERROR: Couldn't transmit command.\n");
	      OutputStatus(txStatus);
	      DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	      
	      
	    }
	    else
	    {
	        printf("Reset PDS successfully...\n");
		fprintf(logFile,"%s","Reset PDS successfully.\n");
 
	    }
 
	 
	    free(pTxBuffer);
	    fprintf(logFile,"%s","Transmit buffer free.\n");
	    DisposeAll(&txChannelId, &rxChannelId, &pTxStreamItem, &pTxTransferOp, &pRxTransferOp, &pAddress);
	    
		
}

/******************************************************************************/
/* Allows user to choose the mode of operation and allows to input            */
/* parameters or opt for default values. Commands sent to the PDS are         */
/* formulated and fills the command buffer with commands.                     */
/******************************************************************************/
int getData()
{
	int mode;
	int flag=0;
	int temp, defaults;
	 
	cmnd_index=0;
	 
        
	mode=atoi(ptrToArg[0][3]);
	  defaults = atoi(ptrToArg[0][4]);

	switch(mode)
	{
		case 1:		/*Image mode*/
		        printf("\n\nMode of operation is Image Mode!\n");
			fprintf(logFile,"%s","Mode of operation is Image Mode!\n");
		         
			if(defaults)
			{
			        printf("Default values used!\n");
				flag=FFdefaultValues();
			}	
			else
			{
			  if(FFPrompt())
			    flag=FFdefaultValues();
			  else
			   flag=1;
		        
			  
			}
		break;

		case 2:		/* Threshold mode*/
		        printf("\n\nMode of operation is Threshold Mode!\n");
			fprintf(logFile,"%s","Mode of operation is Threshold Mode!\n");
			  
			if(defaults)
			{
			        printf("Default values used!\n");
			        flag=FFdefaultValues();
			}			
			else
			{
			   if(FFPrompt())
			     flag=FFdefaultValues();
			   else
			   flag=1;
	 
		 	}
				 
		break;
	
		case 3:		/*Sobel mode*/
		        printf("\n\nMode of operation is Sobel Mode!\n");
			fprintf(logFile,"%s","Mode of operation is Sobel Mode!\n");
			  
			if(defaults)
			{
			        printf("Default values used!\n");
				flag=FFdefaultValues();
			}
			else
			{
			  if(FFPrompt())
			     flag=FFdefaultValues();
			  else
			   flag=1;
                        }
				 
		break;
		
		case 4:	/*Window mode */
		        printf("\n\nMode of operation is Window Mode!\n");
			fprintf(logFile,"%s","Mode of operation is Window Mode!\n");
			 
			 
			if(defaults)
			{
			        printf("Default values used!\n");
				flag=WMdefaultValues();
			}			
			else
			{
			 if(WMPrompt())
			     flag=WMdefaultValues();
			 else
			   flag=1;
			  
			//wbytes = 10*((prompt[8]&0x03)+1)*10*((prompt[8]&0x03)+1)*10*2 + ((prompt[2]&0x08)>>3)*16; //10 represents 10 windows
		 		
			}
		break;
		
		case 5:	/*HK mode */
		        printf("\n\nMode of operation is HK Mode!\n");
			fprintf(logFile,"%s","Mode of operation is HK Mode!\n");
		         
			flag=HKdefaultValues();
						
			break;

		default: /*default mode= image mode */
			printf("Invalid entry.Image mode with default values is set.!!");
			fprintf(logFile,"%s","Invalid entry.Image mode with default values is set!");
			
			flag=FFdefaultValues(mode);	
		break;
	}
	
	 
	sizeOFbuffer=cmnd_index+1;
	return flag;
}

/******************************************************************************/
/*  Fills array prompt[] with default values for full frame mode.Returns 1    */
/*  to the flag variable.                                                     */
/******************************************************************************/
int FFdefaultValues()  //modified for spadex pds engineering model
{
	int l_mode = 0;
	int  arr[2],cmdval;
	float temp;
	pix_width=0;			//1 : 8 bit mode
	hk=1;
			 
	l_mode =  atoi(ptrToArg[0][3]);
 
	cmnd_index = 0;
	

	prompt[cmnd_index++]=0x49;		//Reset
	prompt[cmnd_index++]=0x7E;		//Mode
	/*If you are disabling HK please set hk=0*/
	if (l_mode==1)
	 {   
              cmdval = 0x89;              	//8 bit mode, HK enabled, 1MHz Image Mode
	      pix_width=1;


		//cmdval = 0x09;              	//16 bit mode, HK enabled, 1MHz Image Mode
	        //pix_width=0;
         }

	else
	{
		// cmdval = 0x98;              	//8 bit mode, HK enabled, 6MHz Sobel Mode        	
		cmdval = 0x18;                  //7-16(0)/8bit, 1,5,6-X(0), 4-freq(6Mhz(1)), 3-HK, 2-FF/Win(1), 0-Sobel/Image(1)
                pix_width=0;	
        }
       
        prompt[cmnd_index++]= cmdval;

	prompt[cmnd_index++]=0x53;		//st line address
        stline = 0;
        ConvertU16ToBytes(stline, &arr);
	prompt[cmnd_index++]=arr[1];
	prompt[cmnd_index++]=arr[0];

                
	prompt[cmnd_index++]=0x52;		//st pixel address 
	stpix = 0;
        ConvertU16ToBytes(stpix, &arr);
	prompt[cmnd_index++]=arr[1];
	prompt[cmnd_index++]=arr[0];
	
	prompt[cmnd_index++]=0x55;		//no. of lines
        flines = 1023; 
        ConvertU16ToBytes(flines, &arr);
	prompt[cmnd_index++]=arr[1];
	prompt[cmnd_index++]=arr[0];
	 
	
	prompt[cmnd_index++]=0x54;		//no.of pixels
        framelength = 1023;
        ConvertU16ToBytes(framelength, &arr);
	prompt[cmnd_index++]=arr[1];
	prompt[cmnd_index++]=arr[0];

              

 	//fbytes = ((1024*(flines+1)) + hk*8);
        
	fbytes = (framelength+1)*(flines+1) + (hk*8);
	if (pix_width == 0)
        fbytes = fbytes*2;
	

        //fbytes = 1048584;



        if ((cmdval&0x10) == 0x10)
             temp = 9.37+(framelength/6.0);   
        else
             temp = 19.37+framelength;

        


        temp = round(intgtim/temp);

        printf("%f, %f, %d",temp,intgtim,framelength);
        ConvertU16ToBytes((int)temp, &arr);

	prompt[cmnd_index++]=0x56;		//exposure time	(52ms = 0x34 for 1MHz,0x138 for 6MHz)
	prompt[cmnd_index++]=arr[1];
	prompt[cmnd_index++]=arr[0];            //(3ms)3000/175 = 17, (4.1ms)4100/175=23
	prompt[cmnd_index++]=0x76;		//SPW destination  address enabled
	prompt[cmnd_index++]=0x40;
	prompt[cmnd_index++]=0x77;		//SPW destination address
	prompt[cmnd_index++]=0x04;
	prompt[cmnd_index++]=0x7B;		//Gain
	prompt[cmnd_index++]=in_pga;//0x04;     //default = FD(gain2) 08(gain1) EA (gain4) RS 04 F5 D6
	prompt[cmnd_index++]=0x7A;       	//DR/NDR mode
	prompt[cmnd_index++]=0x00;        	//DR-0,NDR-1
	prompt[cmnd_index++]=0x7C;		//Black ref level
	prompt[cmnd_index++]=0x39;
	prompt[cmnd_index++]=0x7D;		//Col amp offset, 4F PDS, 4E for RS //Col amp offset correction RS 4e 54 71, PDS 4f 62 71
	prompt[cmnd_index++]=in_colamp;//0x4E;
	prompt[cmnd_index++]=0x51;		//Threshold value
	
	if (l_mode==1)
	{
		prompt[cmnd_index++]=0x01;		//default threshold value
		prompt[cmnd_index++]=0xF4;
	}
	else
	{
	   
		prompt[cmnd_index++]=0x00;//0x00;	//Sobel threshold value=100
		prompt[cmnd_index++]=0x64;//0x64;
		
	}
	prompt[cmnd_index++]=0x75;		//start frame
	prompt[cmnd_index++]=0x01;
	sizeOFbuffer=cmnd_index+1;
	
	return 1;	
}

int  FFPrompt()
{
  
		FILE *paramFile;
	        int temp=0;
		char str[200];
	        int winConfig=0;
		int mode=1;
		
		cmnd_index=0;
		char paramFilePathFF[50]="params/ff_params.txt";
		paramFile = fopen(paramFilePathFF,"r"); 
		
		if (paramFile == NULL)
		{
		  printf("Error: Trouble opening param file. Default values taken.\n");
		  return 1;
		}
		mode=atoi(ptrToArg[0][3]);
		winConfig=winConfig|(mode-1); //
		
		prompt[cmnd_index++]=0x49;		//Reset
		prompt[cmnd_index++]=0x7E;		//Mode
		 	
		       
		if(fscanf(paramFile,"%d",&temp)==0)          //Housekeeping 
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		if(temp!=0 && temp!=1)
		{
			fprintf(logFile,"%s","Invalid entry!!!.Health data enabled at end of frame.\n");
			temp=1;     
		}
		hk=temp;
	
		temp*=8;				//left shifting by 3 times
		winConfig=winConfig|temp;
	
		//Redundant Text
		
		if(fgets(str,200,paramFile)==0)          //Housekeeping Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		if(fscanf(paramFile,"%d",&temp)==0)          //Pixel Read Rate 1,2,3,6Mhz
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		if(temp!=0 && temp!=1 && temp!=2 && temp!=3)
		{
			fprintf(logFile,"%s","Invalid entry!!!.6Mhz Mode selected.\n");
			temp=3;   // 3 - 6Mhz  
		}
		
		 
		temp*=16;				//left shifting by 4 times
		winConfig=winConfig|temp;
		
		if(fgets(str,200,paramFile)==0)          //Pixel Read Rate 1,2,3,6Mhz Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		
		if(fscanf(paramFile,"%d",&pix_width)==0)         //8 bit(1) or 16 bit(0)
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		 
		if(pix_width!=0 && pix_width!=1)
		{
			fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 8 bit mode.\n");
			pix_width=1;     
		}
		
		temp=pix_width;
		temp*=128;				//left shifting by 8 times
		prompt[cmnd_index++]=winConfig|temp; 
		
		if(fgets(str,200,paramFile)==0)          //8 bit(1) or 16 bit(0) Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		  

		prompt[cmnd_index++]=0x53; 		
	        
	        if(fscanf(paramFile,"%d",&temp)==0)         
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}                                             //start line address
		 
		if(temp<0 || temp>1023)
		{
			fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 0.\n");
			temp=0;     
		}
		convert(temp);
		
		if(fgets(str,200,paramFile)==0)          //start line address Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		  

		prompt[cmnd_index++]=0x52;		//start pixel address
		prompt[cmnd_index++]=00;
		prompt[cmnd_index++]=00; 
		prompt[cmnd_index++]=0x55;	 	
	        
	        if(fscanf(paramFile,"%d",&temp)==0)           //no. of lines
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}       
		 
		if(temp<0 || temp>1023)
		{
		fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 1023.\n");
		temp=1023;     
		}
		
		if ((prompt[4]*256+prompt[5] +temp) > 1023)
		{
		  fprintf(logFile,"%s","Start line + No. of lines >1023 .Default values taken.\n");
		  return 1;
		}
		convert(temp);

		 
		flines = temp +1;
		
		if(pix_width == 1)
		  fbytes = (1024*flines) + hk*8;
		else
		  fbytes = (1024*flines)*2 + hk*16;
		
	 	fprintf(logFile,"%s %d %s","fbytes-",fbytes, " \n");
			
		if(fgets(str,200,paramFile)==0)          //no. of lines Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		
		prompt[cmnd_index++]=0x54;		//no.of pixels
		prompt[cmnd_index++]=0x03;
		prompt[cmnd_index++]=0xFF;

		prompt[cmnd_index++]=0x56;		//exposure time
		
		if(fscanf(paramFile,"%d",&temp)==0)         
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		} 
		 
		if(temp<0 || temp>1023)
		{
			fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 15.\n");
			temp=15;     
		}
		
		//Should be temp=temp*yMHz*(2^10/10^3)
		if( ((winConfig&0x30)>>4) == 0)
		  temp=temp*2; 
		else if( ((winConfig&0x30)>>4) == 1)
		  temp=temp*3;
		else if( ((winConfig&0x30)>>4) == 2)
		  temp=temp;
		else if( ((winConfig&0x30)>>4) == 3)
		  temp=temp*6;
		  
		convert(temp);
		
		if(fgets(str,200,paramFile)==0)          //exposure time Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		 
		prompt[cmnd_index++]=0x76;		//SPW destination  address enabled
		prompt[cmnd_index++]=0x40;
		prompt[cmnd_index++]=0x77;		//SPW destination address
		prompt[cmnd_index++]=0x04;
		prompt[cmnd_index++]=0x7B;		//Gain
		prompt[cmnd_index++]=in_pga;//0xF4;
 
		prompt[cmnd_index++]=0x7A;        	//DR/NDR mode
		prompt[cmnd_index++]=0x00;		//DR-0,NDR-1
		prompt[cmnd_index++]=0x7C;		//Black Ref level
		prompt[cmnd_index++]=0x2D;
 

		prompt[cmnd_index++]=0x7D;		//Column amplifier offset  
		prompt[cmnd_index++]=in_colamp;//0x4B;
 

		prompt[cmnd_index++]=0x51;		//Threshold value
		if(mode!=1)
		{
			if(fscanf(paramFile,"%d",&temp)==0)         
			{
			  printf("ERROR: File inputs insufficient. Default values taken.");
			  return 1;
		    
			}  
			 
			if(temp<0 || temp>4096)
			{
				fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 100.\n");
				temp=100;     
			}
			convert(temp);
		}
		else
		{	
			prompt[cmnd_index++]=00;
			prompt[cmnd_index++]=00;
		}
		
		if(fgets(str,200,paramFile)==0)          //Threshold value Text
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		 
		prompt[cmnd_index++]=0x75;		//start frame
		prompt[cmnd_index++]=0x01;
		
		fclose(paramFile);
		sizeOFbuffer=cmnd_index+1;
		
		return 0;
	
  
}

/******************************************************************************/
/*  Fills array prompt[] with default values for window.Returns 1             */
/*  to the flag variable                                                      */
/******************************************************************************/
int WMdefaultValues()  
{
	int i,arr[2],cmdval;
	float temp;
        char str[50];
        FILE *fin;

// !!Beware when change in temparr is done, correspondingly change wnum, wsize and 0x79 command
// eg.- wnum = 1, wsize = 20, 0x79 will have 0x12
// Sort by user input to temparr
// Full Frame image in GIMP is in (pixel, line)
// temparr is in (line, pixel) order

   fin = fopen("win_cordinates.txt","r");

      
   float temparr[8][2] = {{467, 504},//465,505
{492,501},
{494, 513.111},
{503, 486},
{485.8, 524.6},
{488.333, 497.667},
{544.5, 510.5},
{610.8, 495.4}};
    i = 0;
     while(!feof(fin))
    {
       fgets(str,50,fin);
       sscanf(str,"%f%f",&temparr[i][0],&temparr[i][1]);
       i++;
     }
 fclose(fin);
	
	cmnd_index=0;
	hk=1;
 
	//printf("wbytes = %d\n",wbytes);
	//wbytes = wnum*wsizeg*wsizeg+hk*8;
	prompt[cmnd_index++]=0x49;	        //Reset 
 
        
	prompt[cmnd_index++]=0x77;		//SPW address
	prompt[cmnd_index++]=0x04;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;

	
	prompt[cmnd_index++]=0x7A;		//CDS/NDR
	prompt[cmnd_index++]=0;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;

	prompt[cmnd_index++]=0x7d;		//Col amp offset  RS 4e 54 71, PDS 4f 62 71
	prompt[cmnd_index++]=in_colamp;//0x4E;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;		


	prompt[cmnd_index++]=0x7B;		//Gain + Offset
	prompt[cmnd_index++]=in_pga;//0x04;             //0x04; 08 for PDS // RS F5, D6 , PDS FD and EA
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;

	prompt[cmnd_index++]=0x78;		//pd gain
	prompt[cmnd_index++]=0x02;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;		


	prompt[cmnd_index++]=0x76;		//SPW enable
	prompt[cmnd_index++]=0xFF;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;

	//If you change 0x79 0r 0x7e, change variables wsizeg, wnum,wbytes and hk accordingly
	prompt[cmnd_index++]=0x79;		//Win_size
	
        wnum=i-1; wsizeg=winsize;
        cmdval = ((wsizeg/10) & 0x000F);        
        cmdval = ((wnum & 0x000F)<<4) | cmdval; 
        prompt[cmnd_index++]=cmdval;//0x11;	
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;		//10 Windows, 10x10 
        wbytes = wnum*wsizeg*wsizeg*2+hk*16;
        
        cmdval = 0x7c;
	prompt[cmnd_index++]=0x7E;		//Mode
	prompt[cmnd_index++]=cmdval;	
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;	       //7-16(0)/8bit, 1,5,6-X(0), 4-freq(6Mhz(1)), 3-HK, 2-FF/Win(1), 0-Sobel/Image(1)


        if ((cmdval&0x10) == 0x10)
             temp = 70.0;   
        else
             temp = 350.0;  
         
        temp = round(intgtim/temp);     
 
        ConvertU16ToBytes((int)temp, &arr);
        prompt[cmnd_index++]=0x56;		//Exposure Time [x(in us)/70]
	prompt[cmnd_index++]=arr[1];               
	prompt[cmnd_index++]=arr[0];             //4000microsec(0x47)
        prompt[cmnd_index++]=0x0;

	prompt[cmnd_index++]=0x7F;		//CDS level data
	prompt[cmnd_index++]=0x00;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;			
	prompt[cmnd_index++]=0x7C;		//Black Level
	prompt[cmnd_index++]=0x39;		
 
	//If (X,Y) is center then in commands put X -(wsizeg/2) or Y -(wsizeg/2) (534, 529) (pix,line)
       
       //RS@10m [21 2 3 5 11 6 1 4]
 


       pivotL[0] = temparr[0][0]- wsizeg/2; 
       pivotL[1] = temparr[1][0]- wsizeg/2;
       pivotL[2] = temparr[2][0]- wsizeg/2;
       pivotL[3] = temparr[3][0]- wsizeg/2;
       pivotL[4] = temparr[4][0]- wsizeg/2;
       pivotL[5] = temparr[5][0]- wsizeg/2;
       pivotL[6] = temparr[6][0]- wsizeg/2;
       pivotL[7] = temparr[7][0]- wsizeg/2;

       pivotP[0] = temparr[0][1]- wsizeg/2;
       pivotP[1] = temparr[1][1]- wsizeg/2;
       pivotP[2] = temparr[2][1]- wsizeg/2;
       pivotP[3] = temparr[3][1]- wsizeg/2;
       pivotP[4] = temparr[4][1]- wsizeg/2;
       pivotP[5] = temparr[5][1]- wsizeg/2;
       pivotP[6] = temparr[6][1]- wsizeg/2;
       pivotP[7] = temparr[7][1]- wsizeg/2;

    

      for(i = 0; i<wnum ; i++)
        {
         
          printf("*****%f %f\n",pivotL[i],pivotP[i]);
          //printf("*****%f %f\n",temparr[i][0],temparr[i][1]);

        
           prompt[cmnd_index++]=0x57;   
           ConvertU16ToBytes(pivotP[i], &arr);	 
	   prompt[cmnd_index++]=arr[1];               
	   prompt[cmnd_index++]=arr[0];
           prompt[cmnd_index++]=0x0;
	   prompt[cmnd_index++]=0x58;   
           ConvertU16ToBytes(pivotL[i], &arr);	 
	   prompt[cmnd_index++]=arr[1];               
	   prompt[cmnd_index++]=arr[0]; 
           prompt[cmnd_index++]=0x0;

         }
      

	prompt[cmnd_index++]=0x75;		//start frame
	prompt[cmnd_index++]=00;
        prompt[cmnd_index++]=0x0;
        prompt[cmnd_index++]=0x0;
	sizeOFbuffer=cmnd_index+1;
	return 1;
}

/******************************************************************************/
/*  Fills array prompt[] with user inputs for window mode.                    */
/*  Returns 1 to the flag variable                                            */
/******************************************************************************/
int WMPrompt()
{
	int i,level,intgStep,slot,rsl_start;
	int wsize;
	int xcor[16],ycor[16],IT,address,fread;
	FILE *paramFile;
	char paramFilePathWM[50]="params/wm_params.txt";
	char str[200];
	
	paramFile = fopen(paramFilePathWM,"r"); 
	
	if (paramFile == NULL)
	{
	  printf("Error: Trouble opening param file. Default values taken.\n");
	  return 1;
	}
	
	if(fscanf(paramFile,"%d",&wsize)==0)          //Window Size 
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //Window Size Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	
	if(wsize<0 || wsize>6)
	{
	   fprintf(logFile,"%s","Invalid entry. Default size chosen as 10x10.");
	   wsize=1;//10X10
	}			

	if(fscanf(paramFile,"%d",&wnum)==0)          //No. of Windows
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //No. of windows Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	wsize++; //for switch-case
	switch(wsize)
	{
		case 1: if(wnum<0 || wnum>16)  //why 16 is taken as max windows?
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 4.");
			wnum=4;
			}
			wsizeg=5;
		break;
		case 2: if(wnum<0 || wnum>16)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 4.");
			wnum=4;
			}
			wsizeg=10;
		break;
		case 3: if(wnum<0 || wnum>16)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 4.");
			wnum=4;
			}
			wsizeg=20;
		break;	
		case 4: if(wnum<0 || wnum>8)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 4.");
			wnum=4;
			}
			wsizeg=30;
		break;
		case 5: if(wnum<0 || wnum>5)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 4.");
			wnum=4;
			}
			wsizeg=40;
		break;	
		case 6: if(wnum<0 || wnum>3)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 1.");
			wnum=1;
			}
			wsizeg=50;
		break;
		case 7: if(wnum<0 || wnum>2)
			{
			fprintf(logFile,"%s","Invalid entry!!!Default value taken as 1.");
			wnum=1;
			}
			wsizeg=60;
		break;
	}

	if(fscanf(paramFile,"%d",&intgStep)==0)          // Integration Step Count
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //Integration Step Count Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	if(intgStep<0 || intgStep>1)
	{
		fprintf(logFile,"%s","Invalid entry. Default rate chosen as 10us.");
		intgStep=0;
	}	
	
	cmnd_index=0;
	prompt[cmnd_index++]=0x49;		//Reset
	prompt[cmnd_index++]=0x76;		//SPW enable
	prompt[cmnd_index++]=0xFF;
	prompt[cmnd_index++]=0x77;		//SPW address
	prompt[cmnd_index++]=0x04;
	prompt[cmnd_index++]=0x7A;		//CDS/NDR
	prompt[cmnd_index++]=0x02;
	prompt[cmnd_index++]=0x7E;		//Mode
	
	if(fscanf(paramFile,"%d",&hk)==0)          
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(hk!=0 && hk!=1)
	{
	  fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 1(HK Enabled).\n");
	  hk=1;     
	}
	 
	 
	 if(fgets(str,200,paramFile)==0)             //HK Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	

		
	if(fscanf(paramFile,"%d",&pix_width)==0)          //8 bit or 16 bit
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //8 bit or 16 bit Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	if(pix_width!=0 && pix_width!=1)
	{
	  fprintf(logFile,"%s","Invalid entry!!!.default value taken to be 16 bit mode.\n");
	  pix_width=0;     
	}
	 
	if(pix_width==0)
	  wbytes = wsizeg*wsizeg*wnum*2 + hk*16;	
	else
	  wbytes = wsizeg*wsizeg*wnum*1 + hk*8;	
	
	 
	
	prompt[cmnd_index++]=4|hk*8|16|((intgStep)*64)|pix_width*128;	//    16bit|IntgStep|3MHz|HK|WinMode
	
	prompt[cmnd_index++]=0x79; //Num of Windows, Window size
	prompt[cmnd_index++]=(wsize-1)|wnum*16;
	
	prompt[cmnd_index++]=0x7B;		//Gain +Offset
	prompt[cmnd_index++]=in_pga;//0xF8;
	
	prompt[cmnd_index++]=0x7F;		//CDS level data
	 
	if(fscanf(paramFile,"%d",&level)==0)          //CDS level
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //CDS level Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	if(level<0 || level>2)
	{
		fprintf(logFile,"%s","Invalid entry. You will receive CDS data.");
		level=0;
	}	
	prompt[cmnd_index++]= level;
	prompt[cmnd_index++]=0x7C;	//Black Level
	prompt[cmnd_index++]=0x2D;
	prompt[cmnd_index++]=0x7D;	//Col amp offset  
	prompt[cmnd_index++]=in_colamp;//0x4B;

	if(fscanf(paramFile,"%d",&IT)==0)       //Integration Time    
	{
	  printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
	  
	}
	
	if(fgets(str,200,paramFile)==0)             //Intg Time Text
	{
          printf("ERROR: File inputs insufficient. Default values taken.");
	  return 1;
		  
	}
	
	prompt[cmnd_index++]=0x56;	
	intgStep==0? convert(IT/10):convert(IT);
	 
	//xcor is pixel, ycor is line
	for(i=0;i<wnum;i++)
	{
		xcor[i]=0;
		ycor[i]=0;
	}
	
	for(i=0;i<wnum;i++)
	{
		//printf("\nX[%d]=",i+1);
		
		if(fscanf(paramFile,"%d",&xcor[i])==0)          
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		 
		if(xcor[i]>1023)
			fprintf(logFile,"%s","Invalid entry. Default set to 0.");
		
		if(i==0)
		{
		  if(fgets(str,200,paramFile)==0)             //Window Coord Text
		    {
		      printf("ERROR: File inputs insufficient. Default values taken.");
		      return 1;
		  
		    }
		}
		 
		
		if(fscanf(paramFile,"%d",&ycor[i])==0)          
		{
		  printf("ERROR: File inputs insufficient. Default values taken.");
		  return 1;
		  
		}
		  
		if(ycor[i]>1023)
			fprintf(logFile,"%s","Invalid entry. Default set to 0.");
		
		 
	}
	

	for(i=0;i<wnum;i++)		//x,y co-ordinates
	{
		prompt[cmnd_index++]=0x57;   
		convert(xcor[i]);
		prompt[cmnd_index++]=0x58;
		convert(ycor[i]);
	}

	
	prompt[cmnd_index++]=0x75;		//start frame
	prompt[cmnd_index++]=01;
	
	fclose(paramFile);
	sizeOFbuffer=cmnd_index +1;
	return 0;
}

int HKdefaultValues()
{
	prompt[cmnd_index++] = 0x49;
        //prompt[cmnd_index++] = 0x49;
	//prompt[cmnd_index++] = 0x7E;
	//prompt[cmnd_index++] = 0x10;
        //prompt[cmnd_index++] = 0x78;
	//prompt[cmnd_index++] = 0x01;
	prompt[cmnd_index++] = 0x76;
	prompt[cmnd_index++] = 0xFF;
	prompt[cmnd_index++] = 0x77;
	prompt[cmnd_index++] = 0x04;
	prompt[cmnd_index++] = 0x4A;
	sizeOFbuffer=cmnd_index+1;

	return 1;
}

/******************************************************************************/
/* Converts 16-bit val to two 8-bit digits and stores to propmt               */
/******************************************************************************/
void convert(int val)
{
	if(val>255)
	{
		prompt[cmnd_index++]=(val/256)&0xFF;
		prompt[cmnd_index++]=val&0x00FF;
	}
	else
	{
		prompt[cmnd_index++]=0;
		prompt[cmnd_index++]=val;
	}
	
 
} 

void terminate(int a)
{
   signal(SIGINT, terminate);
   
   printf("Smooth Termination...\n");
 
  if(txtfile != NULL)
    fclose(txtfile);

  if(testfile != NULL)
    fclose(testfile);
 
  
  if(logFile != NULL)
    fclose(logFile);
   
   return;
}

void ConvertU16ToBytes(int num, int arr[2])
 {
    
   BYTE_TO_WORD U16Num;
    
   U16Num.num = num;
   
   arr[1] = U16Num.byte.arg[1]; 
   arr[0] = U16Num.byte.arg[0]; 
 
         
 }
