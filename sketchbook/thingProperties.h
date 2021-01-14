#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "55b9595d-5a42-4941-b914-152e74c33585";

const char SSID[11]     = {'n', 'i', 'e', 'm', 'i', ' ', 'h', 'o', 'u', 's', 'e'};    // Network SSID (name)
const char PASS[8]     = {'n', 'e', 'm', 'o', '9', '5, '6', '5'};    // Network password (use for WPA, or use as key for WEP)

void onRatingChange();
void onProductChange();
void onDayChange();

Floating point rating;
String product;
int day = 0;

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(product, READWRITE, ON_CHANGE, onProductChange);
  ArduinoCloud.addProperty(rating, READWRITE, ON_CHANGE, onRatingChange);
  ArduinoCloud.addProperty(day, READWRITE, ON_CHANGE, onDayChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

