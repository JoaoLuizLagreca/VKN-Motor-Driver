# TODO: transformas em makebuild

CC="g++"

${CC} -c -I include/ src/libmotor.cpp -o bin/libmotor.so
${CC} src/testemotor.cpp -I include/ -L bin/ -lmotor -lgpiod -o teste
