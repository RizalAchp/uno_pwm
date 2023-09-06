/***********************************************************************************
 * @file           : pwm_timer_register.ino
 * @brief          : PWM dengan metode yang sama dengan pwm_simple.ino namun menggunakan
 *                   memanipulasi register port untuk mengatur dutycycle dan frekuensi
 *                   menghasilkan PWM mengunakan Variable Resistor
 ***********************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2023 Rizal Achmad Pahlevi <e32201406@student.polije.ac.id>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.

 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 **********************************************************************************
 */

/***********************************************************************************
 * Sketch ini menggunakan pin:
 * PIN DIGITAL: [9, 10, dan 11] sebagai output pin PWM
 * PIN ANALOG : [A0] sebagai kontrol dutycycle
 *              [A1] sebagai kontrol frekuensi
 *
 * sketch ini menggunakan metode yang sama dengan pwm_simple.ino namun menggunkakan
 * manipulasi register pada port pin untuk menghasilkan latency yang lebih kecil / lebih cepat
 **********************************************************************************
 */
#include <Arduino.h>

// pin pada Variable Resistor untuk mengatur dutycycle pada pwm
#define PIN_VR_DUTY_CTR A0

// pin pada Variable Resistor untuk mengatur frequensi pada pwm
#define PIN_VR_FREQ_CTRL A1

#define MAP_ANALOG(X, MIN, MAX) ((((X) * ((MAX) - (MIN))) >> 10UL) + (MIN))

void setup()
{
    // Serial.begin(9600);
    pinMode(9, OUTPUT);  /* set as a output put */
    pinMode(10, OUTPUT); /* set as a output put */
    pinMode(11, OUTPUT); /* set as a output put */

    TCCR1A = _BV(COM1A1) | _BV(COM1B1);
    TCCR1B = _BV(WGM13) | _BV(CS11);
}

void loop()
{
    // mengubah nilai analog pada pin `PIN_VR_FREQ_CTRL` dengan nilai yang sesui dengan index
    unsigned int freq = MAP_ANALOG(analogRead(PIN_VR_FREQ_CTRL), 0UL, 65534UL);
    ICR1              = (F_CPU / (2UL * freq));

    // mambaca nilai analog pada pin `PIN_VR_DUTY_CTR` yang biasanya
    // dalam nilai 0 s/d 1024 dan mengubahnya
    // dalam nilai 0 s/d 255 (sebagai persentase)
    unsigned int duty = MAP_ANALOG(analogRead(PIN_VR_DUTY_CTR), 0UL, 255UL);
    OCR1A             = duty; // set pin 9  PWM duty cycle
    OCR1B             = duty; // set pin 10 PWM duty cycle
    OCR2A             = duty; // set pin 11 PWM duty cycle

    // Serial.print("freq: ");
    // Serial.print(freq);
    // Serial.print(", icr: ");
    // Serial.print(icr);
    // Serial.print(", duty: ");
    // Serial.println(duty);
}
