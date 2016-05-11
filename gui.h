#include <gtk/gtk.h>

typedef struct {
  GtkWidget *login;
  GtkWidget *password;
  GtkWidget *password_check;
} user_entry_t;


void try_login();
void create_user(GtkWidget* widget, gpointer* data);
void create_user_form();
void login_page(GtkApplication *app);
int gui_init(int argc, char **argv);
