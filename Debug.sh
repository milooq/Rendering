mkdir -p bin
mkdir -p obj
mkdir -p obj/Debug
make --directory=./obj/Debug --makefile=../../Makefile build_flags="-O0 -g3 -D_DEBUG" program_name=../../bin/application_debug link_flags="-pipe -std=c++17"