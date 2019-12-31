// File Name: select.cpp
// Author: YXG
// Created Time: 2019年11月22日 星期五 10时20分57秒

/*
    目的：高效批量插入数据
*/


#include <iostream>
#include <string.h>
#include <sqlite3.h>
#include <string>
using namespace std;

void doTest(){
    sqlite3 *conn = nullptr;
    //创建数据库和数据表
    int rc = sqlite3_open("db.db",&conn);
    if(rc != SQLITE_OK){
        sqlite3_close(conn);
        return ;    
    }
    const char *createTable= "CREATE TABLE TESTTABLE (first_col INT, second_col REAL,Thired_col TEXT)";
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
    cout<<"Succeed to create TABLE TESTTABLE"<<endl;

    //显示 开启一个事物
    sqlite3_stmt *stmt2 = nullptr;
    const char *beginSQL = "BEGIN TRANSACTION";
    if(sqlite3_prepare_v2(conn,beginSQL,strlen(beginSQL),&stmt2,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt2);
        sqlite3_close(conn);
        return ;
    }
    if(sqlite3_step(stmt2) != SQLITE_DONE){
        sqlite3_finalize(stmt2);
        sqlite3_close(conn);
        return ;    
    }
    sqlite3_finalize(stmt2);
    
    //构建基于绑定变量的插入数据.(?号：表示参数变量的占位符)
    const char *insertSQL = "INSERT INTO TESTTABLE VALUES(?,?,?)";
    //const char *insertSQL = "insert into TESTTABLE VALUES(%d,%f,'%s')";

    sqlite3_stmt *stmt3 = NULL;
    if(sqlite3_prepare_v2(conn,insertSQL,strlen(insertSQL),&stmt3,NULL) != SQLITE_OK){
        sqlite3_finalize(stmt3);
        sqlite3_close(conn);
        return ;    
    }
    int insertCount = 5000;
    const char *str = "This is test data";
    //基于已有的SQL语句，迭代的绑定不同的变量数据
    for(int i = 0; i < insertCount; ++i){
        //在绑定时，最左边的变量索引值是1
        sqlite3_bind_int(stmt3,1,i);
        sqlite3_bind_text(stmt3,3,str,strlen(str),SQLITE_TRANSIENT);
        if(sqlite3_step(stmt3) != SQLITE_DONE){
            sqlite3_finalize(stmt3);
            sqlite3_close(conn);
            return ;
        }
        //重新初始化sqlite3_stmt对象
        sqlite3_reset(stmt3);
        cout<<" Insert Succeed."<<endl; 
    }
    //提交之前的事物
    const char *commit = "COMMIT";
    sqlite3_stmt *stmt4 = nullptr;
    if(sqlite3_prepare_v2(conn,commit,strlen(commit),&stmt4,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt4);
        sqlite3_close(conn);
        return ;    
    }
    if(sqlite3_step(stmt4) != SQLITE_DONE){
        sqlite3_finalize(stmt4);
        sqlite3_close(conn);
        return ;    
    }
    sqlite3_finalize(stmt4);

    //7.为方便下一次的测试，在这里删除创建的表，否则下次运行时无法创建
    const char *drop = "drop TABLE TESTTABLE";
    sqlite3_stmt *stmt5 = nullptr;
    if(sqlite3_prepare_v2(conn,drop,strlen(drop),&stmt5,nullptr) != SQLITE_OK){
         sqlite3_finalize(stmt5);
         sqlite3_close(conn);
         return ;
     }
     if(sqlite3_step(stmt5) == SQLITE_DONE){
          cout<<"The table has been dropend"<<endl;
     }
      
     sqlite3_finalize(stmt5);
     sqlite3_close(conn);
                          


}


int main(int argc ,char ** argv){

    doTest();
    return 0;    
}
