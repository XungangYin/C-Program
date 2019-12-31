// File Name: learn.cpp
// Author: YXG
// Created Time: 2019年11月21日 星期四 10时00分33秒
//g++ learn.cpp -lsqlite3

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <string.h>
#include <algorithm>
using namespace std;

void doTest(){
    sqlite3 *conn = NULL;
    //1.打开/或者创建一个 数据库
    int result  = sqlite3_open("test.db",&conn);
    if(result != SQLITE_OK){
        sqlite3_close(conn);
        return ;    
    }           
    const char * createTableSQL = 
        "CREATE TABLE YXGTABLE(int_col INT,float_col REAL,string_col TEXT ) ";
    //sqlit3_stmt函数用来获取数据库数据
    sqlite3_stmt * stmt = NULL;
    int len = strlen(createTableSQL);
    //2.准备创建数据表，如果创建失败，需要用sqlite3_finalize释放sqlite3_stmt对象，防止内存泄露
	//sqlite3_prepare_v2函数将sql文本转换为SQL语句对象
    if(sqlite3_prepare_v2(conn,createTableSQL,len,&stmt,NULL) != SQLITE_OK){
        if(stmt)
            sqlite3_finalize(stmt);
        sqlite3_close(conn);    
        return ;
    }
    //3.通过sqlite3_step命令执行创建表的语句。
    if(sqlite3_step(stmt) != SQLITE_DONE){
        sqlite3_finalize(stmt);
        sqlite3_close(conn);
        return ;
    }
    
    //4.释放创建表语句对象的资源
    sqlite3_finalize(stmt);
    printf("Succeed to create table now. \n");
    //5.构造查询表数据的sqlite3_stmt对象
    const char *selectSQL = "SELECT * FROM YXGTABLE WHERE 1= 0";
    sqlite3_stmt *stmt2 = NULL;
    if(sqlite3_prepare_v2(conn,selectSQL,strlen(selectSQL),&stmt2,NULL) != SQLITE_OK){
        if(stmt2)
            sqlite3_finalize(stmt2);
        sqlite3_close(conn);
        return ;
    }
    
    //6.根据select语句获取结果集中的字段数量
    int fieldCount = sqlite3_column_count(stmt2);
    cout<<"The column count is :"<<fieldCount<<endl;
   //7.遍历结果集中每个字段meta信息，并获取其声明时的类型.
   for(int i = 0; i < fieldCount; ++i){
        //由于此时Table中并不存在数据，而且SQLLITE数据类型本本省是动态的，所以在没有数据时无法通过sqlite3_column_type获取，此时sqlite3_column_type只是返回SQLITE——NULL，直到有数据时才返回具体类型。因此，这里使用sqlite3_column_decltype函数来获取声明时的类型.
        string stype = sqlite3_column_decltype(stmt2,i);
        cout<<"转换前："<<stype <<endl;
       // stype = strlwr((char *)stype.c_str()); //将字符串参数转换为小写,该函数只能在vc中
       transform(stype.begin(),stype.end(),stype.begin(),::tolower); 
        cout<<"转换后："<<stype<<endl;
		//string::npos本质是一个常数-1.用于判断
        if( stype.find("int") != string::npos){
            printf("The type of %dth column is INTEGER.\n",i);    
        }else if(stype.find("char") != string::npos || stype.find("text") != string::npos){
            printf("The type of %dth column is TEST.\n",i);
        }else if(stype.find("real") != string::npos 
            || stype.find("floa") != string::npos
            || stype.find("doub")!= string::npos){
            printf("The type of %dth column is DOUBLE.\n",i);    
        }
    } 
    sqlite3_finalize(stmt2);
    //8.为方便下次测试运行，在这里删除该函数创建的表，否则下次运行时无法创建，因为它已经存在
    const char *dropSQL = "drop table YXGTABLE";
    sqlite3_stmt *stmt3 = NULL;
    if(sqlite3_prepare_v2(conn,dropSQL,strlen(dropSQL),&stmt3,NULL) != SQLITE_OK){
        if(stmt3)
            sqlite3_finalize(stmt3);
        sqlite3_close(conn);
        return ;
    }
    if(sqlite3_step(stmt3) == SQLITE_DONE){
        cout<<"The YXGTABLE has been droped."<<endl;    
    }
    sqlite3_finalize(stmt3);
    sqlite3_close(conn);

    
}



int main(int argc,char **argv){

    doTest();

    return 0;   
}
