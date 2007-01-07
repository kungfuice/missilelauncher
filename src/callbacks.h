/*
 * GTK Interface Callbacks
 *
 * Copyright (c) 2006-2007 Chris Boersma <chris.boersma@gmail.com>
 *
 * This application is covered by the GPL, read license for details
 */
#include <gtk/gtk.h>
#include "missile.h"

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnUp_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnStop_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnDown_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnLeftDown_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnRightDown_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnLeft_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnLeftUp_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_btnLeftUp_key_press_event           (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

void
on_btnRightUp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnFire_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_main_show                           (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btnRight_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_main_destroy                        (GtkObject       *object,
                                        gpointer         user_data);
