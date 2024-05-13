cmake_minimum_required(VERSION 3.10)
project(ckks_project VERSION 1.0)

add_executable(encrypt src/client_encryption.cpp)
add_executable(decrypt src/client_decryption.cpp)
add_executable(evaluate src/server_evaluation.cpp)

find_package(SEAL)
target_link_libraries(encrypt SEAL::seal)
target_link_libraries(decrypt SEAL::seal)
target_link_libraries(evaluate SEAL::seal)