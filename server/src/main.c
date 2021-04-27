#include <stdio.h>

#include "table.h"

int main(void) {
    /*
    char *cols[] = {"Nombre", "Apellido", "Edad", "Sexo"};

    Table *table = create_table("Personas", 4, cols);

    char *row1[] = {"Jair", "Antonio", "21", "Hombre"};
    insert_table(table, row1);

    char *row2[] = {"Lorena", "Porcayo", "21", "Mujer"};
    insert_table(table, row2);

    insert_table(table, row1);

    char *row3[] = {"Jorge", "Benitez", "21", "Hombre"};
    insert_table(table, row3);
    */

    Table *table = load_table("Personas.csv");

    print_table(table);

    delete_table(table);
}
