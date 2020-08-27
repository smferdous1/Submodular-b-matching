
#executable greedyMatching
add_executable(greedyMatching greedy_matching_test.cpp)
target_compile_features(greedyMatching PRIVATE cxx_std_11)
target_compile_options(greedyMatching PRIVATE -O3 )
target_link_libraries(greedyMatching PRIVATE ra)
target_include_directories(greedyMatching PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable greedyMatching
add_executable(greedySubMatching submodular_greedy_matching_test.cpp)
target_compile_features(greedySubMatching PRIVATE cxx_std_11)
target_compile_options(greedySubMatching PRIVATE -O3 )
target_link_libraries(greedySubMatching PRIVATE ra)
target_include_directories(greedySubMatching PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable locallazygreedy
add_executable(llg submodular_local_greedy_matching_test.cpp)
target_compile_features(llg PRIVATE cxx_std_11)
target_compile_options(llg PRIVATE -O3 )
target_link_libraries(llg PRIVATE ra)
target_include_directories(llg PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable shmlocallazygreedy
add_executable(shmllg shm_submodular_local_greedy_matching_test.cpp)
target_compile_features(shmllg PRIVATE cxx_std_11)
target_compile_options(shmllg PRIVATE -O2 )
target_link_libraries(shmllg PRIVATE ra)
target_include_directories(shmllg PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable testinput
add_executable(testinput test_input.cpp)
target_compile_features(testinput PRIVATE cxx_std_11)
target_compile_options(testinput PRIVATE -O2 )
target_link_libraries(testinput PRIVATE ra)
target_include_directories(testinput PRIVATE "${PROJECT_SOURCE_DIR}/include")
