#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

Table* create_table(int id, char *name, int ncols, char **cols) {
    Table *table = (Table*) malloc(sizeof(Table));

    table->id = id;
    table->name = strdup(name);
    table->rows = 1;
    table->cols = ncols;
    table->data = (char***) calloc(sizeof(char**), 500);
    table->data[0] = (char**) calloc(sizeof(char*), ncols);

    for (int i = 0; i < ncols; i++) {
        table->data[0][i] = strdup(cols[i]);
    }

    return table;
}

void delete_table(Table *table) {
    free(table->name);

    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols; j++) {
            free(table->data[i][j]);
        }

        free(table->data[i]);
    }
    
    free(table->data);
    free(table);
}

void insert_table(Table *table, char **reg) {
    table->data[table->rows] = (char**) calloc(sizeof(char*), table->cols);

    for (int i = 0; i < table->cols; i++) {
        table->data[table->rows][i] = strdup(reg[i]);
    }

    table->rows++;
}

void print_table(Table *table) {
    printf("%s\n", table->name);

    for (int i = 0; i < table->rows; i++) {
        printf("%i\t", i);

        for (int j = 0; j < table->cols; j++) {
            printf("%s\t", table->data[i][j]);
        }
        
        printf("\n");
    }
}
