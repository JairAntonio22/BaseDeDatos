
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

int insert_table(Table *table, char **row);

Table* join_table(Table *table1, Table *table2, char **cols);

void print_table(Table *tabel);
