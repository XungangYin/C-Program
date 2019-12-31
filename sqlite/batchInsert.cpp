#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

int main ()
{
    sqlite3    *db;
    
    int rc = sqlite3_open("lab.db", &db);
    
    if (rc != SQLITE_OK) {
                fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);       
                return 1;
        }

    sqlite3_stmt *res;
    char *SQL = "CREATE TABLE IF NOT EXISTS testtable (ID INTEGER PRIMARY KEY, name TEXT, gender TEXT, age REAL, value REAL)";

        if(sqlite3_prepare_v2(db, SQL, -1, &res, 0)) {
                printf("Failed to create table!\n");
                if (res)
                        sqlite3_finalize(res);
                sqlite3_close(db);
                return 1;
        }

        if (sqlite3_step(res) != SQLITE_DONE) {
                sqlite3_finalize(res);
                sqlite3_close(db);
                return 1;
        }
        sqlite3_finalize(res);
        printf ("Succeed to create testtable now.\n");
        /* 手动开启事物 */
        sqlite3_stmt* stmtb = NULL;
        const char* beginSQL = "BEGIN TRANSACTION";
        if (sqlite3_prepare_v2(db,beginSQL,strlen(beginSQL),&stmtb,NULL) != SQLITE_OK) {
                if (stmtb)
                        sqlite3_finalize(stmtb);
                sqlite3_close(db);
                return 1;
        }
        if (sqlite3_step(stmtb) != SQLITE_DONE) {
                sqlite3_finalize(stmtb);
                sqlite3_close(db);
                return 1;
        }
        sqlite3_finalize(stmtb);
        /*****************************/
        
        char *err_msg = 0;
        char *sql = "INSERT INTO testtable VALUES(NULL, ?, ?, ?, ?)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
                if (stmt)
                        sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 1;
        }

        char *name = "xiao";
        char *m = "male";
        char *f = "female";
        
        for (int i = 1; i <= 10; i++) {
                sqlite3_bind_null (stmt, 0);
                sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
                if(i % 2 == 0)
                        sqlite3_bind_text(stmt, 2, m, -1, SQLITE_TRANSIENT);
                else
                        sqlite3_bind_text(stmt, 2, f, -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, i + 24);
                double fl = i * 9.9;
                sqlite3_bind_double(stmt, 4, fl);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                        sqlite3_finalize(stmt);
                        sqlite3_close(db);
                        return 1;
                }
                sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);

        /* 手动关闭事物 */
        sqlite3_stmt* stmtc = NULL;
        const char *commitSQL = "COMMIT TRANSACTION";
        if (sqlite3_prepare_v2(db,commitSQL,strlen(commitSQL),&stmtc,NULL) != SQLITE_OK) {
                if (stmtc)
                        sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 1;
        }
        if (sqlite3_step(stmtc) != SQLITE_DONE) {
                sqlite3_finalize(stmtc);
                sqlite3_close(db);
                return 1;
        }
        sqlite3_finalize(stmtc);
        /*********************************/
        
        sqlite3_close(db);
        
        return 0;
}
