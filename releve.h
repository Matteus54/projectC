#ifndef RELEVE_H
#define RELEVE_H

typedef struct {
  char compte[35];
  char date_debut[11];
  char date_fin[11];
  double montant_initial;
  double montant_final;
} releve_t;


#endif
