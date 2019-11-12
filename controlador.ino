// ************************ Controlador DMX con Arduino ************************
//
// El programa lee la entrada de cuatro botones pulsadores que determinan el
// canal a controlar (R, G, B o brillo) y de un potenciómetro, que establece
// el nivel del canal seleccionado.
//
// *****************************************************************************

#include <DMXSerial.h>
#include <Bounce2.h>

#define MODO_MASTER     2   // Modo master (controlador)
#define BOTON_RED       6
#define BOTON_GREEN     5
#define BOTON_BLUE      4
#define BOTON_BRILLO    3
#define PIN_NIVEL       A0  // Potenciómetro


// La dirección base es la dirección de la luz DMX, los offsets se le suman
// para obtener la dirección de la función específica a controlar
byte direccion_base = 5;
byte offset_red = 0;
byte offset_green = 1;
byte offset_blue = 2;
byte offset_brillo = 3;

// Variables para guardar el offset y el nivel del potenciómetro actual
byte offset;
byte nivel;


const int debounce_interval = 10;
Bounce boton_red_bounce = Bounce();
Bounce boton_green_bounce = Bounce();
Bounce boton_blue_bounce = Bounce();
Bounce boton_brillo_bounce = Bounce();


void setup() {
    DMXSerial.init(DMXController, MODO_MASTER);

    pinMode(BOTON_RED, INPUT_PULLUP);
    pinMode(BOTON_BLUE, INPUT_PULLUP);
    pinMode(BOTON_GREEN, INPUT_PULLUP);
    pinMode(BOTON_BRILLO, INPUT_PULLUP);

    boton_red_bounce.attach(BOTON_RED);
    boton_red_bounce.interval(debounce_interval);
    boton_green_bounce.attach(BOTON_GREEN);
    boton_green_bounce.interval(debounce_interval);
    boton_blue_bounce.attach(BOTON_BLUE);
    boton_blue_bounce.interval(debounce_interval);
    boton_brillo_bounce.attach(BOTON_BRILLO);
    boton_brillo_bounce.interval(debounce_interval);

    DMXSerial.write(direccion_base + offset_brillo, 100);
}


void loop() {
    // Obtenemos el valor del potenciómetro (entre 1 y 255)
    nivel = map(analogRead(PIN_NIVEL), 0, 1023, 1, 255);

    // Obtenemos el offset actualmente seleccionado (R, G, B o brillo)
    boton_red_bounce.update();
    boton_green_bounce.update();
    boton_blue_bounce.update();
    if (boton_red_bounce.fell()) {
        offset = offset_red;
    }
    if (boton_green_bounce.fell()) {
        offset = offset_green;
    }
    if (boton_blue_bounce.fell()) {
        offset = offset_blue;
    }
    if (boton_brillo_bounce.fell()) {
        offset = offset_brillo;
    }

    // Mandamos el valor obtenido al canal seleccionado
    DMXSerial.write(direccion_base + offset, nivel);
}