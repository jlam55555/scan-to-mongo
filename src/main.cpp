#include <iostream>
#include <fstream>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <sane/sane.h>
#include <sane/saneopts.h>

int main() {

  unsigned char data[50000];
  int maxLength = 50000;
  int length;

  std::ofstream outFile;
  outFile.open("./out/test.pnm");
  //printf ("P6\n# SANE data follows\n%d %d\n%d\n", width, height, (depth <= 8) ? 255 : 65535);
  
  // sane_strstatus to print status
  SANE_Handle handle;
  SANE_Parameters parm;
  SANE_Status openStatus = sane_open("pixma:04A91735_10C369", &handle);
  SANE_Status paramStatus = sane_get_parameters(handle, &parm);
  SANE_Status readStatus;

  const SANE_Option_Descriptor *opt = sane_get_option_descriptor(handle, 0);
  std::cout << "TESTING 1 " << opt->name << " END NAME" << std::endl;
  if(opt == NULL) {
    std::cout << "NULL" << std::endl;
  } else {
    std::cout << "NOT NULL" << std::endl;
    int numOpts;
    sane_control_option(handle, 0, SANE_ACTION_GET_VALUE, &numOpts, 0);
    std::cout << "Number of options: " << numOpts << std::endl;

    for(int i = 0; i < numOpts; i++) {
      opt = sane_get_option_descriptor(handle, i);
      if(SANE_OPTION_IS_SETTABLE(opt->cap)) {
        std::cout << opt->type << "\t" << opt->title << "\t" << opt->name << "\t" << opt->desc << "\t" << opt->size << "\t" << opt->unit << std::endl;
      
        if(strcmp(opt->title, SANE_TITLE_SCAN_RESOLUTION) == 0) {
          int val = 600, info;
          std::cout << sane_strstatus(sane_control_option(handle, i, SANE_ACTION_SET_VALUE, &val, &info)) << val << info << std::endl;
        }
        if(strcmp(opt->title, SANE_TITLE_SCAN_SOURCE) == 0) {
          const char *val = "Automatic Document Feeder";
          int info;
          std::cout << sane_strstatus(sane_control_option(handle, i, SANE_ACTION_SET_VALUE, (void *) val, &info)) << val << info << std::endl;
        }
        /*if(opt->type == SANE_TYPE_FIXED || opt->type == SANE_TYPE_INT) {
          int val, info;
          sane_control_option(handle, i, SANE_ACTION_GET_VALUE, &val, &info);
          std::cout << val << "\t" << info << std::endl;
        }
        std::cout << std::endl;*/
      }
    }
  }

  outFile << "P6\n# SANE data follows\n" << parm.pixels_per_line << " " << parm.lines << "\n" << ((parm.depth <= 8) ? 255 : 65535) << "\n";

  SANE_Status startStatus = sane_start(handle);
  do {
    readStatus = sane_read(handle, data, maxLength, &length);
    outFile.write((const char *) data, length);
  } while(readStatus == SANE_STATUS_GOOD);
  sane_close(handle);
  outFile.close();
  std::cout << sane_strstatus(openStatus) << std::endl;
  std::cout << sane_strstatus(paramStatus) << std::endl;
  std::cout << sane_strstatus(startStatus) << std::endl;
  std::cout << sane_strstatus(readStatus) << " " << length << std::endl;

  PIX *imageData = pixRead("./out/test.pnm");
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
