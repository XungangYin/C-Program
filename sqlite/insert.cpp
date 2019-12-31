// File Name: insert.cpp
// Author: YXG
// Created Time: 2019年11月21日 星期四 15时41分24秒

#include <iostream>
#include <sqlite3.h>
#include <string.h>
#include <string>

using namespace std;

void doTest(){
    sqlite3 *cnn = nullptr;    
    //1.打开并创建数据库
    int rc = sqlite3_open("insert.db",&cnn);
  
    if(rc != SQLITE_OK){
        sqlite3_close(cnn);
        return ;    
    } 
    //数据库语句：创建表
    const char *createTable = " CREATE TABLE TESTTABLE (int_col INT,float_col REAL,string_col TEXT)";
    sqlite3_stmt *stmt  = nullptr;
    int len = strlen(createTable);
    //2.预编译数据表，若编译失败则清空内存
    if(sqlite3_prepare_v2(cnn,createTable,len,&stmt,nullptr) != SQLITE_OK){
        if(stmt)
            sqlite3_finalize(stmt);
        sqlite3_close(cnn);
        return ;
     }
    //3.通过sqlite2_step命令执行预编译好的语句，执行正确返回SQLITE_DONE;
    if(sqlite3_step(stmt) != SQLITE_DONE){
        if(stmt)
            sqlite3_finalize(stmt);
        sqlite3_close(cnn);
        return ;    
    }
    //4.释放创建表 的资源
    sqlite3_finalize(stmt);
    cout<<"Succeed to create testtable now"<<endl;
    
    
    int insertcount = 500;
    //5.构建插入数据的sqlite3_stmt对象
    const char *insertSQL = "insert into TESTTABLE VALUES(%d,%f,'%s')";
    const char *testString = "this is a test.";
    char sql[1024] = {0};
    sqlite3_stmt *stmt2 = nullptr;
    for(int i = 0;i < insertcount; ++i){
        sprintf(sql,insertSQL,i,i*1.0,testString);    
        if(sqlite3_prepare_v2(cnn,sql,strlen(sql),&stmt2,nullptr) != SQLITE_OK){
            sqlite3_finalize(stmt2);
            sqlite3_close(cnn);
            return ;    
        }
        if(sqlite3_step(stmt2) != SQLITE_DONE){
            sqlite3_finalize(stmt2);
            sqlite3_close(cnn);
            return;
        }
        cout<<"Insert Succeed"<<endl;
    } 
    
    sqlite3_finalize(stmt2);

    //6.查询数据表的内容
   /*
    const char *select = "select * from TESTTABLE";
    sqlite3_stmt * stmt4 =  nullptr;
    if(sqlite3_prepare_v2(cnn,select,strlen(select),&stmt4,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt4);
        sqlite3_close(cnn);
        return ;
    }
    if(sqlite3_step(stmt4) != SQLITE_DONE){
        sqlite3_finalize(stmt4);
        sqlite3_close(cnn);    
        return ;
    }
    */
    //7.为方便下一次的测试，在这里删除创建的表，否则下次运行时无法创建
    const char *drop = "drop TABLE TESTTABLE";
    sqlite3_stmt * stmt3 = nullptr;
    if(sqlite3_prepare_v2(cnn,drop,strlen(drop),&stmt3,nullptr) != SQLITE_OK){
        sqlite3_finalize(stmt3);
        sqlite3_close(cnn);
        return ;
    }
    if(sqlite3_step(stmt3) == SQLITE_DONE){
        cout<<"The table has been dropend"<<endl;
     }

    sqlite3_finalize(stmt3);
    sqlite3_close(cnn);
}





int main(int argc,char **argv){
    
    doTest();
    

    return 0;    
}
