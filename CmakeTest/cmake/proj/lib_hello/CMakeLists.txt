#为该目录下所有文件其别名为SRC
aux_source_directory(./ SRC)

#生成动态链接库，库名字叫做hello_shared, SHARED是动态库的标记符号，${SRC}表示取SRC所包含的所有文件
add_library(hello_shared SHARED ${SRC})

#同理，生成静态链接库，动态库和静态库的名字不能重名
add_library(hello_static STATIC ${SRC})

#设置生成库的名称
set_target_properties(hello_static PROPERTIES OUTPUT_NAME "hellos")
set_target_properties(hello_shared PROPERTIES OUTPUT_NAME "hellod")

#设置动态库的版本号
set_target_properties(hello_shared PROPERTIES VERSION 1.1 SOVERSION 1)

#设置库文件路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
