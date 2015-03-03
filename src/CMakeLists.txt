file(
    GLOB_RECURSE
    src_files
    *.cpp
)

# Build the ss (SudokuSolver) library.
add_library(ss ${src_files})

