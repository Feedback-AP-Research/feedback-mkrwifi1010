#include <Arduino_MKRIoTCarrier.h>
//#include <WiFiNINA.h>
#include <SD.h>
#include <SPI.h>
#include "Images.h"   //We save the images code there
#include "thingProperties.h"

MKRIoTCarrier carrier;

File file;
//bool CARRIER_CASE = true;


const byte numChars = 8;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data

int first = 0;
String inData;
boolean newData = false;

int values[200]  = {4};

int r = 0;
char ssid[11]= {'n','i','e','m','i',' ', 'h', 'o', 'u', 's', 'e'};
char pass[8] = {'n','e','m','o','9','5','6','5'};

int count = 1;  
//float temperature = 0;
//float humidity = 0;
float average = 5; 

uint32_t orange = carrier.leds.Color(50, 242, 0);


bool HEARTBEAT_ON = true;
unsigned long heartbeatMillis = 0;
unsigned long startMillis = 0;

const unsigned int backgroundColor = ST77XX_BLACK;



float calculateAverage(int cur, int list[])
 {
	double sum; 
	for(int i = 0; i < cur; i++) 
	  { 
		sum += list[cur-1];
	  };
	return sum/ cur; 
 };


void setup() {
  Serial.begin(9600);
  delay(1500);
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
 
  CARRIER_CASE = false;
  //Wait to open the Serial monitor to start the program and see details on errors
  while (!Serial);

  //Initialize the IoTSK carrier and output any errors in the serial monitor
  
    /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  // setDebugMessageLevel(2);
  // ArduinoCloud.printDebugInfo();
  
  while(ArduinoCloud.connected() != 1){
    ArduinoCloud.update();
    delay(500);
  }
  
  carrier.begin();
  startMillis = millis();
}

  carrier.display.setRotation(0);
  for (int i = 0; i < 200; i++) {
        values[i] = 0;
  }

  uint16_t time = millis();

  carrier.display.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  carrier.leds.fill(orange, 0, 5);
  carrier.leds.setBrightness(15);
  carrier.leds.show();

  
    String fv = WiFi.firmwareVersion();
  Serial.print("Firmware verison: ");
  Serial.println(fv);

    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    int status = WiFi.begin(ssid, pass);
    delay(10000);
    Serial.println(status);
    printConnectionStatus();
}


int i = 0;
int ledC = 0;
int ledA = 0;



void printConnectionStatus() {
  //configuring display, setting background color, text size and text color
  carrier.display.fillScreen(ST77XX_PTLAR); //red background
  carrier.display.setTextColor(ST77XX_WHITE); //white text
  carrier.display.setTextSize(2); //medium sized text

  carrier.display.setCursor(30, 110); //sets new position for printing (x and y)
  carrier.display.print("Status: ");
  carrier.display.println("Connected");
  carrier.display.setCursor(30, 150);
  carrier.display.println(WiFi.localIP());

}

void fileWrite(int rating) {
    file = SD.open("test.csv", FILE_WRITE);
    if (file) {
    Serial.print("Writing to test.csv...");
    file.print(rating);
    // close the file:
    file.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void fileRead() {
 if (!SD.begin(10))
  {
    Serial.println("Card failed to initialize, or not present");
 
    return;
  }
Serial.println("card initialized.");
// open the file named test.csv
File myFile = SD.open("test.csv");
if (myFile)
  {
    delay(1000);
        //static boolean recvInProgress = false;
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (myFile.available()> 0) {
        rc = myFile.read();
        if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                ndx = 0;
                strcpy(tempChars, receivedChars);
                parseData();
                showParsedData();
            }
    }
  }
}

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index
 
    strtokIndx = strtok(tempChars, ";"); // this continues where the previous call left off
    first = atoi(strtokIndx);     // convert this part to an integer
}

void showParsedData() {
    Serial.print("first ");
    Serial.println(first);
    inData = ""; // Clear recieved buffer
}

void displayInstructions() { 
  carrier.display.fillScreen(ST77XX_GREEN); //red background
  carrier.display.setTextColor(ST77XX_WHITE); //white text
  carrier.display.setTextSize(4); //medium sized text
  carrier.display.setCursor(30, 50); //sets position for printing (x and y)
  carrier.display.println(" Rate  "); //prints text
  delay(1000);
  carrier.display.println("  Yoplait "); //prints a variable
  delay(1000);
  carrier.display.println("  please "); //prints text
  delay(7000);
}

