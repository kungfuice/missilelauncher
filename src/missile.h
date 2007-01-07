/*
 * Main Application Include File
 *
 * Copyright (c) 2006-2007 Chris Boersma <chris.boersma@gmail.com>
 *
 * This application is covered by the GPL, read license for details
 */

#ifndef DEPEND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <assert.h>
#include <usb.h>
#include <errno.h>
#endif

#ifndef __INC_MissileLauncher_h
#define __INC_MissileLauncher_h

/*set the vendor and product id of the device*/
#define VENDOR_ID 0x1941
#define PRODUCT_ID 0x8021

// packet cmd masks
#define MISSILE_STOP  0
#define MISSILE_LEFT  1
#define MISSILE_RIGHT 2
#define MISSILE_UP    4
#define MISSILE_DOWN  8
#define MISSILE_FIRE  16

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * This structure encapsulates our connection to the
	 * missile launcher
	 */
	typedef struct missile { 
		struct usb_device *device;						/*usb device*/
		usb_dev_handle    *handle;						/*device handle*/
		int               debug;    					/*set to 1 if debug enabled*/
		int               timeout;  					/*usb timeout in miliseconds*/
		int               interface_claimed;	/*is this interface claimed*/
	};

	/*
	 * Initialize usb connection
	 *
	 * @param debug - set the debug value for the usb connection 1 true, 0 false
	 *
	 * @return 0 on success, -1 on failure
	 */
	int missile_init(int debug);

	/*
	 * Create the missile connection
	 *
	 * @param debug - set the debug value for this connection 1 true, 0 false
	 * @param timeout - milisecond USB timeout value
	 *
	 * @return missile structure
	 */
	struct missile *missile_create(int debug, int timeout);

	/*
	 * Clean up usb connection
	 *
	 * @param control			current connection
	 *
	 * @return 0 on success, -1 on failure
	 */
	int missile_destroy(struct missile *control);

	/*
	 * Find usb device
	 *
	 * @param control			current connection
	 * @param device_num	device number to enumerate
	 *
	 * @return 0 on success, -1 on failure
	 */
	int missile_finddevice(struct missile *control, int device_num);
	
	/*
	 * Perform an USB Missile Launcher Command
	 *
	 * @param control			current connection
	 * @param cmd					cmd to execute, 0, 1, 2, 4, 8, 10 are acceptable
	 *
	 * @return 0 on success, -1 on failure
	 */ 
	int missile_do(struct missile *control, int cmd);
		    
	/*
	 * Send a usb command
	 *
	 * @param control    library handle
	 * @param ...        (first) 8 bytes of packet
	 *
	 * @return 0 on success, -1 on failure
	 */ 
	 int missile_sendcommand64(struct missile *control, unsigned char buf[],
			 											 int delay);

#ifdef __cplusplus
}
#endif

#endif //__INC_MissileLauncher_h
