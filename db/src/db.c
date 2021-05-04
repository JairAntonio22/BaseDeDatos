#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

DB* create_db(char *name) {
    DB *db = (DB*) malloc(sizeof(DB));

    db->name = strdup(name);
    db->size = 0;
    db->tables = (Table**) calloc(sizeof(Table*), 500);

    return db;
}

void delete_db(DB *db) {
    if (db == NULL) {
        return;
    }

    free(db->name);

    for (int i = 0; i < db->size; i++) {
        free(db->tables[i]);
    }

    free(db->tables);
}

DB* load_db(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return NULL;
    }

    char *name = strdup(filename);
    name[strchr(name, '.') - name] = '\0';

    DB *db = create_db(name);
    free(name);

    char *tablename = (char*) calloc(sizeof(char), 50);

    while (!feof(file)) {
        fscanf(file, "%s\n", tablename);
        db->tables[db->size] = load_table(tablename);
        db->size++;
    }

    free(tablename);

    return db;
}

void save_db(DB *db) {
    if (db == NULL) {
        return;
    }

    char *filename = (char*) calloc(sizeof(char), 50);
    strcpy(filename, db->name);
    strcat(filename, ".txt");

    FILE *file = fopen(filename, "w");

    for (int i = 0; i < db->size; i++) {
        strcpy(filename, db->tables[i]->name);
        strcat(filename, ".csv");

        fprintf(file, "%s\n", filename);
        save_table(db->tables[i]);
    }

    fclose(file);
    free(filename);
}

void add_table(DB *db, char *name, int ncols, char **cols) {
    if (db == NULL) {
        return;
    }

    for (int i = 0; i < db->size; i++) {
        if (strcmp(name, db->tables[i]->name) == 0) {
            return;
        }
    }

    db->tables[db->size] = create_table(name, ncols, cols);
    db->size++;
}

Table* get_table(DB *db, char *name) {
    if (db == NULL) {
        return NULL;
    }

    int index = 0;

    while (strcmp(db->tables[index]->name, name) != 0 && index < db->size) {
        index++;
    }

    return index == db->size ? NULL : db->tables[index];
}

Table* select_db(DB *db, char *name, int ncols, char **cols, char **where) {
    if (db == NULL) {
        return NULL;
    }

    Table *table = get_table(db, name);

    return table == NULL ? NULL : select_table(table, ncols, cols, where);
}

Table* join_db(DB *db, char *name1, char *name2, char **cols) {
    if (db == NULL) {
        return NULL;
    }

    Table *table1 = get_table(db, name1);
    Table *table2 = get_table(db, name2);

    return table1 == NULL || table2 == NULL
        ? NULL 
        : join_table(table1, table2, cols);
}

int insert_db(DB *db, char *name, char **row) {
    if (db == NULL) {
        return -1;
    }

    Table *table = get_table(db, name);

    return table == NULL ? -1 : insert_table(table, row);
}
