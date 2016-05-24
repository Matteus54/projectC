#include <gtk/gtk.h>

#define UNUSED(p) ((void)(p))

void show_hide_button_box(GtkWidget *button, GtkWidget *box);
void clean_window();
void close_window(GtkWidget *widget, gpointer window);



int gui_init(int argc, char **argv);
