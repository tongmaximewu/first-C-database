
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Database db_create(){
        Database db;
        db.records = (Record*)malloc(4 * sizeof(Record));
        db.size = 0;
        db.capacity = 4;
        return db;
}

void db_append(Database* db, Record const *item){
        //more space 
        if(db->size == db->capacity){
                db->capacity *= 2;
                Record *tmpRecord = (Record*)malloc(db->capacity * sizeof(Record));

                if(tmpRecord == NULL){
                        return;
                }

                for(int i =0; i<db->size; i++){
                        tmpRecord[i] = db->records[i];
                }
                free(db->records);

                db->records = tmpRecord;
        }
        //add 
        db->records[db->size++] = *item;
}
Record * db_index(Database * db, int index){
        //invalid index
        if (index<0 || index >= db->size){
                return NULL;
        }
        return &db->records[index];
}

Record * db_lookup(Database* db, const char* handle){
        for(int i = 0; i < db->size; i++){
                if (strcmp(db->records[i].handle, handle) == 0){
                        return &db->records[i];
                }
        }
        return NULL;
}

void db_free(Database* db){
        free(db->records);
        db->records = NULL;
        db->size==0;
        db->capacity=0;
}

void db_load_csv(Database *db, char const *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", path);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        Record record;
        char *token = strtok(line, ",");
        strncpy(record.handle, token, MAX_HANDLE);
        token = strtok(NULL, ",");
        record.followers = strtoul(token, NULL, 10);
        token = strtok(NULL, ",");
        strncpy(record.comment, token, MAX_COMMENT);
        token = strtok(NULL, ",");
        record.timestamp = strtoul(token, NULL, 10);
        db_append(db, &record);
    }

    fclose(file);
}

void db_write_csv(Database* db, char const* path) {
        FILE *file = fopen(path, "w");
        if ( file == NULL){
                fprintf(stderr, "error: could not open file %s\n", path);
                return;
        }
        for (int i = 0; i < db->size; i++){
                Record* record = &db->records[i];
                fprintf(file, "%s,%lu,%s,%lu\n",  record->handle, record->followers, record->comment, record->timestamp);
        }
        fclose(file);
}
