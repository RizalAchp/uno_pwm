/**
 **********************************************************************************
 * @file           : pwm_register.ino
 * @brief          : PWM dengan metode yang sama dengan pwm_simple.ino namun menggunakan
 *                   memanipulasi register port untuk mengatur dutycycle dan frekuensi
 *                   menghasilkan PWM mengunakan Variable Resistor
 **********************************************************************************
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

/**
 * Sketch ini menggunakan pin:
 * PIN DIGITAL: [9, 10, dan 11] sebagai output pin PWM
 * PIN ANALOG : [A0] sebagai kontrol dutycycle
 *              [A1] sebagai kontrol frekuensi
 *
 * sketch ini menggunakan metode yang sama dengan pwm_simple.ino namun menggunkakan
 * manipulasi register pada port pin untuk menghasilkan latency yang lebih kecil / lebih cepat
 **/
#include <Arduino.h>

// pin pada Variable Resistor untuk mengatur dutycycle pada pwm
#define PIN_VR_DUTY_CTR A0

// pin pada Variable Resistor untuk mengatur frequensi pada pwm
#define PIN_VR_FREQ_CTRL A1

#define MAP_ANALOG(X, MIN, MAX) ((((X) * ((MAX) - (MIN))) >> 10UL) + (MIN))

void setup(void)
{
    // info register port:
    // register (DDRB) == PIN 8 s/d 13
    // BYTE => 0 0  0   0   0   0  0 0
    // PIN  => _ _ 13  12  11  10  9 8

    // set pin 9, 10, 11 menjadi output menggunakan Register secara langsung
    DDRB |= B00001110;
    PORTB &= B11110001;
}

void loop(void)
{
    // mengubah nilai analog pada pin `PIN_VR_FREQ_CTRL` dengan nilai:
    //        => 10  (us)  s/d 65634 (us) (mikrodetik)
    //        => 100 (KHz) s/d 15.26 (Hz)
    //
    // yang artinya jarak frequensi yang dapat diatur "secara teori"
    // adalah: 15.26 Hz s/d 1 MHz
    unsigned int freq = MAP_ANALOG((unsigned long)analogRead(PIN_VR_FREQ_CTRL), 0UL, 65534UL);

    // mambaca nilai analog pada pin `PIN_VR_DUTY_CTR` yang biasanya
    // dalam nilai 0 s/d 1024 dan mengubahnya
    // dalam nilai 10 s/d <freq> (sebagai persentase dari variable <freq>)
    unsigned int duty = MAP_ANALOG((unsigned long)analogRead(PIN_VR_DUTY_CTR), 0UL, freq);

    // menggunakan register untuk menghidupkan(set 1) atau mematikan (set 0) pin
    // pada register port, menggunakan register agar lebih cepat atau mendapatkan
    // latency atau cpu cycle yang lebih kecil.
    // anda juga bisa menggunakan cara seperti biasanya yaitu:
    //        `digitalWrite(PIN, LEVEL)`
    //
    // namun latency atau cpu cycle yang didapatkan lebih besar
    // dibandingkan dengan menggunakan register port sehingga menghasilkan delay
    // yang kurang presisi.
    // (ref: http://electronoobs.com/images/Arduino/tut_130/Register_control.jpg)

    // bitOR `|` pada port agar hanya pada bit 9,10,11 saja yang hidup (HIGH)
    PORTB |= B00001110;
    // delay dengan persentase duty sebagai lama waktu hidup (HIGH) pada pin 9,10,11
    delayMicroseconds(duty);

    // bitAND '&' pada port agar hanya pada bit 9,10,11 saja yang mati (LOW)
    PORTB &= B11110001;
    // delay dengan persentase duty sebagai lama waktu mati (LOW) pada pin 9,10,11
    delayMicroseconds(freq - duty);
}
