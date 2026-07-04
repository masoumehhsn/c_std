# uuid/CMakeLists.txt
add_library(uuid SHARED uuid.c)
target_include_directories(uuid PUBLIC ${CMAKE_SOURCE_DIR}/uuid)

# uuid draws its v4/v7 randomness from the secrets CSPRNG. MD5 (v3) and
# SHA-1 (v5) are embedded, so there is no OpenSSL dependency.
target_link_libraries(uuid secrets)
