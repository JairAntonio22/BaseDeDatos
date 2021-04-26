
typedef struct Table {
    char *name;
    int rows;
    int cols;
    char ***data;
} Table;

Table* create_table(char *name, int ncols, char **cols);

void delete_table(Table *table);

Table* load_table(char *filename);

void save_table(Table *table);

void insert_table(Table *table, char **reg);

void print_table(Table *tabel);
