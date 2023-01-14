#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int n = 16; // n represents the number of bits a number is represented by
// int maxDigits = 1 + ((n - 1) / 3); // if x != 0
int maxDigits = 5;
uint8_t fourBitMask = (uint8_t)15; 


void printBCD(uint8_t *BCD) {
    for (int i = maxDigits - 1; i >= 0; i --) {
        printf("%u", BCD[i]);
    }
    printf("\n");

}

uint8_t *convert2BCD(const char *binary) {
    // create an array of maxDigits digits
    uint8_t *decimal = (uint8_t *)malloc((sizeof(uint8_t)) * maxDigits); 
    // initialize all digits to zero
    for (int i = 0; i < maxDigits; i++) {
        decimal[i] = (uint8_t)0;
    }
    /**
     * char *binary:
     * 
     * depends on big/little endian when binary is sent from FPGA to Arduino
     * 
     * index    0   1 2 ... 49 50
     * value   MSB             LSB
    */

    /**
     * uint8_t *decimal:
     * 
     * index      0       ....       49
     * value  ones place  ....  largest digit
     * */   

    /** TODO: change index based on little / big endian 
     *        currently expects input to be little endian (MSB in index zero)
    */
    uint8_t prevMSB = (uint8_t)(binary[0]); 
            printf("prevMSB: %c\n", prevMSB);

    // begin conversion
    for (int i = 0; i < n; i++) {
        
        // check if we need to add 3
        for (int j = maxDigits - 1; j >= 0; j--) { 
            printf("index: %d\n", j);
            if (decimal[j] >= 5) {
                decimal[j] += 3;
            }

        }

        // perform left shift
        for (int j = 0; j < maxDigits; j++) {
            uint8_t newPrevMSB = decimal[j] >> 3;
            decimal[j] = ((decimal[j] << 1) & ~((uint8_t)1)) ^ prevMSB; // set the LSB of the digit to be the MSB of the prev digit
            prevMSB = newPrevMSB;                                       // update the prevMSB
            printf("prevMSB UPDATE: %u\n", prevMSB);
            
            decimal[j] = decimal[j] & fourBitMask;    // get rid of 1 in 5th bit index
        }   

        if (i < n - 1) {
            prevMSB = binary[i + 1];
            printf("prevMSB: %c\n", prevMSB);
            
        }
    }
    return decimal;
}

int main() {
    
    const char *input = "1111111011011100";
    uint8_t *test = convert2BCD(input);
    for (int i = 0; i < n; i ++) {
        printf("%c", input[i]);
    }
    printf("\n");
    printBCD(test);
}

    