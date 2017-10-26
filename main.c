#include <xc.h>
#include <pic12f675.h>

#define _XTAL_FREQ 4000000; // frecuencia del oscilador para el delay
#define BOTON_ENTRADA GPIObits.GP3
#define BOTON_CONTROL GPIObits.GP5
#define LED_RESULTADO GPIObits.GP0
#define LED_MODO_ENTRADA GPIObits.GP1

//_CONFIG(MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & WDTE_OFF & PWRTE_OFF & FOSC_INTRCIO);

unsigned char operando1; // operando1 de 8 bits
unsigned char operando2; // operando2 de 8 bits
unsigned char resultado;
bit flagFlanco = 0; // sirve para detectar el flanco de subida
int tiempoParpadeo = 1000;

void configuracion() {
    /*
     * GP1 -> salida al LED de resultado.
     * GP2 -> salida al LED de modo de entrada. LOW=operando 1 HIGH=operando2
     * GP0 -> entrada de operando / reset
     * GP3 -> entrada de control de operando. LOW= ingresar operando1, HIGH= ingresar operando2
     */
    TRISIO = 0b111100;

    // configurar el timer
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111; // se incrementa cada 256 us
}

void divisionEntera() {
    unsigned char temp = 1;

    if (operando2 == 0) {
        return;
    }

    //  Se calcula cuantas posiciones enteras tendrá el resultado
    while (operando2 <= operando1) {
        operando2 <<= 1;
        temp <<= 1;
    }

    operando2 >>= 1;
    temp >>= 1;

    while (temp != 0) {
        if (operando1 >= operando2) {
            operando1 -= operando2;
            resultado |= temp;
        }
        temp >>= 1;
        operando2 >>= 1;
    }
}

void parpadearLEDResultado() {
    GPIObits.GPIO0 = 0;
    _delay(tiempoParpadeo);
    GPIObits.GPIO0 = 1;
    _delay(tiempoParpadeo);
}

void mostrarResultado() {
    divisionEntera();

    while (resultado > 0) {
        CLRWDT();
        parpadearLEDResultado();
        resultado--;
    }
}

void leerDatos() {
    if (BOTON_ENTRADA == 1) {
        if (flagFlanco == 0) {
            long espera = 200000;
            while (espera > 0) {                
                espera--;
                CLRWDT();
                if (BOTON_ENTRADA == 0) {
                    flagFlanco = 1;
                    if (BOTON_CONTROL == 0) { // ingresando operando1
                        operando1 += 1;
                    } else { // ingresando operando2
                        operando2 += 1;
                    }
                    return;
                } else if (espera == 0) {
                    mostrarResultado();
                    return;
                }
            }
        }
    } else {
        flagFlanco = 0;
    }
}

int main() {
    configuracion();
    for (;;) {
        CLRWDT();
        leerDatos();
    }
}
