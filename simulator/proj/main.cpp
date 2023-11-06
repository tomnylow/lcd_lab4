
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>

#define NUM_SAMPLES 1000
#define AMP 1.0
#define FREQ 2.0
#define DUTY_CYCLE 0.5
#define NOISE_LEVEL 0.2
#define WINDOW_SIZE 20

// Function for generating random number with normal distribution
float randn() {
    float u = 0.0, v = 0.0, s = 0.0;
    do {
        u = (float)rand() / RAND_MAX * 2 - 1;
        v = (float)rand() / RAND_MAX * 2 - 1;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);
    s = sqrt(-2.0 * log(s) / s);
    return u * s;
}

// Function for generating noisy sine wave
void generate_noisy_sine_wave(float* clean_signal, float* noisy_signal) {
    for (int i = 0; i < NUM_SAMPLES; i++) {
        float time = (float)i / NUM_SAMPLES * 2 * M_PI;

        // Original signal - sine wave
        clean_signal[i] = AMP * sin(FREQ * time);

        // Adding noise to the signal
        noisy_signal[i] = clean_signal[i] + NOISE_LEVEL * randn();
    }
}

// Function for applying moving average filter
void moving_average_filter(float* input, float* output) {
    float window[WINDOW_SIZE] = {0};
    float sum = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        // Add current value to the window
        sum -= window[i % WINDOW_SIZE];
        window[i % WINDOW_SIZE] = input[i];
        sum += input[i];

        // Calculate average value in the window
        output[i] = sum / WINDOW_SIZE;
    }
}

// Function for applying median filter
void median_filter(float* input, float* output) {
    float window[WINDOW_SIZE];

    for (int i = 0; i < NUM_SAMPLES; i++) {
        // Fill the window with current values
        for (int j = 0; j < WINDOW_SIZE; j++) {
            window[j] = input[i - WINDOW_SIZE / 2 + j];
        }

        // Sort the window
        for (int j = 0; j < WINDOW_SIZE - 1; j++) {
            for (int k = j + 1; k < WINDOW_SIZE; k++) {
                if (window[j] > window[k]) {
                    float temp = window[j];
                    window[j] = window[k];
                    window[k] = temp;
                }
            }
        }

        // Calculate the median value in the window
        output[i] = window[WINDOW_SIZE / 2];
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    float clean_sine_wave[NUM_SAMPLES];
    float noisy_sine_wave[NUM_SAMPLES];
    float filtered_sine_wave_moving_average[NUM_SAMPLES];
    float filtered_sine_wave_median[NUM_SAMPLES];

    srand(time(NULL));

    generate_noisy_sine_wave(clean_sine_wave, noisy_sine_wave);

    moving_average_filter(noisy_sine_wave, filtered_sine_wave_moving_average);

    median_filter(noisy_sine_wave, filtered_sine_wave_median);


    // Plotting the graphs
    int x, y;

    // Plotting the Noisy Sine Wave
    cleardevice();
    setcolor(RED);
    for (int i = 0; i < NUM_SAMPLES; i++) {
        x = i;
        y = (int)noisy_sine_wave[i] * 150 + 240;
        putpixel(x, y, RED);
    }
    delay(1000);

    // Plotting the Filtered Sine Wave using Moving Average Filter
    cleardevice();
    setcolor(YELLOW);
    for (int i = 0; i < NUM_SAMPLES; i++) {
        x = i;
        y = (int)filtered_sine_wave_moving_average[i] * 150 + 240;
        putpixel(x, y, YELLOW);
    }
    delay(1000);

    // Plotting the Filtered Sine Wave using Median Filter
    cleardevice();
    setcolor(GREEN);
    for (int i = 0; i < NUM_SAMPLES; i++) {
        x = i;
        y = (int)filtered_sine_wave_median[i] * 150 + 240;
        putpixel(x, y, GREEN);
    }
    delay(1000);

    closegraph();

    return 0;
}
