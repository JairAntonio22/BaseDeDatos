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
    free(db->name);

    for (int i = 0; i < db->size; i++) {
        free(db->tables[i]);
    }

    free(db->tables);
}

DB* load_db(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: %s not found", filename);
        exit(0);
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
    db->tables[db->size] = create_table(name, ncols, cols);
    db->size++;
}

int insert_db(DB *db, char *name, char **row) {
    int index = 0;

    while (strcmp(db->tables[index]->name, name) != 0 && index < db->size) {
        index++;
    }

    if (index == db->size) {
        return -1;

    } else {
        insert_table(db->tables[index], row);
        return 0;
    }
}
