cmake_minimum_required(VERSION 3.0.0)                                   #最低cmake最低版本要求



####################################### 编译模式设置 #######################################
set(DEBUG_VERSION "DebugMode")
set(RELEASE_VERSION "ReleaseMode")
set(COMPILE_MODE ${DEBUG_VERSION})              # 编译模式
set(TARGET_NAME "cmake_template")                   # 目标可执行程序名称
string(TIMESTAMP COMPILE_TIME %m%d_%H%M)        # 生成编译时间

# Debug模式（目标：工程名+编译时间）
if(${COMPILE_MODE} MATCHES ${DEBUG_VERSION})   
    message(STATUS "Compile mode: ${COMPILE_MODE}")
    set(USE_RELEASE_MODE 0)
    set(CMAKE_BUILD_TYPE "Debug")
    set(VERSION_MAJOR  0)    # 一级版本号
    set(VERSION_MINOR  0)    # 二级版本号
    set(VERSION_LEVEL3 1)    # 三级版本号
    set(target ${TARGET_NAME}_debug)
   # set(target ${TARGET_NAME}_${COMPILE_TIME})
# Release模式（目标：工程名+版本号）
else()
    message(STATUS "Compile mode: ${COMPILE_MODE}")
    set(USE_RELEASE_MODE 1)
    set(CMAKE_BUILD_TYPE "Release") 
    set(VERSION_MAJOR  0)    # 一级版本号
    set(VERSION_MINOR  0)    # 二级版本号
    set(VERSION_LEVEL3 1)    # 三级版本号
    set(target ${TARGET_NAME}_V${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_LEVEL3})
endif()
project(${target})

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "config.h.in"
  "../config.h"
)

include(CTest)
enable_testing()



#设置debug与release路径
set(PUBLISH_BINARY_DIR 	${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PUBLISH_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PUBLISH_BINARY_DIR}/release)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#set(CMAKE_BUILD_TYPE,Debug)

####################################### 编译选项配置 #######################################
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_C_FLAGS "-fdata-sections -std=gnu99 -g -rdynamic  -Wall -pthread")
set(CMAKE_CXX_FLAGS "-fdata-sections -Wno-psabi") 


#定义宏与编译选项控制
add_definitions (-DDEV_A)    
set(DEV_A ON)


#设置编译器路径
#set(CMAKE_CXX_COMPILER "x86_64-linux-gnu-g++")
#set(CMAKE_C_COMPILER "x86_64-linux-gnu-g++")


####################################### 头文件 ############################################

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

list(APPEND INCLUDE_DIR_LIST config.h)

#将INCLUDE_DIR_LIST中路径列表加入工程		
include_directories(   													#INCLUDE_DIR_LIST路径列表加入工程
        ${INCLUDE_DIR_LIST}
        #3party/zlog/include                                        #添加第三方库头文件目录
)

message(STATUS "INCLUDE_DIR_LIST = ${INCLUDE_DIR_LIST}")				#打印INCLUDE_DIR_LIST中列表信息

####################################### 源文件 ############################################

# 查找source目录下的所有*.cpp,*.c源文件,并将文件列表保存到 DIR_LIB_SRCS 变量
file(GLOB_RECURSE SRC_FILE_LIST "src/*.cpp" "src/*.cc" "src/*.cxx" "src/*.c")	#遍历获取source/路径下所有的*.cpp、*.cc、*.cxx和*.c文件列表

# 选择编译
if(DEV_A)
    file(GLOB_RECURSE REMOVE_SRCS "dev_a.c")
else()
     file(GLOB_RECURSE REMOVE_SRCS "dev_b.c")
endif()
list(REMOVE_ITEM SRC_FILE_LIST ${REMOVE_SRCS})

message(STATUS "SRC_FILE_LIST = ${SRC_FILE_LIST}")	

######################################## 生成 ############################################

message(STATUS "################## Compile time:${COMPILE_TIME} ##################")

add_executable(${PROJECT_NAME} ${SRC_FILE_LIST})         #生成可执行文件

#添加动态库文件，需要放在add_executable后面
target_link_libraries(
        ${PROJECT_NAME}   
        #${PROJECT_SOURCE_DIR}/3rdparty/zlog/libsam9x60/libzlog.so.1
)

include(CPack)
