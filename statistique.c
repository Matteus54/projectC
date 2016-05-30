#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "statistique.h"
#include "gui.h"
#include "bdd.h"
#include "compte.h"

extern GtkWidget *window;
extern GtkWidget *grid;

void statistique_window(GtkWidget *widget, gpointer* data) {
  UNUSED(widget);
  UNUSED(data);

  clean_window();

  gtk_window_set_title(GTK_WINDOW(window), "Statistique");
  gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  GtkWidget *button_retour, *button_releve_stat;

  button_releve_stat = gtk_button_new_with_label("Releve");
  widget_set_margins(button_releve_stat, 0, 5, 0, 0);
  g_signal_connect(button_releve_stat, "clicked", G_CALLBACK(releve_statistique_window), NULL);

  button_retour = gtk_button_new_with_label("Retour");
  widget_set_margins(button_retour, 0, 5, 0, 0);
  g_signal_connect(button_retour, "clicked", G_CALLBACK(main_window), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_releve_stat, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_retour, 0, 1, 1, 1);

  gtk_widget_show_all(window);
}

void releve_statistique_window(GtkWidget *widget, gpointer *data) {
  UNUSED(widget);
  UNUSED(data);

  clean_window();

  gtk_window_set_title(GTK_WINDOW(window), "Statistique Releve");
  gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);


  GtkListStore      *model, *modelReleve;
  GtkWidget         *view, *viewReleve;
  GtkTreeViewColumn* column;
  GtkTreeSelection*  select;

  model = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_DOUBLE, G_TYPE_STRING);
  modelReleve = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_DOUBLE);

  account_t **listCompte = bdd_get_list_account();
  if(listCompte != NULL) {
    while(*listCompte != NULL) {
      gtk_list_store_insert_with_values(model, NULL, -1,
                                      0, (*listCompte)->iban,
                                      1, (*listCompte)->solde,
                                      2, (*listCompte)->libelle,
                                      -1);
      listCompte++;
    }
  }

  livret_t **listLivret = bdd_get_list_livret();
  if(listLivret != NULL) {
    while(*listLivret != NULL) {
      gtk_list_store_insert_with_values(model, NULL, -1,
                                      0, (*listLivret)->iban ,
                                      1, (*listLivret)->solde,
                                      2, (*listLivret)->libelle,
                                      3, (*listLivret)->plafond,
                                      4, (*listLivret)->interet,
                                      5, (*listLivret)->type_livret,
                                      -1);
      listLivret++;
    }
  }

  view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
  g_object_unref(model);

  column = gtk_tree_view_column_new_with_attributes("Iban", gtk_cell_renderer_text_new(), "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Solde", gtk_cell_renderer_text_new(), "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Libelle", gtk_cell_renderer_text_new(), "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Plafond", gtk_cell_renderer_text_new(), "text", 3, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Interet", gtk_cell_renderer_text_new(), "text", 4, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Type", gtk_cell_renderer_text_new(), "text", 5, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(tree_select_releve), modelReleve);

  viewReleve = gtk_tree_view_new_with_model(GTK_TREE_MODEL(modelReleve));
  g_object_unref(modelReleve);

  column = gtk_tree_view_column_new_with_attributes("Compte", gtk_cell_renderer_text_new(), "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewReleve), column);
  column = gtk_tree_view_column_new_with_attributes("date_debut", gtk_cell_renderer_text_new(), "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewReleve), column);
  column = gtk_tree_view_column_new_with_attributes("date_fin", gtk_cell_renderer_text_new(), "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewReleve), column);
  column = gtk_tree_view_column_new_with_attributes("montant initial", gtk_cell_renderer_text_new(), "text", 3, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewReleve), column);
  column = gtk_tree_view_column_new_with_attributes("montant final", gtk_cell_renderer_text_new(), "text", 4, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewReleve), column);

  GtkWidget *button_retour;

  button_retour = gtk_button_new_with_label("Retour");
  widget_set_margins(button_retour, 0, 5, 0, 0);
  g_signal_connect(button_retour, "clicked", G_CALLBACK(statistique_window), NULL);


  gtk_grid_attach(GTK_GRID(grid), view, 0, 0, 1, 2);
  gtk_grid_attach(GTK_GRID(grid), viewReleve, 1, 0, 1, 2);
  gtk_grid_attach(GTK_GRID(grid), button_retour, 0, 1, 1, 1);

  gtk_widget_show_all(window);
}

void tree_select_releve(GtkTreeSelection *selection, gpointer data) {
  UNUSED(data);
  GtkTreeIter iter;
  GtkTreeModel* model;
  GtkListStore* modelReleve = data;
  char *iban;
  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    gtk_tree_model_get (model, &iter, 0, &iban, -1);

    gtk_list_store_clear(modelReleve);

    releve_t **listReleve = bdd_get_list_releve(iban);
    if(listReleve != NULL) {
      while(*listReleve != NULL) {
        gtk_list_store_insert_with_values(modelReleve, NULL, -1,
                            0, (*listReleve)->compte,
                            1, (*listReleve)->date_debut,
                            2, (*listReleve)->date_fin,
                            3, (*listReleve)->montant_initial,
                            4, (*listReleve)->montant_final,
                            -1);
        listReleve++;
      }
    }
  }
}