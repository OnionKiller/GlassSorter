cmake_minimum_required(VERSION 3.14)

set(Header_Files
    "Glass.h"
    "SorterSolver.h"
    "SortingProblemSolution.h"
    "SortingProblemState.h"
    "StateProvider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "main.cpp"
    "SorterSolver.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(GlassSorter ${ALL_FILES})
