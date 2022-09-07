
#executable greedyMatching
add_executable(greedyMatching greedy_matching_test.cpp)
target_compile_features(greedyMatching PRIVATE cxx_std_11)
target_compile_options(greedyMatching PRIVATE -O3 )
target_link_libraries(greedyMatching PRIVATE ra)
target_include_directories(greedyMatching PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable submodular greedyMatching
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

#executable lb
add_executable(lb load_balance_test.cpp)
target_compile_features(lb PRIVATE cxx_std_11)
target_compile_options(lb PRIVATE -O3 )
target_link_libraries(lb PRIVATE ra)
target_include_directories(lb PRIVATE "${PROJECT_SOURCE_DIR}/include")

#executable lb_sim
add_executable(lb_sim load_balance_test_simple.cpp)
target_compile_features(lb_sim PRIVATE cxx_std_11)
target_compile_options(lb_sim PRIVATE -O3 )
target_link_libraries(lb_sim PRIVATE ra)
target_include_directories(lb_sim PRIVATE "${PROJECT_SOURCE_DIR}/include")
