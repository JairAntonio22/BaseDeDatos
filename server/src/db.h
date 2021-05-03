#include "table.h"

typedef struct DB {
    char *name;
    int size;
    Table **tables;
} DB;

DB* create_db(char *name);

void delete_db(DB* db);

DB* load_db(char *filename);

void save_db(DB *db);

void add_table(DB *db, char *name, int ncols, char **cols);

Table* select_db(DB *db, char *name, int ncols, char **cols, char **where);

Table* join_db(DB *db, char *name1, char *name2, char **cols);

int insert_db(DB *db, char *name, char **row);
