#include <stdio.h>

#include "db.h"

int main(void) {
    if (1) { 
        char *cols[] = {"Usuario", "Password"};
        Table *users = create_table("Usuarios", 2, cols);

        {
            char *row[] = {"admin", "admin"};
            insert_table(users, row);
        }

        {
            char *row[] = {"hector", "maestro"};
            insert_table(users, row);
        }

        {
            char *row[] = {"david", "azul"};
            insert_table(users, row);
        }

        {
            char *row[] = {"nina", "cacahuates"};
            insert_table(users, row);
        }

        {
            char *row[] = {"rodrigo", "Kwok"};
            insert_table(users, row);
        }

        {
            char *row[] = {"Jair", "lambda"};
            insert_table(users, row);
        }

        save_table(users);
        delete_table(users);

    } else {
        DB *db = load_db("MyDB.txt");

        print_table(db->tables[0]);
        print_table(db->tables[1]);

        char *cols1[] = {"CursoNombre", "Nombre"};
        Table *table1 = join_db(db, "Personas", "Cursos", cols1);

        print_table(table1);

        printf("%s\n", encode_table(db->tables[0]));

        save_db(db);
        delete_db(db);
    }
}
