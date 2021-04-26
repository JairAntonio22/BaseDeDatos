#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

DB* create_DB(char *name) {
    DB* db = (DB*) malloc(sizeof(DB));

    db->name = strdup(name);
    db->size = 0;
    db->tables = (Table*) calloc(sizeof(Table), 500);

    return db;
}

void delete_DB(DB *db) {
    free(db->name);
    free(db->tables);
}

void add_table(DB *db, Table table) {
    db->tables[db->size] = table;
    db->size++;
}

DB* load_db(char *filename) {
    return NULL;
}

void save_db(DB *db) {
    FILE *file = fopen(db->name, "w");

    for (int i = 0; i < db->size; i++) {
        fprintf(file, "%s\n", db->tables[i].name);
        save_table(&db->tables[i]);
    }
}

