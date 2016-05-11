#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define UNUSED(p) ((void)(p))

static sqlite3 *db;
static char *zErrMsg = 0;
static int rc;
static char* request;



/*
unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while(c = *str++) {
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  }
  return hash;
}
*/


int callback(void *NotUsed, int argc, char **argv, char **azColName){
  UNUSED(NotUsed);
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}


int bdd_execute(char *sql) {
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK) {
    fprintf(stderr, "SQL ERROR: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 1;
  }
  else {
    fprintf(stdout, "Records created successfully\n");
    return 0;
  }
}


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

  request = "CREATE TABLE IF NOT EXISTS utilisateur (login VARCHAR2(30) PRIMARY KEY, password VARCHAR2(30) NOT NULL);";
  bdd_execute(request);
}
