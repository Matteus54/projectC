#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_INFO_SZ 101

const char* getfield(char* line, int num) // a reecrire (copier coller d'internet)
{
	char copy[1024];
	
	strcpy(copy, line);

	const char* tok;
	for (tok = strtok(copy, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

float traitement_releve(float argent)
{
int i;
FILE *releve, *depenses, *revenus;
char path[100], newLine[1224], line[1024];
char *categorie = malloc(MAX_INFO_SZ);
char *labels = malloc(MAX_INFO_SZ);
char *a = malloc(MAX_INFO_SZ);

if (labels == NULL) {
	printf("No memory\n");
	return 1;
}

depenses = fopen("./depenses", "a+");
revenus = fopen("./revenus", "a+");

printf("le releve est il dans le dossier par default [Y/N]\n");
fgets(a, MAX_INFO_SZ, stdin);

if ((strlen(a)>0) && (a[strlen(a) -1] == '\n'))
	a[strlen(a) -1] = '\0';

if (strcmp(a, "Y") == 0)
{
	printf("veuillez donner le nom du releve\n");
	fgets(a, MAX_INFO_SZ, stdin);

	if ((strlen(a)>0) && (a[strlen(a) -1] == '\n'))
		a[strlen(a) -1] = '\0';

	strcpy(path, "./releves/");
	strcat(path, a);

	releve = fopen(path, "r");

} else if (strcmp(a, "N") == 0) {
	printf("veuillez donner le chemin (absolu ou relatif) du releve\n");
	fgets(a, MAX_INFO_SZ, stdin);

	if ((strlen(a)>0) && (a[strlen(a) -1] == '\n'))
		a[strlen(a) -1] = '\0';

	strcpy(path, a);
	releve = fopen(path, "r");
} else {
	printf("traitement annule\n");
	return 1;
}

printf("\n\n");

fgets(line, 1024, releve); //on saute la premiere ligne (celle avec les titres des colonnes)
while(fgets(line, 1024, releve)) {

	// 
	printf("libelle : %s \n", getfield(line, 3));
	printf("date : %s \n", getfield(line, 1));
	printf("date valeur : %s \n", getfield(line, 2));
	printf("montant : %s \n\n", getfield(line, 4));

	printf("veuillez entrer la categorie :\n");
	fgets(categorie, MAX_INFO_SZ, stdin);
	printf("\n");

	if ((strlen(categorie)>0) && (categorie[strlen(categorie) -1] == '\n'))
		categorie[strlen(categorie) -1] = '\0';

	printf("veuillez entrer les labels (separes par des virgule) :\n");
	fgets(labels, MAX_INFO_SZ, stdin);
	printf("\n");

	if ((strlen(labels)>0) && (labels[strlen(labels) -1] == '\n'))
		labels[strlen(labels) -1] = '\0';

	
	// creation de la ligne qui sera ecrite
	strcpy(newLine, "");


	for(i=1; i<5; i++) {
		strcat(newLine, getfield(line, i));
		strcat(newLine, ",");
	}


	strcat(newLine, categorie); strcat(newLine, ",");

	strcat(newLine, labels);
	
	strcat(newLine, "\n");


	// ecriture de la ligne
	if (atof(getfield(line, 4)) <= 0) {
		fprintf(depenses, "%s", newLine);
	} else {
		fprintf(revenus,  "%s", newLine);
	}

	argent += atof(getfield(line, 4));
}

printf("%.2f\n", argent);

fclose(releve);
fclose(revenus);
fclose(depenses);

return argent;
}

void afficher_file(FILE *file)
{
char line[1024];
int i;

printf("\n");
printf("date		date valeur	libelle		montant		categorie	labels\n");
while(fgets(line, 1024, file)) {
	printf("%s	", getfield(line, 1));
	printf("%s	", getfield(line, 2));
	printf("%10s	", getfield(line, 3));
	printf("%s€		", getfield(line, 4));
	printf("%s	", getfield(line, 5));
	i = 6;
	while(getfield(line, i)) {
		printf("%s, ", getfield(line, i));
		i++;
	}
	printf("\n");
}

printf("\n\n");
}

void afficher_depenses()
{
FILE *file;
file = fopen("./depenses", "r");
afficher_file(file);
}


void afficher_revenus()
{
FILE *file;
file = fopen("./revenus", "r");
afficher_file(file);
}



int main(int argc, char *argv[])
{
float argent;
char *b = malloc(MAX_INFO_SZ);
FILE *info_comptes;

info_comptes = fopen("./info", "w+");
fgets(b, 100, info_comptes);
argent = atof(b);

printf("Bonjour ");

while (1)
{
printf("que voulez vous faire?\n");
printf("(1) importer un releve\n");
printf("(2) afficher les depense\n");
printf("(3) afficher les revenus\n");
printf("(Q) Quitter\n");
fgets(b, MAX_INFO_SZ, stdin);

if ((strlen(b)>0) && (b[strlen(b) -1] == '\n'))
	b[strlen(b) -1] = '\0';

if (strcmp(b,"1") == 0) {
	fprintf(info_comptes, "%.2f", traitement_releve(argent));
} else if (strcmp(b,"2") == 0) {
	afficher_depenses();
} else if (strcmp(b,"3") == 0) {
	afficher_revenus();
} else if (strcmp(b,"Q") == 0) {
	break;
}

}

fclose(info_comptes);

return 0;
}
