int hash (const char* word);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
char **bdd_get_type_livret();
int bdd_login(char* request);
int bdd_execute(char *sql);
void bdd_init();
