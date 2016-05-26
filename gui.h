#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

#define UNUSED(p) ((void)(p))

void show_hide_button_box(GtkWidget *button, GtkWidget *box);
void clean_window();
void close_window(GtkWidget *widget, gpointer window);
void alert_window(char *alertText);
void main_window();
void main_handler(GtkApplication *app);
int gui_init(int argc, char **argv);

#endif
