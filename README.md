
Compilation

    g++ src/main.cpp -o bin/main `pkg-config --cflags --libs tesseract lept libmongocxx libbsoncxx`

Execution

    bin/main
