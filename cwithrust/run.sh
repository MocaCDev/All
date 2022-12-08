cargo build
gcc main.c -o main -lcwithrust -L./target/debug
LD_LIBRARY_PATH=./target/debug ./main