// File Name: select2.cpp
// Author: YXG
// Created Time: 2019年11月25日 星期一 09时53分51秒

#include <iostream>
#include <sqlite3.h>
#include <string.h>
#include <string>

using namespace std;

void doTest(){
    sqlite3 *conn = nullptr;
    int rc = sqlite3_open("db2.db",&conn);
    if(rc != SQLITE_OK){
        sqlite3_close(conn);
        return ;    
    }    
    
    const char *createTable = "create table testtable (int_col INT, float_col REAL,string_col TEXT)";
    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(conn,createTable,strlen(createTable),&stmt,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_close(conn);
        return ;    
    }
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        sqlite3_close(conn);
        return ;    
    }
    sqlite3_finalize(stmt);
    cout<<"Succeed to creat database and table"<<endl;

    //插入数据
    sqlite3_stmt *stmt2 = nullptr;
    const char *insertSQL = "INSERT INTO testtable VALUES(%d,%f,'%s')";
    int count = 5;
    char c[64] = {0};

    for(int i  =0; i< count; ++i){
        sprintf(c,insertSQL,i,i*1.1,"this is a test.");
            
        if(sqlite3_prepare_v2(conn,c,strlen(c),&stmt2,nullptr) != SQLITE_OK){
            sqlite3_finalize(stmt2);
            sqlite3_close(conn);
         return ;    
         }

        if(sqlite3_step(stmt2) != SQLITE_DONE){
            sqlite3_finalize(stmt2);
            sqlite3_close(conn);
            return ;    
        }
        cout<<"Succeend to insert test data."<<endl;
    }

    sqlite3_finalize(stmt2);
    //执行select查询
    const char *selectSQL = "SELECT * FROM testtable";
    sqlite3_stmt *stmt3 = nullptr;
    if(sqlite3_prepare_v2(conn,selectSQL,strlen(selectSQL),&stmt3,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt3);
        sqlite3_close(conn);
        return ;
    }
    int fieldCount = sqlite3_column_count(stmt3);
    do{
        int rc = sqlite3_step(stmt3);
        //对于查询而言，如果结果中有数据，则返回SQLITE_ROW()
        if(rc == SQLITE_ROW){
            for(int i = 0;i < fieldCount;++i){
            //先记录返回字段的类型，然后获取具体的值
            int type = sqlite3_column_type(stmt3,i);
            if(type == SQLITE_INTEGER){
                int v = sqlite3_column_int(stmt3,i);
                cout<<"The Integer value is: "<<v<<endl;
            }else if(type == SQLITE_FLOAT){
                double v = sqlite3_column_double(stmt3,i);
                cout<<"The float value is: "<<v<<endl;
            }else if(type == SQLITE_TEXT){
                const char * v = (const char *)sqlite3_column_text(stmt3,i);
                printf("The text value is: %s .\n",v);    
            }else{
                cout<<"The result is nullptr"<<endl;    
            }  
          }
        }else if(rc == SQLITE_DONE){
            cout<<"Select finished "<<endl;    
            break;
        }else{
            cout<<"Field to select"<<endl;
            sqlite3_finalize(stmt3);
            sqlite3_close(conn);
            return ;    
        }
    
    
    }while(true);
    sqlite3_finalize(stmt3);

    //指定查询第n行的数据（n应该小于数据表中的行数）
    sqlite3_stmt *stmt5 = nullptr;
    int r =  3;
   // const char * select2 = "select * from testtable limit '\r',1";//从第n行开始的一行数据
    const char *select2="  SELECT * FROM testtable LIMIT 2,1";
    int t = sqlite3_prepare_v2(conn,select2,strlen(select2),&stmt5,nullptr);
    if(t != SQLITE_OK){
        cout<<"编译失败"<<endl;
    }

    int s_rc =  sqlite3_step(stmt5);
    if(s_rc  == SQLITE_ROW){ 
            for(int i = 0;i < fieldCount;++i){
            //先记录返回字段的类型，然后获取具体的值
            int type = sqlite3_column_type(stmt5,i);
            if(type == SQLITE_INTEGER){
                int v = sqlite3_column_int(stmt5,i);
                cout<<"The Integer value is: "<<v<<endl;
            }else if(type == SQLITE_FLOAT){
                double v = sqlite3_column_double(stmt5,i);
                cout<<"The float value is: "<<v<<endl;
            }else if(type == SQLITE_TEXT){
                const char * v = (const char *)sqlite3_column_text(stmt5,i);
                printf("The text value is: %s .\n",v);    
            }else{
                cout<<"The result is nullptr"<<endl;    
            }  
          }
    }else {
        sqlite3_finalize(stmt5);
        cout<<"选择特定行失败"<<endl;    
    }

        sqlite3_finalize(stmt5);
    
//7. 为了方便下一次测试运行，我们这里需要删除该函数创建的数据表，否则在下次运行时将无法
 //创建该表，因为它已经存在。
     const char* dropSQL = "DROP TABLE testtable";
     sqlite3_stmt* stmt4 = NULL;
     if (sqlite3_prepare_v2(conn,dropSQL,strlen(dropSQL),&stmt4,NULL) != SQLITE_OK) {
          if (stmt4)
              sqlite3_finalize(stmt4);
              sqlite3_close(conn);
              return;
          }
          if (sqlite3_step(stmt4) == SQLITE_DONE) {
              printf("The test table has been dropped.\n");
          }
              sqlite3_finalize(stmt4);
              sqlite3_close(conn);
    
}


int main(int argc, char **argv){
    
    
    doTest();

    return 0;    
}
