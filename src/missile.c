/*
 * Main Application Entry Point
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
#include "missile.h"

int missile_init(int debug)
{
	usb_set_debug(debug);
	usb_init();
	usb_find_busses();
	usb_find_devices();
	
	return 0;
}

struct missile *missile_create(int debug, int timeout) {
	struct missile *control = malloc(sizeof(*control));
	if (control == NULL)
		return NULL;

	control->device							= NULL;
	control->handle							= NULL;
	control->debug							= debug;
	control->timeout						= timeout;
	control->interface_claimed 	= 0;

	return control;
}

int missile_destroy(struct missile *control) 
{
	int ret;
	if (control == NULL)
		return -1;

	if (control->handle != NULL) {
		ret = usb_release_interface(control->handle, 0);
		assert(ret == 0);
		ret = usb_close(control->handle);
		assert(ret == 0);
	}
	free(control);
	return 0;
}

int missile_finddevice(struct missile *control, int device_num)
{
	int 	device_count = 0;
	struct usb_bus *bus;

	/*assert that our connection is still live*/
	assert(control != NULL);
	assert(control->interface_claimed == 0);
	assert(device_num >= 0 && device_num < 255);

	/*iterate devices looking for out device*/
	for (bus = usb_get_busses(); bus != NULL; bus = bus->next) {
		struct usb_device *dev;

		for (dev = bus->devices; dev != NULL; dev = dev->next) {
			if (control->debug) {
				printf("Found device: %04x-%04x\n", dev->descriptor.idVendor,
						dev->descriptor.idProduct);
			}

			/*if we have found our device claim it*/
			if (dev->descriptor.idVendor == VENDOR_ID &&
					dev->descriptor.idProduct == PRODUCT_ID) {
				if (control->debug)
					printf("Found control num %d\n", device_count);
				if (device_count == device_num) {
					control->device = dev;
					/*open the usb connection*/
					control->handle = usb_open(control->device);
					if (control->handle == NULL) {
						perror("usb_open failed\n");
						return -1;
					}
					if (control->debug)
						printf("Successfully opened device\n");
					return 0;
				}
				device_count ++;
			}
		}
	}
	if (control->debug)
		fprintf(stderr, "missile_usb_finddevice(%d) failed to find device %04x-%04x\n",
				device_count, VENDOR_ID, PRODUCT_ID);
	return -1;
}

