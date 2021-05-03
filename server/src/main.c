#include <stdio.h>

#include "db.h"

int main(void) {
    if (0) { 
        DB *db = create_db("MyDB");

        {
            char *cols[] = {"Nombre", "Edad", "Sexo", "CursoNombre"};
            add_table(db, "Personas", 4, cols);

            {
                char *row[] = {"Jair", "21", "Hombre", "Ajedrez"};
                insert_db(db, "Personas", row);
            }

            {
                char *row[] = {"David", "21", "Hombre", "Python"};
                insert_db(db, "Personas", row);
            }

            {
                char *row[] = {"Lorena", "21", "Mujer", "Dibujo"};
                insert_db(db, "Personas", row);
            }

            {
                char *row[] = {"Itzel", "20", "Mujer", "Dibujo"};
                insert_db(db, "Personas", row);
            }
        }

        {
            char *cols[] = {"Nombre", "Categoria"};
            add_table(db, "Cursos", 2, cols);

            {
                char *row[] = {"Dibujo", "Arte"};
                insert_db(db, "Cursos", row);
            }

            {
                char *row[] = {"Python", "Programacion"};
                insert_db(db, "Cursos", row);
            }

            {
                char *row[] = {"Ajedrez", "Juegos"};
                insert_db(db, "Cursos", row);
            }
        }

        save_db(db);
        delete_db(db);

    } else {
        DB *db = load_db("MyDB.txt");

        print_table(db->tables[0]);
        print_table(db->tables[1]);

        char *cols[] = {"CursoNombre", "Nombre"};
        Table *table = join_db(db, "Personas", "Cursos", cols);
        print_table(table);
        delete_table(table);

        save_db(db);
        delete_db(db);
    }
}
