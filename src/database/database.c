#include "database.h"
#include <sqlite3.h>
#include <stdio.h>

static sqlite3* db;

void init_database(const char* db_name) {
    if (sqlite3_open(db_name, &db)) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Opened database successfully\n");
    }
}

void execute_query(const char* query) {
    char* err_msg = 0;
    if (sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Query executed successfully\n");
    }
}
