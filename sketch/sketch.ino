#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
 
#define SCREEN_WIDTH 128  // OLED display ANCHO, in pixels
#define SCREEN_HEIGHT 64  // OLED display ALTO, in pixels

#define OLED_RESET -1        //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Por defecto se utilizan estos pines con la libreria
#define rxPin 4
#define txPin 3

// Definimos el Pin se salida
const int ledPIN = 9;

SoftwareSerial neogps(rxPin, txPin);
TinyGPSPlus gps;

// Control de datos de GPS
boolean hayDatosGPS;

#define latitudDestino 60.265462
#define longitudDestino -0.852455

void setup() {
  //Begin serial communication Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  //Begin serial communication Neo6mGPS
  neogps.begin(9600);
  //Declaramos variable global a false
  hayDatosGPS = false;
  // Declaramos el LED

  // Declaramos el LED
  pinMode(ledPIN, OUTPUT);  //definir pin como salida
  // SSD1306_SWITCHCAPVCC, inicializa la imagen en la pantalla oled indefinidamente
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    // Bucle necesario para inicializacion
    for (;;)
      ;
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
}

void loop() {

  // Limpio la pantalla
  display.clearDisplay();

  for (unsigned long start = millis(); millis() - start < 1000;) {
    if (neogps.available() && gps.encode(neogps.read()) && gps.location.isValid() == 1) {
      hayDatosGPS = true;
    }
  }

  if (hayDatosGPS == true) {
    hayDatosGPS = false;
    pintaCoordenadas();
  } else {
    pintaSinDatos();
  }
}

void pintaCoordenadas() {
  if (gps.location.isValid() == 1) {
    display.setTextSize(0);
    display.clearDisplay();
    double distanciaToDestino = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), latitudDestino, longitudDestino);

    //Distancia hasta destino
    pintaDistanciaHastaDestino(distanciaToDestino);
    //Latitud actual del gps
    pintaLatitud(gps.location.lat());
    //Longitud actual del gps
    pintaLongitud(gps.location.lng());
    //Latitud destino del gps
    pintaLatitudDestino(latitudDestino);
    //Longitud destino del gps
    pintaLongitudDestino(longitudDestino);
    // Pinto en pantalla
    display.display();

    if (distanciaToDestino <= 15) {
      digitalWrite(ledPIN, HIGH);
    } else {
      digitalWrite(ledPIN, LOW);
    }
  }
}

void pintaSinDatos() {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.setTextSize(2);
  display.print(F("SIN DATOS"));
  display.display();
}

void pintaLatitud(double latitud) {
  display.setCursor(0, 0);
  display.println(F("Lat. :"));
  display.setCursor(60, 0);
  display.println(latitud, 6);
}

void pintaLatitudDestino(double latitud) {
  display.setCursor(0, 25);
  display.println(F("Lat. :"));
  display.setCursor(60, 25);
  display.println(latitud, 6);
}

void pintaLongitud(double longitud) {
  display.setCursor(0, 8);
  display.println(F("Lon. :"));
  display.setCursor(60, 8);
  display.println(longitud, 6);
}

void pintaLongitudDestino(double longitud) {
  display.setCursor(0, 34);
  display.println(F("Lon. :"));
  display.setCursor(60, 34);
  display.println(longitud, 6);
}

void pintaDistanciaHastaDestino(double distancia) {
  display.setCursor(0, 50);
  display.println(F("Metros: "));
  display.setCursor(45, 50);
  display.println(distancia, 2);
}