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

  GtkWidget *button_retour, *button_check_stat;

  GtkTreeSelection*  selectAccount;
  GtkTreeSelection*  selectReleve;

  selectAccount = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(selectAccount, GTK_SELECTION_SINGLE);

  selectReleve = gtk_tree_view_get_selection(GTK_TREE_VIEW(viewReleve));
  gtk_tree_selection_set_mode(selectReleve, GTK_SELECTION_SINGLE);

  tree_selection_t* tree = malloc(sizeof(tree_selection_t));
  tree->account = selectAccount;
  tree->releve = selectReleve;

  button_check_stat = gtk_button_new_with_label("Checks stat");
  widget_set_margins(button_check_stat, 0, 5, 0, 0);
  g_signal_connect(button_check_stat, "clicked", G_CALLBACK(check_stat), tree);

  button_retour = gtk_button_new_with_label("Retour");
  widget_set_margins(button_retour, 0, 5, 0, 0);
  g_signal_connect(button_retour, "clicked", G_CALLBACK(statistique_window), NULL);

  int h = 30;
  gtk_grid_attach(GTK_GRID(grid), view, 0, 0, 1, h);
  gtk_grid_attach(GTK_GRID(grid), viewReleve, 2, 0, 1, h);
  gtk_grid_attach(GTK_GRID(grid), button_check_stat, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_retour, 1, 1, 1, 1);

  int i;
  for (i = 2; i<h; i++)
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" "), 1, i, 2, 1);

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
                            1, (*listReleve)->date_debut,
                            0, (*listReleve)->compte,
                            2, (*listReleve)->date_fin,
                            3, (*listReleve)->montant_initial,
                            4, (*listReleve)->montant_final,
                            -1);

        printf("%s\n", (*listReleve)->compte);
        listReleve++;
      }
    }
  }
}

void check_stat(GtkWidget *widget, tree_selection_t *tree) {
  UNUSED(widget);

  GtkTreeSelection *selectAccount = tree->account;
  GtkTreeIter iterAccount;
  GtkTreeModel *modelAccount;

  GtkTreeSelection *selectReleve = tree->releve;
  GtkTreeIter iterReleve;
  GtkTreeModel *modelReleve;

  char *iban;
  char *date_debut_releve;
  char *date_fin_releve;
  if(gtk_tree_selection_get_selected(selectAccount, &modelAccount, &iterAccount)) {
    gtk_tree_model_get(modelAccount, &iterAccount, 0, &iban, -1);
    if(gtk_tree_selection_get_selected(selectReleve, &modelReleve, &iterReleve)) {
      gtk_tree_model_get(modelReleve, &iterReleve, 1, &date_debut_releve, -1);
      gtk_tree_model_get(modelReleve, &iterReleve, 2, &date_fin_releve, -1);

      transaction_t** listTransaction = bdd_get_list_transaction(iban, date_debut_releve, date_fin_releve);
      char *xvals[1000];
      double yvals[1000];
      int i = 0;
      int k = 0;
      if(listTransaction != NULL) {
        while(listTransaction[i] != NULL) {
          int j;
          int trouve = 0;
          for(j = 0; j < k; j++) {
            if(strcmp(xvals[j], listTransaction[i]->date) == 0) {
              yvals[j] += listTransaction[i]->montant;
              trouve = 1;
            }
          }
          if(!trouve) {
            xvals[k] = listTransaction[i]->date;
            yvals[k] = listTransaction[i]->montant;
            k++;
          }
          i++;
        }

        FILE *fptr;
        fptr = fopen("data.txt", "w");
        if(fptr == NULL) {
          fprintf(stderr, "Can't create data.txt\n");
        }
        else {
          int j;
          for(j = 0; j < i; j++) {
            if (xvals[j] != NULL) {
              printf("%f\n", yvals[j]);
              printf("%s\n", xvals[j]);
              fprintf(fptr, "%s %f\n", xvals[j], yvals[j]);
            }
          }
          fclose(fptr);
        }

        system("cat mongraph.conf | gnuplot");

        //open_picture("mon_graphe.png");
      }


    }
  }

}
