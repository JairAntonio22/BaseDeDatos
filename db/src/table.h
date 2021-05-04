
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

Table* select_table(Table *table, int ncols, char **cols, char **where);

Table* join_table(Table *table1, Table *table2, char **cols);

int insert_table(Table *table, char **row);

void print_table(Table *tabel);
