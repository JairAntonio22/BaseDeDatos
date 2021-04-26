#include "table.h"

typedef struct DB {
    char *name;
    int size;
    Table *tables;
} DB;

DB* create_DB(char *name);

void delete_DB(DB* db);

void add_table(DB *db, Table table);

DB* load_db(char *filename);

void save_db(DB *db);
