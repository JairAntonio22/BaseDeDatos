
typedef struct Table {
    int id;
    char *name;
    int rows;
    int cols;
    char ***data;
} Table;

Table* create_table(int id, char *name, int ncols, char **cols);

void delete_table(Table *table);

void insert_table(Table *table, char **reg);

void print_table(Table *tabel);
