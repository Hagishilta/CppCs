g++ -c -DBUILD_MY_DLL test2.c
g++ -shared -o test2.dll test2.o -Wl,--out-implib,libtest2.a
