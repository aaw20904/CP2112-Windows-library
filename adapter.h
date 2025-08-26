#ifndef ADAPTER_H_INCLUDED
#define ADAPTER_H_INCLUDED
#include <windows.h>
#include "SLABCP2112.h"  // CP2112 HID-to-SMBus header
//Author: Andrii Androsovych

        #define CP2112_VENDOR_ID 0x10c4
        #define CP2112_PRODUCT_ID 0xea90

// Define function pointer types for HID-to-SMBus DLL
typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_GetNumDevices)(DWORD * lpdwNumDevices,
                                                                    const WORD vid,
                                                                    const WORD 	pid );

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_Open) 	(HID_SMBUS_DEVICE *pdevice,
                                                                 const DWORD  deviceNum,
                                                                 const WORD	vid,
                                                                 const WORD	pid	);

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_Close)(HID_SMBUS_DEVICE);

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_WriteRequest)	(const HID_SMBUS_DEVICE device,
                                                                const BYTE  	slaveAddress,
                                                                BYTE *  	buffer,
                                                                const BYTE  	numBytesToWrite
                                                            );

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_SetTimeouts)(const HID_SMBUS_DEVICE device,
                                                          const DWORD  responseTimeout
                                                          );

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_SetSmbusConfig)( 	const HID_SMBUS_DEVICE  device,
                                                                const DWORD  	bitRate,
                                                                const BYTE  	address,
                                                                const BOOL  	autoReadRespond,
                                                                const WORD  	writeTimeout,
                                                                const WORD  	readTimeout,
                                                                const BOOL  	sclLowTimeout,
                                                                const WORD  	transferRetries
                                                            );

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_GetOpenedString)( 	const HID_SMBUS_DEVICE 	device,
                                                                char *  	deviceString,
                                                                const HID_SMBUS_GETSTRING  	options
                                                            );

typedef HID_SMBUS_STATUS (WINAPI *pfHidSmbus_GetString)( const DWORD deviceNum,
                                                            const WORD  	vid,
                                                            const WORD  	pid,
                                                            char *  	deviceString,
                                                            const HID_SMBUS_GETSTRING  	options
                                                        );

typedef HID_SMBUS_STATUS (*pfHidSmbus_ReadRequest)(	const HID_SMBUS_DEVICE device,
                                                    const BYTE 	slaveAddress,
                                                    const WORD 	numBytesToRead
                                                  );

typedef HID_SMBUS_STATUS  (*pfHidSmbus_GetReadResponse)( const HID_SMBUS_DEVICE  	device,
                                                        HID_SMBUS_S0 *  	status,
                                                        BYTE *  	buffer,
                                                        const BYTE  	bufferSize,
                                                        BYTE *  	numBytesRead
                                                      );

typedef HID_SMBUS_STATUS (*pfHidSmbus_TransferStatusRequest)(const HID_SMBUS_DEVICE	device);

typedef HID_SMBUS_STATUS (*pfHidSmbus_GetTransferStatusResponse)(const HID_SMBUS_DEVICE	device,
                            HID_SMBUS_S0 *  	status,
                            HID_SMBUS_S1 *  	detailedStatus,
                            WORD *  	numRetries,
                            WORD *  	bytesRead
                        );
typedef HID_SMBUS_STATUS (*pfHidSmbus_ForceReadResponse)(
                                                         HID_SMBUS_DEVICE device,
                                                          WORD numBytesToRead
                                                        );


 typedef struct adapterStruct {
        WORD vendorID;
        WORD productID;
        DWORD numDevices;
        HMODULE libraryHandle;
        HID_SMBUS_DEVICE device;
        pfHidSmbus_GetNumDevices HidSmbus_GetNumDevicesX;
        pfHidSmbus_Open HidSmbus_OpenX;
        pfHidSmbus_Close HidSmbus_CloseX;
        pfHidSmbus_WriteRequest HidSmbus_WriteRequestX;
        pfHidSmbus_ReadRequest  HidSmbus_ReadRequestX;
        pfHidSmbus_GetReadResponse  HidSmbus_GetReadResponseX;
        pfHidSmbus_SetTimeouts HidSmbus_SetTimeoutsX;
        pfHidSmbus_SetSmbusConfig HidSmbus_SetSmbusConfigX;
        pfHidSmbus_GetString HidSmbus_GetStringX;
        pfHidSmbus_TransferStatusRequest HidSmbus_TransferStatusRequestX;
        pfHidSmbus_GetTransferStatusResponse HidSmbus_GetTransferStatusResponseX;
        pfHidSmbus_ForceReadResponse HidSmbus_ForceReadResponseX;
    } adapterStruct;

  int initAdapter (adapterStruct *hAdapter,
                    DWORD bitsPerSec,
                     DWORD timeouts,
                     DWORD numberOfDeviceToOpen,
                     char* reportString);

 void deInitAdapterCloseLib(adapterStruct * hAdapter);



unsigned int readData (adapterStruct * adapterHandle,
                       unsigned char * buffer,
                        unsigned char addressFrom,
                                char* textReport,
                        unsigned short amountOfBytes);

unsigned int  writeData ( adapterStruct * hAdapter,
                          unsigned char * buffer,
                           unsigned char addrWhom,
                                   char *textReport,
                                    WORD amountOfBytes);

unsigned int testFunction ( adapterStruct * hAdapter,
                          unsigned char * buffer,
                           unsigned char addrWhom,
                                   char *textReport,
                                    WORD amountOfBytes);

 int loadDynamicLibrary(adapterStruct *hAdapter, char* report);
   int amountOfCP2112 (adapterStruct *hAdapter);
 //the number of CP2112 device, which will has been opened during initialization inside the initAdapter() function

 #define CP2112_DEVICE_NUMBER   0

#endif // ADAPTER_H_INCLUDED
