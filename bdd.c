#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define UNUSED(p) ((void)(p))

static sqlite3 *db;
static char *zErrMsg = 0;
static int rc;
static char* request;
const unsigned char* login;

//Constante qui sert a definir la taille max du hashing
const int MUST_BE_LESS_THAN = 100000000; // 8 decimal digits max


//Fonction qui hash un String et renvoi son entier sur MUST_BE_LESS_THAN digits max
int hash (const char* word) {
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return hash % MUST_BE_LESS_THAN;
}


//Fonction qui sert a pas grand chose (utilise sur lapi de la connexion a la BDD)
int callback(void *NotUsed, int argc, char **argv, char **azColName){
  UNUSED(NotUsed);
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


int bdd_login(char* request) {
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      const unsigned char* text;
      text = sqlite3_column_text(stmt,0);
      printf("login with: %s\n", text);
      login = text;
      sqlite3_finalize(stmt);
      return 1;
    }
    else{
      printf("Can't able to log, login or password incorrect\n");
      sqlite3_finalize(stmt);
      return 0;
    }
  }
  else {
    printf("SQL ERROR LOGIN\n");
    return 0;
  }
}

//Fonction qui permet dexecuter une requete SQL en parametre
int bdd_execute(char *sql) {
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK) {
    fprintf(stderr, "SQL ERROR: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 0;
  }
  else {
    fprintf(stdout, "Records created successfully\n");
    return 1;
  }
}

/* Initialise la BDD lors du lancement de lapplication
  1- Creer le fichier banqueDB.db si il nexiste pas ou accede a la BDD si il existe
  2- Creer les tables si elles nexistent pas
    (utilisateur)
  3- Execute les requetes de creation des tables
*/
void bdd_init() {
  printf("Initialisation de la BDD \n");
  rc = sqlite3_open("banqueDB.db", &db);

  if(rc){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  }
  else {
    fprintf(stdout, "Opened database successfully\n");
  }

  request = "CREATE TABLE IF NOT EXISTS utilisateur (login VARCHAR2(30) PRIMARY KEY, password INT(8) NOT NULL);";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS compte (id VARCHAR2(30) PRIMARY KEY, solde INT(10) NOT NULL,"\
          "libelle VARCHAR2(255), booleanLivret BOOLEAN, login VARCHAR2(30) NOT NULL, CONSTRAINT compte_fk FOREIGN KEY (login) REFERENCES utilisateur(login));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret_type (id VARCHAR2(5) PRIMARY KEY, libelle VARCHAR2(30));";
  bdd_execute(request);

  request = "INSERT INTO livret_type VALUES ('A', 'Livret A');";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret (id VARCHAR2(30) PRIMARY KEY, "\
          "plafond INT(10) NOT NULL, interet NUMBER(4,2) NOT NULL, type VARCHAR2(5) NOT NULL,"\
          "CONSTRAINT livret_id_fk FOREIGN KEY (id) REFERENCES compte(id)"\
          "CONSTRAINT livret_type_fk FOREIGN KEY (type) REFERENCES livret_type(id));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS typeTransaction (id VARCHAR2(10) PRIMARY KEY, libelle VARCHAR2(255) NOT NULL)";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS transactionCompte (id VARCHAR2(30) PRIMARY KEY, montant INT(10) NOT NULL,"\
          "date DATE NOT NULL, commentaire VARCHAR2(255), type VARCHAR2(10) NOT NULL, compte_id VARCHAR2(30) NOT NULL,"\
          "CONSTRAINT transaction_compte_fk FOREIGN KEY (compte_id) REFERENCES compte(id),"\
          "CONSTRAINT transaction_type_fk FOREIGN KEY (type) REFERENCES typeTransaction(id));";
  bdd_execute(request);


}
