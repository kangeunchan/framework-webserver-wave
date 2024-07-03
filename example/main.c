#include "../include/wave.h"

void test_read() {
    char filename[] = "SETTINGS.wave";
    waveFileReader(filename, "./example");
}

int main(void) {
    test_read();

    return 0;
}