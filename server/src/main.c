#include <stdio.h>

#include "db.h"

int main(void) {
    if (0) { 
        DB *db = create_db("MyDB");

        char *cols[] = {"Nombre", "Edad", "Sexo"};

        add_table(db, "Personas", 3, cols);

        {
            char *row[] = {"Jair", "21", "Hombre"};
            insert_db(db, "Personas", row);
        }

        {
            char *row[] = {"David", "21", "Hombre"};
            insert_db(db, "Personas", row);
        }

        {
            char *row[] = {"Lorena", "21", "Mujer"};
            insert_db(db, "Personas", row);
        }

        {
            char *row[] = {"Itzel", "20", "Mujer"};
            insert_db(db, "Personas", row);
        }

        save_db(db);
        delete_db(db);

    } else {
        DB *db = load_db("MyDB.txt");

        print_table(db->tables[0]);

        char *row[] = {"Eric", "21", "Hombre"};
        insert_db(db, "Personas", row);

        save_db(db);

        delete_db(db);
    }
}
