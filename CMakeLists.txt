cmake_minimum_required(VERSION 3.0.0)                                   #最低cmake最低版本要求
project(cmake_template VERSION 0.1.0)                                         #设置项目名称

include(CTest)
enable_testing()

# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -pthread" )         

#设置编译器路径
set(CMAKE_CXX_COMPILER "x86_64-linux-gnu-g++")
set(CMAKE_C_COMPILER "x86_64-linux-gnu-g++")


#设置debug与release路径
set(PUBLISH_BINARY_DIR 	${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PUBLISH_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PUBLISH_BINARY_DIR}/release)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#set(CMAKE_BUILD_TYPE,Debug)


#自动查找头文件路径函数(没有去重)
macro(FIND_INCLUDE_DIR result curdir)                                   #定义函数,2个参数:存放结果result；指定路径curdir；
    file(GLOB_RECURSE children "${curdir}/*.hpp" "${curdir}/*.h" )	    #遍历获取{curdir}中*.hpp和*.h文件列表
    message(STATUS "children= ${children}")								#打印*.hpp和*.h的文件列表
    set(dirlist "")														#定义dirlist中间变量，并初始化
    foreach(child ${children})											#for循环
        string(REGEX REPLACE "(.*)/.*" "\\1" LIB_NAME ${child})			#字符串替换,用/前的字符替换/*h
        if(IS_DIRECTORY ${LIB_NAME})									#判断是否为路径
            LIST(APPEND dirlist ${LIB_NAME})							#将合法的路径加入dirlist变量中
        endif()															#结束判断
    endforeach()														#结束for循环
    set(${result} ${dirlist})											#dirlist结果放入result变量中
endmacro()																#函数结束


#查找include目录下的所有*.hpp,*.h头文件,并路径列表保存到 INCLUDE_DIR_LIST 变量中
FIND_INCLUDE_DIR(INCLUDE_DIR_LIST src)			                        #调用函数，指定参数


#将INCLUDE_DIR_LIST中路径列表加入工程		
include_directories(   													#INCLUDE_DIR_LIST路径列表加入工程
        ${INCLUDE_DIR_LIST}
        #third-party/protobuf/include                                    #添加第三方库头文件目录
)

message(STATUS "INCLUDE_DIR_LIST = ${INCLUDE_DIR_LIST}")				#打印INCLUDE_DIR_LIST中列表信息

# 查找source目录下的所有*.cpp,*.c源文件,并将文件列表保存到 DIR_LIB_SRCS 变量
file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp" "src/*.cc" "src/*.cxx" "src/*.c")	#遍历获取source/路径下所有的*.cpp、*.cc、*.cxx和*.c文件列表

message(STATUS "SRC_DIR_LIST = ${SRC_DIR_LIST}")	

add_executable(${PROJECT_NAME} ${SRC_DIR_LIST})         #生成可执行文件

#添加动态库文件，需要放在add_executable后面
target_link_libraries(
        ${PROJECT_NAME}   
       # ${PROJECT_SOURCE_DIR}/third-party/protobuf/libprotobuf-lite.so
)

include(CPack)
