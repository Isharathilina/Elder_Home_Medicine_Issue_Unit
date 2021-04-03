

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <LiquidCrystal.h>

#define FIREBASE_HOST "https://caregiver-face-detection.firebaseio.com/"  // firebae host address
#define FIREBASE_AUTH "3gEHztcJ1osS0WByAEYSteDthL0pbGrM49Zm1WpE"          // db secret key
#define WIFI_SSID "Fed Geek"
#define WIFI_PASSWORD "11002299"

// LCD config
const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;   
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

//Define FirebaseESP8266 data object
FirebaseData fbdo;
FirebaseJson json;

void printResult(FirebaseData &data);
String path = "/IOT";
int ledPin=9;

void setup()
{
  pinMode(D0, OUTPUT); //m1
  pinMode(D1, OUTPUT); // m2
  pinMode(D4, OUTPUT); //m3
  pinMode(ledPin, OUTPUT);    // for LED
  digitalWrite(ledPin, LOW);   // off LED


  Serial.begin(115200);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("....Welcome....");
  delay(1000);
  lcd.clear();

  lcd.print("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  lcd.clear();
  lcd.print("Connected !");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  fbdo.setBSSLBufferSize(1024, 1024);
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");
  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);
  
  Serial.println("Set default tablet values...");
  lcd.clear();
  lcd.print("Connecting DB..");

  for (uint8_t i = 0; i < 3; i++)
  {
    //Also can use Firebase.set instead of Firebase.setDouble
    if (Firebase.setDouble(fbdo, path + "/Tablet" + (i + 1), 1  )  )
    {
      Serial.println("PASSED");
      lcd.setCursor(0, 1);
      lcd.print("OK");

    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
      lcd.setCursor(0, 1);
      lcd.print("Fail.!");
    }
    
  }
  Firebase.setDouble(fbdo, path + "/Run", 0  );  // set run value



  


}


void runMoter1(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D0, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                     
    digitalWrite(D0, LOW);
    delay(500);                     
  }
}

void runMoter2(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D1, HIGH);   
    delay(500);                     
    digitalWrite(D1, LOW);
    delay(500);                     
  }
}

void runMoter3(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D4, HIGH);   
    delay(500);                     
    digitalWrite(D4, LOW);
    delay(500);                     
  }
}


void printTabletData()
{
  
  Serial.println("------------------------------------");
  Serial.println("Get double test...");

  for (uint8_t i = 0; i < 3; i++)
  {
    //Also can use Firebase.get instead of Firebase.setInt
    if (Firebase.getInt(fbdo, path + "/Tablet" + (i + 1)))
    {
      Serial.println("PASSED");
     // Serial.println("PATH: " + fbdo.dataPath());
    //  Serial.println("TYPE: " + fbdo.dataType());
    //  Serial.println("ETag: " + fbdo.ETag());
      Serial.print("VALUE: ");
      printResult(fbdo);
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  }

}


void printResult(FirebaseData &data)
{

  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
  {
    Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println();
    Serial.println("Iterate JSON data:");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        Serial.print(", Key: ");
        Serial.print(key);
      }
      Serial.print(", Value: ");
      Serial.println(value);
    }
    json.iteratorEnd();
  }
  else if (data.dataType() == "array")
  {
    Serial.println();
    //get array data from FirebaseData using FirebaseJsonArray object
    FirebaseJsonArray &arr = data.jsonArray();
    //Print all array values
    Serial.println("Pretty printed Array:");
    String arrStr;
    arr.toString(arrStr, true);
    Serial.println(arrStr);
    Serial.println();
    Serial.println("Iterate array values:");
    Serial.println();
    for (size_t i = 0; i < arr.size(); i++)
    {
      Serial.print(i);
      Serial.print(", Value: ");

      FirebaseJsonData &jsonData = data.jsonData();
      //Get the result data from FirebaseJsonArray object
      arr.get(jsonData, i);
      if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
        Serial.println(jsonData.boolValue ? "true" : "false");
      else if (jsonData.typeNum == FirebaseJson::JSON_INT)
        Serial.println(jsonData.intValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
        Serial.println(jsonData.floatValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
        printf("%.9lf\n", jsonData.doubleValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
               jsonData.typeNum == FirebaseJson::JSON_NULL ||
               jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
               jsonData.typeNum == FirebaseJson::JSON_ARRAY)
        Serial.println(jsonData.stringValue);
    }
  }
  else if (data.dataType() == "blob")
  {

    Serial.println();

    for (int i = 0; i < data.blobData().size(); i++)
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      if (i < 16)
        Serial.print("0");

      Serial.print(data.blobData()[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  else if (data.dataType() == "file")
  {

    Serial.println();

    File file = data.fileStream();
    int i = 0;

    while (file.available())
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      int v = file.read();

      if (v < 16)
        Serial.print("0");

      Serial.print(v, HEX);
      Serial.print(" ");
      i++;
    }
    Serial.println();
    file.close();
  }
  else
  {
    Serial.println(data.payload());
  }
}

void loop()
{
  lcd.clear();
  lcd.print("Running..");

  double runData;
  Firebase.getInt(fbdo, path + "/Run");  // get run value
  runData = fbdo.doubleData();
  Serial.println(runData);
   //printResult(fbdo);
  if(runData==1.0)
  {
    //printTabletData();
     lcd.clear();
     lcd.print("Data Received");

    double tb1Data;
    Firebase.getInt(fbdo, path + "/Tablet1");  // get run value
    tb1Data = fbdo.doubleData();
    Serial.println(tb1Data);

    double tb2Data;
    Firebase.getInt(fbdo, path + "/Tablet2");  // get run value
    tb2Data = fbdo.doubleData();
    Serial.println(tb2Data);

    double tb3Data;
    Firebase.getInt(fbdo, path + "/Tablet3");  // get run value
    tb3Data = fbdo.doubleData();
    Serial.println(tb3Data);
    
    digitalWrite(ledPin, HIGH);   // on LED
    lcd.clear();
    lcd.print("TB1="+String(int(tb1Data)));
    lcd.print("  TB2="+String(int(tb2Data)));
    lcd.setCursor(0, 1);
    lcd.print("TB3="+String(int(tb3Data)));
    lcd.print(" Confirm?");

    int buttonval=0;
    while(buttonval<800)
    {
      
      buttonval = analogRead(A0);   // read the input (RX) pin
      Serial.println("Input val = "+String(buttonval));
      delay(100);

    }

    lcd.clear();
    lcd.print("Confirmed");
    lcd.setCursor(0, 1);
    lcd.print("Processing Start");
    delay(1000);



    // run moters
    runMoter1(int(tb1Data));
    runMoter2(int(tb2Data));
    runMoter3(int(tb3Data));

    Firebase.setDouble(fbdo, path + "/Run", 0  );  // set run value 0
    lcd.clear();
    lcd.print("Done");
    digitalWrite(ledPin, LOW);   // off LED
    delay(2000);

  }



   delay(500);
}
