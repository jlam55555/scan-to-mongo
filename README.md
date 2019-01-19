# scan-to-mongo

Scan PDFs directly into searchable text Mongo database using Tesseract's OCR

Currently in the works.

---

### Compilation

This requires the `tesseract`, `leptonica`, `libmongocxx`, and `libbsoncxx` libraries to be installed.

    g++ src/main.cpp -o bin/main `pkg-config --cflags --libs tesseract lept libmongocxx libbsoncxx`

---

### Execution

    bin/main
