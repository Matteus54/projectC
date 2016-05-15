#include <gtk/gtk.h>

typedef struct {
  GtkWidget *login;
  GtkWidget *password;
  GtkWidget *password_check;
} user_entry_creation_t;

typedef struct {
  GtkWidget *login;
  GtkWidget *password;
} user_entry_login_t;



void clean_window();
void close_window(GtkWidget *widget, gpointer window);
void try_login(GtkWidget* widget, gpointer* data);
void create_user(GtkWidget* widget, gpointer* data);
void create_user_form();
void login_page(GtkApplication *app);
int gui_init(int argc, char **argv);
