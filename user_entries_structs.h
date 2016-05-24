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
