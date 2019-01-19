#include <iostream>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

int main() {

  PIX *imageData = pixRead("/home/jon/Documents/scan/scandoc2.jpg");
  tesseract::TessBaseAPI tess;
  tess.Init("./data/", "eng");
  tess.SetImage(imageData);

  mongocxx::instance instance{}; // This should be done only once.
  mongocxx::uri uri("mongodb://localhost:27017");
  mongocxx::client client(uri);
  
  mongocxx::database db = client["test"];
  mongocxx::collection collection = db["users"];
  mongocxx::cursor cursor = collection.find({});
  for(auto doc : cursor) {
    std::cout << "DB ITEM " << bsoncxx::to_json(doc) << "\n";
  }

  std::cout << "Testing! " << tess.Version() << std::endl;

  std::cout << tess.GetUTF8Text() << std::endl;

  return 0;
}
