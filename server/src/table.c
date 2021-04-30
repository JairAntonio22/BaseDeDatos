#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

Table* create_table(char *name, int ncols, char **cols) {
    Table *table = (Table*) malloc(sizeof(Table));

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

Table* dup_table(Table *table) {
    Table *dup = create_table(table->name, table->cols, table->data[0]);

    for (int i = 1; i < table->rows; i++) {
        insert_table(dup, table->data[i]);
    }

    return dup;
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

Table* load_table(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: %s not found\n", filename);
        exit(0);
    }

    char *word = (char*) calloc(sizeof(char), 500);
    int size = 0;

    char ***data = (char***) calloc(sizeof(char**), 500);
    int nrows = 0;
    int ncols = 0;

    data[0] = (char**) calloc(sizeof(char*), 500);
    int ccol = 0;

    for (char c = fgetc(file); c != EOF; c = fgetc(file)) {
        switch (c) {
        case ',':
            word[size] = '\0';
            data[nrows][ccol] = strdup(word);
            size = 0;
            ccol++;
            break;

        case '\n':
            word[size] = '\0';
            data[nrows][ccol] = strdup(word);
            size = 0;
            ncols = ccol > ncols ? ccol : ncols;
            ccol = 0;
            nrows++;
            data[nrows] = (char**) calloc(sizeof(char*), 500);
            break;

        default:
            word[size] = c;
            size++;
        }
    }

    fclose(file);
    free(word);

    char *name = strdup(filename);
    name[strchr(name, '.') - name] = '\0';

    Table *table = create_table(name, ncols + 1, data[0]);
    free(name);

    for (int i = 1; i < nrows; i++) {
        insert_table(table, data[i]);

        for (int j = 0; j < ncols; j++) {
            free(data[i][j]);
        }
        
        free(data[i]);
    }

    free(data);

    return table;
}

void save_table(Table *table) {
    char *filename = (char*) calloc(sizeof(char), strlen(table->name) + 5);
    strcpy(filename, table->name);
    strcat(filename, ".csv");
    
    FILE *file = fopen(filename, "w");
    
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols - 1; j++) {
            fprintf(file, "%s,", table->data[i][j]);
        }

        fprintf(file, "%s\n", table->data[i][table->cols - 1]);
    }

    free(filename);
    fclose(file);
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
