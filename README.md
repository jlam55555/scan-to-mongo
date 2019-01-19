# scan-to-mongo

Scan PDFs directly into searchable text Mongo database using Tesseract's OCR

---

### Compilation

    g++ src/main.cpp -o bin/main `pkg-config --cflags --libs tesseract lept libmongocxx libbsoncxx`

---

### Execution

    bin/main
