#include <windows.h>
#include "SLABCP2112.h"
#include "adapter.h"
#include <stdio.h>


 int checkTransferResultCode( HID_SMBUS_STATUS status, char* reportString);
 void checkTransactionStatus(HID_SMBUS_S0 errCode, char* text);

 void initDlls (adapterStruct* hdl) {
      hdl->HidSmbus_GetNumDevicesX = (pfHidSmbus_GetNumDevices) GetProcAddress (hdl->libraryHandle, "HidSmbus_GetNumDevices");
      hdl->HidSmbus_OpenX = (pfHidSmbus_Open)GetProcAddress (hdl->libraryHandle, "HidSmbus_Open");
      hdl->HidSmbus_CloseX = (pfHidSmbus_Close)GetProcAddress (hdl->libraryHandle, "HidSmbus_Close");
      hdl->HidSmbus_WriteRequestX = (pfHidSmbus_WriteRequest) GetProcAddress (hdl->libraryHandle, "HidSmbus_WriteRequest");
      hdl->HidSmbus_ReadRequestX = ( pfHidSmbus_ReadRequest) GetProcAddress (hdl->libraryHandle, "HidSmbus_ReadRequest");
      hdl->HidSmbus_GetReadResponseX = (pfHidSmbus_GetReadResponse) GetProcAddress(hdl->libraryHandle, "HidSmbus_GetReadResponse");
      hdl->HidSmbus_SetTimeoutsX = (pfHidSmbus_SetTimeouts) GetProcAddress (hdl->libraryHandle, "HidSmbus_SetTimeouts");
      hdl->HidSmbus_SetSmbusConfigX = (pfHidSmbus_SetSmbusConfig) GetProcAddress ( hdl->libraryHandle, "HidSmbus_SetSmbusConfig");
      hdl->HidSmbus_GetStringX = (pfHidSmbus_GetString) GetProcAddress ( hdl->libraryHandle, "HidSmbus_GetString");
      hdl->HidSmbus_TransferStatusRequestX = (pfHidSmbus_TransferStatusRequest) GetProcAddress(hdl->libraryHandle, "HidSmbus_TransferStatusRequest");
      hdl->HidSmbus_GetTransferStatusResponseX = (pfHidSmbus_GetTransferStatusResponse) GetProcAddress(hdl->libraryHandle,"HidSmbus_GetTransferStatusResponse");
      hdl->HidSmbus_ForceReadResponseX = (pfHidSmbus_ForceReadResponse) GetProcAddress(hdl->libraryHandle,"HidSmbus_ForceReadResponse");
      return;
 }

  //write a meaning of HID_SMBUS_STATUS  inside a given string (pointer)
  int checkTransferResultCode( HID_SMBUS_STATUS status, char* reportString) {
     switch(status) {
  case HID_SMBUS_SUCCESS:
      strcpy(reportString,"HID_SMBUS_SUCCESS \n");
      return 0;
    break;
      case HID_SMBUS_DEVICE_NOT_FOUND:
      strcpy(reportString,"HID_SMBUS_DEVICE_NOT_FOUND \n");
    break;
          case HID_SMBUS_INVALID_HANDLE:
      strcpy(reportString,"HID_SMBUS_INVALID_HANDLE \n");
    break;
          case HID_SMBUS_INVALID_DEVICE_OBJECT:
      strcpy(reportString,"HID_SMBUS_INVALID_DEVICE_OBJECT \n");
    break;
          case HID_SMBUS_INVALID_PARAMETER:
      strcpy(reportString,"HID_SMBUS_INVALID_PARAMETER \n");
    break;
          case HID_SMBUS_INVALID_REQUEST_LENGTH:
      strcpy(reportString,"HID_SMBUS_INVALID_REQUEST_LENGTH \n");
    break;
          case HID_SMBUS_DEVICE_ALREADY_OPENED:
      strcpy(reportString,"HID_SMBUS_DEVICE_ALREADY_OPENED \n");
    break;
        case HID_SMBUS_READ_ERROR:
      strcpy(reportString,"HID_SMBUS_READ_ERROR \n");
    break;
          case HID_SMBUS_WRITE_ERROR:
      strcpy(reportString,"HID_SMBUS_WRITE_ERROR \n");
    break;
          case HID_SMBUS_READ_TIMED_OUT	:
      strcpy(reportString,"HID_SMBUS_READ_TIMED_OUT \n");
    break;
          case HID_SMBUS_DEVICE_IO_FAILED:
      strcpy(reportString,"HID_SMBUS_DEVICE_IO_FAILED \n");
    break;
          case HID_SMBUS_DEVICE_ACCESS_ERROR:
      strcpy(reportString,"HID_SMBUS_DEVICE_ACCESS_ERROR \n");
    break;
          case HID_SMBUS_DEVICE_NOT_SUPPORTED:
      strcpy(reportString,"HID_SMBUS_DEVICE_NOT_SUPPORTED \n");
    break;
          case  HID_SMBUS_UNKNOWN_ERROR	:
      strcpy(reportString,"HID_SMBUS_UNKNOWN_ERROR \n");
    break;
          default:
             strcpy(reportString,"  ? error ? \n");

    }
    return 1;

  }

  void checkTransactionStatus(HID_SMBUS_S0 errCode, char* text) {
     switch (errCode) {
   case HID_SMBUS_S0_IDLE :
     strcpy(text, "HID_SMBUS_S0_IDLE \n");
     break;
   case HID_SMBUS_S0_BUSY:
       strcpy(text, "HID_SMBUS_S0_BUSY \n");
    break;
   case HID_SMBUS_S0_COMPLETE:
       strcpy(text, "HID_SMBUS_S0_COMPLETE \n");
    break;
   case HID_SMBUS_S0_ERROR:
       strcpy(text, "HID_SMBUS_S0_ERROR \n");
    break;
   default:
       strcpy(text, "Unknown error \n");
     }

  }
 /*
   I N F O R M A T I O N
   STEP 1. Call this function first of all. It loads dynamic library and
   assigns procedures to function pointers inside adapterStruct.
   You should checking firstly DLL dependencies (maybe there absent any MFC library)
 */
  int loadDynamicLibrary(adapterStruct *hAdapter, char* report) {
       //1) load a libnrary
         hAdapter->libraryHandle = LoadLibrary("SLABHIDtoSMBus.dll");
         //2)checking ststus
         if (hAdapter == NULL){
            sprintf(report,"Error loading of dll library! \n");
            return 1;
         }
         initDlls(hAdapter);
         sprintf(report,"Library loaded successsfully! \n");
         return 0;
  }

  /*
  I N F O R M A T I O N
  STEP 1.1 optional.How many CP2212 devices are there in PC?
  When not found - free library and returns -1
  */

  int amountOfCP2112 (adapterStruct *hAdapter) {
         HID_SMBUS_STATUS cpStatus;
             //3)amount of devices inside cp2112
         cpStatus = hAdapter->HidSmbus_GetNumDevicesX(&hAdapter->numDevices, CP2112_VENDOR_ID, CP2112_PRODUCT_ID);

         if ( cpStatus != SILABS_STATUS_SUCCESS ) {
            return -1;
        }
        //4)When found - show result
       return hAdapter->numDevices;
  }

  /*I N F O R M A T I O N:

    STEP 2. Initializes I2C adapter.This function initializes CP2112 uses the DLL library
    Call it before any data transactions
  */

    int initAdapter (   adapterStruct *hAdapter,  //pointer to structure-handle
                        DWORD bitsPerSec,         ///bits per Sec
                        DWORD timeouts,           //timeouts in mSec
                        DWORD numberOfDeviceToOpen, //number of device to open.Starts from 0
                        char* reportString ) {        //a string for reporting about success/fail in text format
    ///in this function disable AutoReadResponse
        HID_SMBUS_STATUS cpStatus;  //status of an operatoin
     //1) Has the library been loaded?

    if (hAdapter->libraryHandle==NULL) {
          strcpy(reportString, "Error loading SLABHIDtoSMBus.dll\n");
          return 1;
    }

    //2)Try to open the device with number in the numberOfDeviceToOpen variable.
    cpStatus = hAdapter->HidSmbus_OpenX(&hAdapter->device, numberOfDeviceToOpen, CP2112_VENDOR_ID, CP2112_PRODUCT_ID);

    if ( cpStatus != HID_SMBUS_SUCCESS) {
        strcpy(reportString,"Failed to open device.\n");

        return 1;
    }

    //3) When the IC has opened:
     strcpy(reportString,"Device opened.\n");

    // 4)Configure SMBus:
    //
    cpStatus = hAdapter->HidSmbus_SetSmbusConfigX(hAdapter->device,
                                                  bitsPerSec,  ///bitrate
                                                   0x20,    //OWN slave address of the CP2112 chip.Only for ACK.So, don`t care
                                                   FALSE,    //AutoReadResponse , there are bugs when TRUE
                                                   timeouts,    //Write timeout
                                                   timeouts,    //read timeout
                                                   FALSE,     //sclLowTimeout, timeout enable when SCL holds low > 25mS
                                                   1);        //transferRetries

     if (cpStatus != HID_SMBUS_SUCCESS) {
       strcpy(reportString, "Error of clock configuration! \n");

       return 1;
    }
    sprintf(reportString,"device initialized successfully! \n" );
    return 0;

  }
  //==============Don`t use this function!!!.I investigated the bug of the chip when autoReadResponse enable===========================

  unsigned int testFunction ( adapterStruct * hAdapter,
                          unsigned char * buffer,
                           unsigned char addrWhom,
                                   char *textReport,
                                    WORD amountOfBytes){
    HID_SMBUS_STATUS   operationResult;
     HID_SMBUS_S0       statusOfRequest;
     HID_SMBUS_S1       detailedStatus;
     BYTE               bytesRead;
     WORD               numRetries;
     unsigned char       bReadResult;

     operationResult =  hAdapter->HidSmbus_ReadRequestX(hAdapter->device,(addrWhom << 1),amountOfBytes);
      if (operationResult != HID_SMBUS_SUCCESS) {
            sprintf(textReport, "1)ReadRequest  .  \n");
            checkTransferResultCode(operationResult, (textReport + 16) );
            return operationResult;
      }
      operationResult = hAdapter->HidSmbus_GetReadResponseX(hAdapter->device,&statusOfRequest,buffer,64,&bytesRead);
           if (operationResult != HID_SMBUS_SUCCESS) {
            sprintf(textReport, "1)ReadRequest  .  \n");
            checkTransferResultCode(operationResult, (textReport + 16) );
            return operationResult;
      }
      return 0;


 }

     /* STEP 3
     I N F O R M A T I O N: This function for reading block of data from slave device
     PARAMETERS:-----------------------
      @ hAdapter - pointer to instance of the adapterStruct structure;
      @ buffer - a buffer to the data, that a master reads from a slave;
      @ addressFrom - address of slave device;
      @ textReport - write information as a text string about the result of reading of a block of data
      @ amountOfBytes - amount of data in bytes, must be < 61 bytes
    RETURNED VALUE:----------------------
      When success - returned zero, otherwise - an error code.
     */
  unsigned int readData (adapterStruct * adapterHandle, unsigned char * buffer, unsigned char addressFrom, char* textReport, unsigned short amountOfBytes) {

     HID_SMBUS_STATUS   operationResult;
     HID_SMBUS_S0       statusOfRequest;
     HID_SMBUS_S1       detailedStatus;
     WORD               bytesRead;
     WORD               numRetries;
     unsigned char       bReadResult;


     //A) send read request
    operationResult  = adapterHandle->HidSmbus_ReadRequestX(adapterHandle->device, (addressFrom << 1) , amountOfBytes);
    if (operationResult != HID_SMBUS_SUCCESS) {
            sprintf(textReport, "1)ReadRequest  .  \n");
            checkTransferResultCode(operationResult, (textReport + 16) );
            return operationResult;
    }
    //B) REad transfer status
    operationResult  = adapterHandle->HidSmbus_TransferStatusRequestX(adapterHandle->device);
    if (operationResult != HID_SMBUS_SUCCESS) {
            sprintf(textReport, "2)TransferStatusRequest .\n");
             checkTransferResultCode(operationResult, (textReport + 23) );
            return operationResult;
    }
    //C) transfer status response
     statusOfRequest  = HID_SMBUS_S0_BUSY;
     while (statusOfRequest == HID_SMBUS_S0_BUSY){
            //wait until transmision in progress
             operationResult =  adapterHandle->HidSmbus_GetTransferStatusResponseX(adapterHandle->device, &statusOfRequest, &detailedStatus, &numRetries, &bytesRead);
             if (operationResult == HID_SMBUS_READ_TIMED_OUT ){
                break;
             }
            Sleep(1);
     }
        //check status

     if ((statusOfRequest == HID_SMBUS_S0_COMPLETE )|| (statusOfRequest == HID_SMBUS_S0_IDLE)|| ( operationResult == HID_SMBUS_READ_TIMED_OUT)) {
         puts("3)GetTransferStatusResponse completed successfully! \n");
     } else  {
         sprintf(textReport,"3)GetTransferStatusResponse   \n");
         checkTransactionStatus(statusOfRequest, (textReport + 28) );
         return statusOfRequest;
     }
       //optional

    //D)  Force read
      operationResult = adapterHandle->HidSmbus_ForceReadResponseX(adapterHandle->device, amountOfBytes);
            if (operationResult != HID_SMBUS_SUCCESS) {
                sprintf(textReport, "4)ForceReadResponse  \n");
                 checkTransferResultCode(operationResult, (textReport + 20) );
                 return operationResult;
            }
     //E) get read response
         statusOfRequest  = HID_SMBUS_S0_BUSY;
     while (statusOfRequest == HID_SMBUS_S0_BUSY) {
         operationResult = adapterHandle->HidSmbus_GetReadResponseX(adapterHandle->device,&statusOfRequest,buffer,64,&bReadResult);
         if (operationResult == HID_SMBUS_READ_TIMED_OUT ) {
            break;
         }
         Sleep(1);
     }
          if ((statusOfRequest == HID_SMBUS_S0_COMPLETE )|| (statusOfRequest == HID_SMBUS_S0_IDLE)|| ( operationResult == HID_SMBUS_READ_TIMED_OUT)) {
       puts("5)GetReadResponse completed successfully! \n");
     } else {
         sprintf(textReport,"5)GetReadResponse  \n");
         checkTransactionStatus(statusOfRequest, (textReport + 18) );
         return statusOfRequest;

     }

     sprintf( textReport, "5.1) Read bytes: %02X   \n", bytesRead);
     checkTransactionStatus(statusOfRequest, (textReport + 21) );

     return statusOfRequest;

    /// AUTO RESPONSE DISABLED TEST END----------------- }
  }


  /*
    INFORMATION:
    This function closes a device and the DLL library
  */
  void deInitAdapterCloseLib(adapterStruct * hAdapter) {
      // Close the device
    hAdapter->HidSmbus_CloseX(hAdapter->device);
    FreeLibrary(hAdapter->libraryHandle);
  }
      //S  T  E  P   4. INFORMATION.
     //This function write block of data into slave device
     //VERY IMPORTANT NOTE!! The amount of data MUST BE 1-61 bytes (because of endpoint type is "HID interrupts"
    // with maximum packet size = 64 BYTES.
     /*
     PARAMETERS:-----------------------
      @ hAdapter - pointer to instance of the adapterStruct structure;
      @ buffer - a buffer to the data, that a master sends to a slave;
      @ addressFrom - address of slave device;
      @ textReport - write information about  result of writing of a block of data in text format
      @ amountOfBytes - amount of data in bytes
    RETURNED VALUE:----------------------
      When success - returned zero, otherwise - an error code.
      */

  unsigned int  writeData (adapterStruct * hAdapter, unsigned char * buffer, unsigned char addrWhom, char *textReport, WORD amountOfBytes ){
     HID_SMBUS_STATUS   operationResult;
     HID_SMBUS_S0       statusOfRequest;
     HID_SMBUS_S1       detailedStatus;
     WORD               bytesRead;
     WORD               numRetries;
     unsigned char      bReadResult;

     //A) write request----------------------
     operationResult = hAdapter->HidSmbus_WriteRequestX(hAdapter->device,
                                                        (addrWhom << 1),
                                                        buffer,
                                                        amountOfBytes);

      if (operationResult != HID_SMBUS_SUCCESS) {
              sprintf(textReport, "2)TransferStatusRequest .\n");
             checkTransferResultCode(operationResult, (textReport + 23) );
            return operationResult;
         }
    //B) transfer stats request---
    operationResult = hAdapter->HidSmbus_TransferStatusRequestX(hAdapter->device);
     if (operationResult != HID_SMBUS_SUCCESS) {
              sprintf(textReport, "2)TransferStatusRequest .\n");
             checkTransferResultCode(operationResult, (textReport + 23) );
            return operationResult;
         }
    //C) transfer status response-------
         statusOfRequest  = HID_SMBUS_S0_BUSY;
     while (statusOfRequest == HID_SMBUS_S0_BUSY) {
            //wait until transmision in progress
             operationResult =  hAdapter->HidSmbus_GetTransferStatusResponseX(hAdapter->device, &statusOfRequest, &detailedStatus, &numRetries, &bytesRead);
             if (operationResult == HID_SMBUS_READ_TIMED_OUT ) {
                break;
             }
            Sleep(1 );
     }
        //check status

     if ((statusOfRequest == HID_SMBUS_S0_COMPLETE )|| (statusOfRequest == HID_SMBUS_S0_IDLE)|| ( operationResult == HID_SMBUS_READ_TIMED_OUT)) {
         puts("3)GetTransferStatusResponse completed successfully! \n");
     } else  {
         sprintf(textReport,"3)GetTransferStatusResponse   \n");
         checkTransactionStatus(statusOfRequest, (textReport + 28) );
         return statusOfRequest;
     }
     //D) write result of transaction:
     sprintf( textReport, "5.1) Wrote bytes: %02X   \n", bytesRead);
     checkTransactionStatus(statusOfRequest, (textReport + 21) );

     return operationResult;


  }


