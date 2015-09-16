add_library (cnum CNum/type.cpp Counter.cpp)
add_executable(main main.cpp)

find_library(libX11 Xaw Xmu Xt Xext png z m /usr/X11/lib)
find_library(libXaw Xaw /usr/X11/lib)

target_link_libraries(main ntl plotter ${libX11} ${libXaw})
