/**
 **********************************************************************************
 * @file           : pwm_simple.ino
 * @brief          : PWM dengan metode yang simple menggunakan bawaan dari arduino
 *                   untuk mengatur dutycycle dan frekuensi untuk menghasilkan PWM
 *                   mengunakan Variable Resistor
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
**/

/**
 * Sketch ini menggunakan pin:
 * PIN DIGITAL: [3, 5, dan 6] sebagai output pin PWM
 * PIN ANALOG : [A0] sebagai kontrol dutycycle
 *              [A1] sebagai kontrol frekuensi
 *
 * Q: Mengapa tidak menggunakan hanya analogWrite() saja?
 * A: Jika hanya menggunakan analogWrite(), kita hanya bisa mengatur nilai duty cycle dari sinyal
 *    PWM. Namun, dalam sketch ini kita bertujuan untuk menjelaskan lebih detail bagaimana PWM
 *    bekerja. Di sini terdapat dua faktor penting, yaitu frekuensi dan duty cycle, yang perlu
 *    diketahui pada setiap frekuensi. Konsep ini dapat diilustrasikan dalam bentuk pulsa pada pin
 *    [3, 5, dan 6], di mana kita mengatur jeda waktu menggunakan delayMicroseconds berdasarkan
 *    input analog untuk menentukan baik duty cycle maupun frekuensinya secara bersamaan.
 *
 **/

#include <Arduino.h>

// pin pada Variable Resistor untuk mengatur dutycycle pada pwm
#define PIN_VR_DUTY_CTR A0

// pin pada Variable Resistor untuk mengatur frequensi pada pwm
#define PIN_VR_FREQ_CTRL A1

#define MAP_ANALOG(X, MIN, MAX) ((((X) * ((MAX) - (MIN))) >> 10UL) + (MIN))

void setup(void)
{
    // set pin 9, 10, 11 menjadi output
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
}

void loop(void)
{
    // mengubah nilai analog pada pin `PIN_VR_FREQ_CTRL` dengan nilai:
    //        => 0 (us)  s/d 65634 (us) (mikrodetik)
    //        => 1 (MHz) s/d 15.26 (Hz)
    //
    // yang artinya jarak frequensi yang dapat diatur "secara teori"
    // adalah: 15.26 Hz s/d 1 MHz
    unsigned int freq = MAP_ANALOG((unsigned long)analogRead(PIN_VR_FREQ_CTRL), 0UL, 65534UL);

    // mambaca nilai analog pada pin `PIN_VR_DUTY_CTR` yang biasanya
    // dalam nilai 10 s/d 1024 dan mengubahnya
    // dalam nilai 10 s/d <freq> (sebagai persentase dari variable <freq>)
    unsigned int duty = MAP_ANALOG((unsigned long)analogRead(PIN_VR_DUTY_CTR), 0UL, freq);

    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    // delay dengan persentase duty_analog dari freq_analog sebagai lama waktu hidup (HIGH)
    // pada pin 9, 10, 11
    delayMicroseconds(duty);

    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);

    // delay dengan freq_analog dikurangi duty sebagai lama waktu mati (LOW) pada pin 9, 10, 11
    delayMicroseconds(freq - duty);
}
