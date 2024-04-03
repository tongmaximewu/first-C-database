#ifndef DB_H
#define DB_H
#include <time.h>
//global variables:
#define MAX_HANDLE 32
#define MAX_COMMENT 64

typedef struct Record {
        char handle[MAX_HANDLE];
        unsigned long followers;
        char comment[MAX_COMMENT];
        time_t timestamp;
} Record;


typedef struct Database {
        Record* records;
        int size;
        int capacity;
} Database;

Database db_create();

void db_append(Database * db, Record const * item);

Record *db_index(Database * db, int index);

Record *db_lookup(Database * db, char const * handle);

void db_free(Database * db);

void db_load_csv(Database * db, char const * path);

void db_write_csv(Database * db, char const * path);

#endif