int claim_interface(struct missile *control)
{
	int ret;
	char dummy[65535];

	if (control->interface_claimed == 0) {
		if (control->debug)
			printf("Trying to detach kernel driver\n");
		
		/*try and detach the kernel driver*/
		ret = usb_get_driver_np(control->handle, 0, dummy, sizeof(dummy));
		if (ret == 0) {
			if (control->debug)
				printf("Interface already claimed by driver %s, attempting to detach it\n", dummy);
				
			/* try to detach device in case usbhid has got a hold of it */
			ret = usb_detach_kernel_driver_np(control->handle, 0);
			if (ret != 0) {
				if (errno = ENODATA) {
					if (control->debug)
						printf("Device already detached\n");
				}
				else {
					if (control->debug) {
						printf("Detach failed: %s[%d]\n", strerror(errno), errno);
						printf("Continuing anyway\n");
					}
				}
			}
			else {
				if (control->debug)
					printf("detach successful\n");
			}
		}
		
		/*set the configuration for the device otherwise it will not
		 * accept any commands*/
		ret = usb_set_configuration(control->handle, 1);
		if (ret < 0) {
			perror("usb_set_configuration failed\n");
			return -1;
		}

		/*claim the base device*/
		ret = usb_claim_interface(control->handle, 0);
		if (ret < 0) {
			perror("usb_claim_interface failed\n");
			return - 1;
		}

		/*set the altnertate interface for this device*/
		ret = usb_set_altinterface(control->handle, 0);
		assert(ret >= 0);

		ret = usb_get_descriptor(control->handle, 0x00000001, 0x00000000, dummy, 0x00000012);
		if (control->debug)
			printf("1 get descriptor returned %d, bytes: \n", ret);
		
		ret = usb_get_descriptor(control->handle, 0x0000002, 0x0000000, dummy, 0x0000009);
		if (control->debug)
			printf("2 get descriptor returned %d, bytes: \n", ret);

		ret = usb_get_descriptor(control->handle, 0x0000002, 0x0000000, dummy, 0x0000022);
		if (control->debug)
			printf("3 get descriptor returned %d, bytes: \n", ret);
		
		ret = usb_release_interface(control->handle, 0);
		if (ret != 0) 
			printf("failed to release interface before set_configuration: %d\n", ret);

		ret = usb_set_configuration(control->handle, 0x0000001);
		if (control->debug)
			printf("4 set configuration returned %d\n", ret);

		ret = usb_claim_interface(control->handle, 0);
		if (ret != 0) 
			printf("claim after set_configuration failed with error %d\n", ret);

		ret = usb_set_altinterface(control->handle, 0);
		if (control->debug)
			printf("4 set alternate setting returned %d\n", ret);

		ret = usb_control_msg(control->handle, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 
				0x000000a, 0x0000000, 0x0000000, dummy, 0x0000000, 1000);
		if (control->debug)
			printf("5 control msg returned %d\n", ret);
		
		ret = usb_get_descriptor(control->handle, 0x0000022, 0x0000000, dummy, 0x0000074);
		if (control->debug)
			printf("6 get descriptor returned %d, bytes: ", ret);

		control->interface_claimed = 1;
	}
	return 0;
}

int missile_do(struct missile *control, int cmd)
{
	int delay = 0;
	unsigned char buf[64];
	memset(buf, 0, 64 );

	switch (cmd) {
		case 0:				/* stop */
			memcpy(buf, "\x00\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 100;
			break;
		case 1:				/* move up */
			memcpy(buf, "\x01\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 100;
			break;
		case 2:				/* move down */
			memcpy(buf, "\x02\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 100;
			break;
		case 4:				/* left */
			memcpy(buf, "\x04\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 100;
			break;
		case 8:				/* right */
			memcpy(buf, "\x08\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 100;
			break;
		case 10:			/* fire */
			memcpy(buf, "\x10\x00\x00\x00\x00\x00\x00\x00", 0x0000008);
			delay = 3500;
			break;
	}
	
	/*execute the command*/
	if (missile_sendcommand64(control, buf, delay)) {
		fprintf(stderr, "missile_usb_sendcommand64() failed: %s\n", 
				strerror(errno));
		return -1;
	}

	return 0;
}

int missile_sendcommand64(struct missile *control, unsigned char buf[],
													int delay) 
{
  int  ret;
  
  assert(control != NULL);
  
	/* make sure we cliam the interface before using*/
  ret = claim_interface(control);
  if (ret != 0) {
    return -1;
	fprintf(stderr, "woops couldn't claim device\n");
  }
  
	/*send the given command to the device, and sleep after for a period of 
	 * delay times 1000*/
  ret = usb_control_msg(control->handle, USB_TYPE_CLASS + USB_RECIP_INTERFACE, 
		  0x0000009, 0x0000200, 0x0000000, buf, 0x0000008, control->timeout);
	usleep(delay*1000);
  ret = usb_interrupt_read(control->handle, 0x00000081, buf, 
		  0x0000008, control->timeout);

	/*send the stop command otherwise the device will keep going*/
	memcpy(buf, "\x00\x00\x00\x00\x00\x00\x00\x00", 0x0000008); /* stop */
 	ret = usb_control_msg(control->handle, 
				USB_TYPE_CLASS + USB_RECIP_INTERFACE, 
		  	0x0000009, 0x0000200, 0x0000000, buf, 0x0000008, control->timeout);
  return 0;
}
