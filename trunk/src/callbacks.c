/*
 *  GTK Interface callbacks
 *   
 *  Copyright (c) 2006-2007 Chris Boersma <chris.boersma@gmail.com>
 *  
 *  This application is covered by the GPL, read license for details
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "missile.h"

#define DEBUG 1
#define USB_TIMEOUT 1000

struct missile *control;
GtkWidget *about;

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if (control->debug)
		fprintf(stderr, "Destroying device\n");
	missile_destroy(control);
	gtk_main_quit();
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	about = create_about();
	gtk_widget_show(about);
}


void
on_btnUp_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button Up Clicked!\n");
	if (control->interface_claimed > 0)
		missile_do(control, 1);
}


void
on_btnStop_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	 if (DEBUG)
   	fprintf(stderr, "Button Up Clicked!\n");
	 if (control->interface_claimed > 0)
	 	missile_do(control, 0);
}


void
on_btnDown_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button Down Clicked!\n");
	if (control->interface_claimed > 0)
		missile_do(control, 2);
}


void
on_btnLeftDown_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button LeftDown Clicked!\n");
	if (control->interface_claimed > 0) {
		missile_do(control, 2);
		missile_do(control, 4);
	}
}


void
on_btnRightDown_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

	if (DEBUG)
		fprintf(stderr, "Button RightDown Clicked!\n");
	if (control->interface_claimed > 0) {
		missile_do(control, 2);
		missile_do(control, 8);
	}
}


void
on_btnLeft_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button Left Clicked!\n");
	if (control->interface_claimed > 0)
		missile_do(control, 4);
}


void
on_btnLeftUp_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

	if (DEBUG)
		fprintf(stderr, "Button LeftUp Clicked!\n");
	if (control->interface_claimed > 0) {
		missile_do(control, 1);
		missile_do(control, 4);
	}
}

void
on_btnRightUp_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button RightUp Clicked!\n");
	if (control->interface_claimed > 0) {
		missile_do(control, 1);
		missile_do(control, 8);
	}
}

void
on_btnFire_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Button Fire Clicked!\n");
	if (control->interface_claimed > 0)
		missile_do(control, 10);
}


void
on_main_show                           (GtkWidget       *widget,
                                        gpointer         user_data)
{
	/*intialize missile interface*/
	if (missile_init(DEBUG) != 0) exit(0);

	control = missile_create(DEBUG, USB_TIMEOUT);
	if (control == NULL) exit(0);

	if (missile_finddevice(control, 0) != 0) exit(0);
	claim_interface(control);
}


void
on_btnRight_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	if (DEBUG)
		fprintf(stderr, "Right Button Clicked!\n");
	if (control->interface_claimed > 0)
		missile_do(control, 8);
}


void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(about);	
}

void
on_main_destroy                        (GtkObject       *object,
                                        gpointer         user_data)
{
	missile_destroy(control);
	gtk_main_quit();
}

