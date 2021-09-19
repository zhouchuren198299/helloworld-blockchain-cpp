cmake_minimum_required(VERSION 3.0)
project(helloworld_blockchain_cpp)

set(CMAKE_CXX_STANDARD 14)
set(project_name "helloworld_blockchain_cpp")


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(
        ${project_name}
        main.cpp
        crypto/Sha256Util.cpp crypto/Sha256Util.h crypto/Ripemd160Util.cpp crypto/Ripemd160Util.h crypto/ByteUtil.cpp crypto/ByteUtil.h crypto/Base58Util.cpp crypto/Base58Util.h crypto/AccountUtil.cpp crypto/AccountUtil.h crypto/MerkleTreeUtil.cpp crypto/MerkleTreeUtil.h
        util/MathUtil.cpp util/MathUtil.h util/FileUtil.cpp util/FileUtil.h util/NetUtil.cpp util/NetUtil.h util/OperateSystemUtil.cpp util/OperateSystemUtil.h util/SystemUtil.cpp util/SystemUtil.h util/ThreadUtil.cpp util/ThreadUtil.h util/TimeUtil.cpp util/TimeUtil.h util/StringUtil.cpp util/StringUtil.h util/LogUtil.cpp util/LogUtil.h util/JsonUtil.cpp util/JsonUtil.h util/KvDbUtil.cpp util/KvDbUtil.h
        core/tool/BlockTool.cpp core/tool/BlockTool.h core/tool/BlockDtoTool.cpp core/tool/BlockDtoTool.h core/tool/TransactionDtoTool.cpp core/tool/TransactionDtoTool.h core/tool/ScriptDtoTool.cpp core/tool/ScriptDtoTool.h
        core/model/model.h
        dto/dto.cpp dto/dto.h
)

include_directories(thirdpart/openssl/include thirdpart/leveldb/include)

if(WIN32)
    target_link_libraries(
            ${project_name}
            ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/windows/x86/libcrypto.lib ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/windows/x86/libssl.lib
            ${CMAKE_SOURCE_DIR}/thirdpart/leveldb/lib/windows/x86/leveldb.lib
    )
    file(COPY ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/windows/x86/libcrypto-1_1.dll DESTINATION ${CMAKE_BUILD_DIRECTORY}/)
    file(COPY ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/windows/x86/libssl-1_1.dll DESTINATION ${CMAKE_BUILD_DIRECTORY}/)
endif()

if(APPLE)
    target_link_libraries(${project_name} ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/mac/libcrypto.1.1.dylib ${CMAKE_SOURCE_DIR}/thirdpart/openssl/lib/mac/libssl.1.1.dylib)
endif()
