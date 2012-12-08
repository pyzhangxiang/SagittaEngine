

set(Source_File matrix.cc matrix2.hxx cmatrix)
source_group_by_dir(Source_File)

add_library(matclpro STATIC ${Source_File})
