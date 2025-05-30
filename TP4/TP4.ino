//Grupo 1  Octaviano Sznajdleder,Roman Otero, Adam Bairros 


#include <Preferences.h>
#include <U8g2lib.h>
#include "DHT.h"

Preferences preferences;

/* ----------------------------------------------------------------------------------------------------- */

// Definicion de constructores y variables globales

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/* ----------------------------------------------------------------------------------------------------- */
void printBMP_OLED(void );
void printBMP_OLED2(void) ;

#define BOTON1 34
#define BOTON2 35
#define P1 0
#define P2 1
#define RST 20
#define ESPERA1 2
#define ESPERA2 3
#define AUMENTAR 4
#define RESTAR 5
int estado = RST;
#define LED 25
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp;
int valorU;
int millis_valor;
int millis_actual;
int millis_valor_temp;
int millis_actual_temp;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON1, INPUT);
  pinMode(BOTON2, INPUT);
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  u8g2.begin();
  dht.begin();
  preferences.begin("memoria", false);
  valorU = preferences.getUInt("valorUmbral", 23);     


}

void loop() {

  millis_actual_temp = millis();
  if (millis_actual_temp - millis_valor_temp >= 2000) {
    temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    if (temp >= valorU) {
      digitalWrite(LED, HIGH);
    }
    if (temp < valorU) {
      digitalWrite(LED, LOW);
    }
    millis_valor_temp = millis();
  }
  switch (estado) {
    case RST:

      millis_valor = millis();
      estado = P1;
      break;

    case P1:
      Serial.println("p1");

      printBMP_OLED();
      if (digitalRead(BOTON1) == LOW) {
        Serial.println("boton 1 pres");
        millis_valor = millis();
        estado = ESPERA1;
      }

      break;
    case ESPERA1:
      millis_actual = millis();

      if (millis_actual - millis_valor  >= 5000) {
        Serial.println("pasaron 5 seg");
        estado = P2;
      }
      if (digitalRead(BOTON1) == HIGH) {
        Serial.println("boton 1  soltado");
        estado = P1;
      }
      break;
    case P2:

      if (digitalRead(BOTON1) == HIGH) {
        Serial.println("P2");

        printBMP_OLED2();
        if (digitalRead(BOTON1) == LOW) {
          Serial.println("AUMENTAR");

          estado = AUMENTAR;
        }
        if (digitalRead(BOTON2) == LOW) {
          millis_valor = millis();
          estado = RESTAR;
        }

      }

      break;
    case ESPERA2:

      if (digitalRead(BOTON2) == HIGH) {
        preferences.putUInt("valorUmbral", valorU);
        Serial.print("Se guardo valor umbral: ");
        Serial.println(valorU);

        estado = P1;
      }

      break;

    case AUMENTAR:


      if (digitalRead(BOTON1) == HIGH) {
        valorU = valorU + 1;
        estado = P2;
      }


      break;

    case RESTAR:

      millis_actual = millis();
      if (millis_actual - millis_valor >= 5000)
      {
        estado = ESPERA2;
      }


      if (digitalRead(BOTON2) == HIGH) {
        valorU = valorU - 1;
        estado = P2;
      }


      break;
  }



}



void printBMP_OLED(void) {
  char stringU[5];
  char stringtemp[5];
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_t0_11b_tr); // choose a suitable font
  sprintf (stringtemp, "%.2f" , temp); ///convierto el valor float a string
  sprintf (stringU, "%d" , valorU); ///convierto el valor float a string
  u8g2.drawStr(0, 35, "T. Actual:");
  u8g2.drawStr(60, 35, stringtemp);
  u8g2.drawStr(90, 35, "°C");
  u8g2.drawStr(0, 50, "V. Umbral:");
  u8g2.drawStr(60, 50, stringU);
  u8g2.drawStr(75, 50, "°C");
  u8g2.sendBuffer();          // transfer internal memory to the display
}

void printBMP_OLED2(void) {
  char stringU[5];
  u8g2.clearBuffer();          // clear the internal memory
  sprintf (stringU, "%d" , valorU);
  u8g2.setFont(u8g2_font_t0_11b_tr); // choose a suitable font
  u8g2.drawStr(0, 50, "V. Umbral:");
  u8g2.drawStr(60, 50, stringU);
  u8g2.drawStr(75, 50, "°C");
  u8g2.sendBuffer();          // transfer internal memory to the display
}
