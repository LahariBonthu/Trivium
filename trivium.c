#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STATE_SIZE 288

// Function to initialize Trivium with 80-bit key and IV
void initialize_trivium(unsigned char *state, unsigned char *key, unsigned char *iv) {
    memset(state, 0, STATE_SIZE);
    
    for (int i = 0; i < 80; i++) {
        state[i] = key[i];
    }
    
    for (int i = 0; i < 80; i++) {
        state[i + 93] = iv[i];
    }
    
    state[285] = 1;
    state[286] = 1;
    state[287] = 1;
    
    for (int i = 0; i < 1152; i++) {
        unsigned char t1 = state[65] ^ state[92] ^ (state[90] & state[91]) ^ state[170];
        unsigned char t2 = state[161] ^ state[176] ^ (state[174] & state[175]) ^ state[263];
        unsigned char t3 = state[242] ^ state[287] ^ (state[285] & state[286]) ^ state[68];
        
        memmove(&state[1], &state[0], 92);
        memmove(&state[94], &state[93], 83);
        memmove(&state[178], &state[177], 110);
        
        state[0] = t3;
        state[93] = t1;
        state[177] = t2;
    }
}

// Generate one bit of keystream
unsigned char generate_bit(unsigned char *state) {
    unsigned char z = state[65] ^ state[92] ^ state[161] ^ state[176] ^ state[242] ^ state[287];
    
    unsigned char t1 = state[65] ^ state[92] ^ (state[90] & state[91]) ^ state[170];
    unsigned char t2 = state[161] ^ state[176] ^ (state[174] & state[175]) ^ state[263];
    unsigned char t3 = state[242] ^ state[287] ^ (state[285] & state[286]) ^ state[68];
    
    for (int i = 0; i < 92; i++) {
        state[i] = state[i + 1];
    }
    for (int i = 93; i < 176; i++) {
        state[i] = state[i + 1];
    }
    for (int i = 177; i < 287; i++) {
        state[i] = state[i + 1];
    }
    
    state[92] = t1;
    state[176] = t2;
    state[287] = t3;
    
    return z;
}

int main() {
    unsigned char state[STATE_SIZE];
    unsigned char key[80], iv[80];
    unsigned char buffer[1024 * 1024]; // 1 MB buffer
    FILE *fp;
    
    srand(time(NULL));
    for (int i = 0; i < 80; i++) {
        key[i] = rand() % 2;
        iv[i] = rand() % 2;
    }
    
    printf("Random key (hex): 0x");
    for (int i = 0; i < 80; i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++) {
            byte = (byte << 1) | key[i + j];
        }
        printf("%02x", byte);
    }
    printf("\n");
    
    printf("Random IV (hex): 0x");
    for (int i = 0; i < 80; i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++) {
            byte = (byte << 1) | iv[i + j];
        }
        printf("%02x", byte);
    }
    printf("\n");
    
    initialize_trivium(state, key, iv);
    
    printf("Generating 1 MB of keystream...\n");
    for (int i = 0; i < 1024 * 1024; i++) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++) {
            byte = (byte << 1) | generate_bit(state);
        }
        buffer[i] = byte;
    }
    
    fp = fopen("trivium_output.bin", "wb");
    if (fp == NULL) {
        printf("Failed to create output file!\n");
        return 1;
    }
    
    fwrite(buffer, 1, 1024 * 1024, fp);
    fclose(fp);
    
    printf("Keystream saved to trivium_output.bin\n");
    printf("Use the NIST STS to analyze this file to get the test results table.\n");
    
    return 0;
}
