#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

#define UNUSED(p) ((void)(p))

void widget_set_margins(GtkWidget *widget, int top, int bottom, int left, int right);
void widget_set_margin(GtkWidget *widget, int margin);

void show_hide_button_box(GtkWidget *button, GtkWidget *box);

void clean_window();
void close_window(GtkWidget *widget, gpointer window);

char* file_browser();
void open_picture(char *filename);

void alert_dialog(GtkWidget *activeWindow, gchar *text);
void alert_dialog_then_close(GtkWidget *activeWindow, gchar *text);

void main_window();
void main_handler(GtkApplication *app);
int gui_init(int argc, char **argv);

#endif