void fadeLoop() {
  carrier.leds.fill(orange, ledA, ledC);
  for ( i = 0; i < 125; i++) {
    carrier.leds.setBrightness(i);
    carrier.leds.show();
    delay(10);
  }

  for ( i = 125; i > -1; i--) {
    carrier.leds.setBrightness(i);
    carrier.leds.show();
    delay(10);
  }
  carrier.leds.clear();
  delay(500);

}

void drawStars() {
  carrier.display.drawBitmap(44, 25, star, 152, 72, 0x04B3);
}

void loop() {
  ArduinoCloud.update();
  float newAverage = calculateAverage(count, values);
  // read the sensor values
//  temperature = carrier.Env.readTemperature();
//  humidity = carrier.Env.readHumidity();

  carrier.display.fillScreen(ST77XX_BLACK);
  fadeLoop();
  carrier.display.fillScreen(ST77XX_BLACK);
  drawStars();
  fadeLoop();
   
  //Update touch buttons
  carrier.Buttons.update();
//  carrier.Buttons.updateConfig(4);

  carrier.display.fillScreen(ST77XX_RED); //red background
  carrier.display.setTextColor(ST77XX_WHITE); //white text
  carrier.display.setTextSize(6); //medium sized text
 
  
 if(count < 200) {
   if (carrier.Button0.onTouchDown()) { 
      Serial.println("button zero pressed");
      carrier.Buttons.update();
      fileWrite(1); 
      count+=1;
      values[count] = 1;
      delay(200); 
      carrier.Buzzer.sound(500); //sets frequency of buzzer
      carrier.Buzzer.noSound(); //stops buzzer
      newAverage = calculateAverage(count, values);
   } 
  if (carrier.Button1.onTouchDown()) {
      Serial.println("button one pressed"); 
      carrier.Buttons.update();
      fileWrite(2); 
      count+=1;
      values[count] = 2;
  //    newAverage = calculateAverage();
      delay(200);
      carrier.Buzzer.sound(500); //sets frequency of buzzer
      carrier.Buzzer.noSound(); //stops buzzer
      newAverage = calculateAverage(count, values);
   }
  if (carrier.Button2.onTouchDown()) { 
      Serial.println("button two pressed");
      carrier.Buttons.update();
      fileWrite(3); 
      count+=1;
      values[count] = 3;
    //  newAverage = calculateAverage();
      delay(200);
      carrier.Buzzer.sound(500); //sets frequency of buzzer
      carrier.Buzzer.noSound(); //stops buzzer
      newAverage = calculateAverage(count, values);
   }
  if (carrier.Button3.onTouchDown()) { 
      Serial.println("button three pressed");
      carrier.Buttons.update();
      fileWrite(4); 
      count+=1;
      values[count] = 4;
      //newAverage = calculateAverage();
      delay(200);
      carrier.Buzzer.sound(500); //sets frequency of buzzer
      carrier.Buzzer.noSound(); //stops buzzer
      newAverage = calculateAverage(count, values);
   }
  if (carrier.Button4.onTouchDown()) { 
      Serial.println("button four pressed");
      carrier.Buttons.update();
      fileWrite(5); 
      count+=1;
      values[count] = 5;
      //newAverage = calculateAverage();
      delay(200);
      carrier.Buzzer.sound(500); //sets frequency of buzzer
      carrier.Buzzer.noSound(); //stops buzzer
      newAverage = calculateAverage(count, values);
   }
   Serial.println(count);
};
  
  carrier.display.setCursor(60, 10); //sets position for printing (x and y)
  carrier.display.print(newAverage); //prints text
  // print each of the sensor values
//  Serial.print("Temperature = ");
//  Serial.print(temperature);
//  Serial.println(" °C");
  
  displayInstructions();
  Serial.print("Average = "); 
  Serial.print(newAverage);
  Serial.println(" %");

  
  carrier.display.fillScreen(ST77XX_RED); //red background
  carrier.display.setTextColor(ST77XX_WHITE); //white text
  carrier.display.setTextSize(7); //medium sized text
  carrier.display.setCursor(60, 80); //sets position for printing (x and y)
  carrier.display.println(roundf(newAverage*10)/10, 1); //prints text ✭
  //carrier.display.println("✭✭✭✭"); //prints text
 delay(6000);
  //function to print out values
  // if (carrier.Button0.onTouchDown()) {
//    printTemperature();
  // }

  // if (carrier.Button1.onTouchDown()) {
//    printHumidity();
  // }
}

void onProductChange() {
    updateMessages();
}

void onRatingChange() {
    updateMessages();
}

void onDayChange() {
    updateMessages();
}
