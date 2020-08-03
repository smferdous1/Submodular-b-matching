
set(HEADER_LIST
     "${PROJECT_SOURCE_DIR}/include/graph.h"
     "${PROJECT_SOURCE_DIR}/include/matching.h"
     "${PROJECT_SOURCE_DIR}/include/input.h"
     "${PROJECT_SOURCE_DIR}/include/utility.h"

    )



# Make an automatic library - will be static or dynamic based on user setting
add_library(ra
    graph.cpp
    greedyMatching.cpp
    submodularGreedyMatching.cpp
    submodularGreedybMatching.cpp
    localLazyGreed.cpp
    shmLocalLazyGreed.cpp
    cluster.cpp
    input.cpp
    utility.cpp
    ${HEADER_LIST}
    )
#find_package(OpenMP)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(ra PUBLIC OpenMP::OpenMP_CXX)
#endif()

target_link_libraries(ra PUBLIC -qopenmp)
#link light_graph
#target_link_libraries(ra PRIVATE
# ${MKL_LIB_LINK}
#)


target_include_directories(ra PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${MKL_HOME}/include"
    )

# All users of this library will need at least C++11
target_compile_features(ra PUBLIC cxx_std_11)
target_compile_options(ra PRIVATE -O2 -qopenmp)


