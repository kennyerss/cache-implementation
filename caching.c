#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


uint64_t convert(char *hex);
uint64_t power(int base, int exponent);

// Track the hit and misses of each cache
uint64_t directHit = 0;
uint64_t directMiss = 0;
uint64_t fullyHit = 0;
uint64_t fullyMiss = 0;
uint64_t setHit = 0;
uint64_t setMiss = 0;

// Stores the uint64_t number
uint64_t number = 0;

int main(int argc, char *argv[]) {

  // Create caches
  uint64_t fullyAssociative[512];
  uint64_t directMapped[512];
  uint64_t setAssociative[512];

  // Buffer size for each line
  char buf[17];

  // read file in stdin
  while (fgets(buf, 17, stdin) != NULL) {

    // Convert each line to uint64_t number
    number = convert(buf);

    // Fully associative cache
    // Extract the bits needed for fully associative
    uint64_t fullyMask = 0x3f;
    uint64_t offset = number & fullyMask; // last six bits
    uint64_t fullyTag = number >> 6;
    // printf("full tag: %llu\n", fullyTag);

    int found = 0; // Tracks if fullyTag has been hit

    for (int i = 0; i < 512; i++) {
      if(fullyAssociative[i] == fullyTag) { // if there's a hit, increment fullyHit by 1
        found = 1;
        fullyHit++;
        for (int j = i; j > 0; j--) { // Shifting down every element in cache
          fullyAssociative[j] = fullyAssociative[j-1];
        }
        fullyAssociative[0] = fullyTag; // Set topmost entry as most recently used
      }
    }
    if (found == 0) { // There is a miss, increment fullyMiss by 1
        for (int j=512; j>0; j--) {
          fullyAssociative[j] = fullyAssociative[j-1]; // Shift elements in array down
        }
      fullyAssociative[0] = fullyTag; // Sets tag as topmost
      fullyMiss++;
    }

    // Direct mapped cache
    // Extract the bits needed for direct mapped
    uint64_t entryMask = 0x7fff;
    uint64_t directTag = number >> 15; // Shift last 15 bits to get tag line
    uint64_t entry = number & entryMask;
    uint64_t entryShifted = entry >> 6; // Shift the last six bits to get entry

    if (directMapped[entryShifted] == directTag){ // direct mapped hit
      directHit ++;
    } else { // direct mapped miss
      directMapped[entryShifted] = directTag;
      directMiss++;
    }

    // Set associative cache
    // Extract the bits needed for set associative
    uint64_t newMask = 0xfff;
    uint64_t setTag = number >> 12;
    uint64_t setEntryMask = number & newMask;
    uint64_t setEntry = setEntryMask >> 6;

    int setFound = 0; // Stores whether or not if tag is found in set
    int setToLook = setEntry * 8; // Multiply the set entry by 8 to start index

    for (int i = 0; i < 7; i++ ) { // Index stops at 7 b
      if(setAssociative[setToLook] == setTag) { // if there's a hit in a set, increment setHit by 1
        setFound = 1;
        setHit++;
        for (int j = 7; j > 0; j--) { // Shift down every element of the set
          setAssociative[setToLook] = setAssociative[setToLook-1];
        }
        setAssociative[setToLook] = setTag;
      }
      setToLook++;
    }

    if (setFound == 0) { // if there's a miss in a set, shift every element of the set
      for (int j = 7; j > 0; j--) {
        setAssociative[setToLook] = setAssociative[setToLook-1];
        setToLook --;
      }
      setAssociative[setToLook] = setTag;
      setMiss++;
    }

  }
  printf("Fully associative hit: %llu\n", fullyHit);
  printf("Fully associative miss: %llu\n", fullyMiss);
  printf("Direct mapped hit: %llu\n", directHit);
  printf("Direct mapped miss: %llu\n", directMiss);
  printf("Set associative hit: %llu\n", setHit);
  printf("Set associative miss: %llu\n", setMiss);
  return 0;
}


// Power function to exponentiate number
uint64_t power(int base, int exponent){
  uint64_t result = 1;
  for (int e = exponent; e > 0 ; e--) {
    result = result * base;
  }
  return result;

}

// convert each line of text in fgets to uint64_t
uint64_t convert(char *hex) {
  int base = 16; // base for power function
  uint64_t number = 0; // Resulting uint64_t number to return
  int counter = 0; // Tracks exponent counter

  int index = strlen(hex) - 1; // Start from right to left
  int conversion = 0; // Stores the hex to decimal conversion

  if (hex[index] == '\n') { // If last bit is a new line character, decrement index
    index -= 1;
  }

  // Traverses through hexadecimal line from right to left
  for(int i = index; i >= 0; i--) {
    if(hex[i] == '0'){
      conversion = 0;
    }
    if(hex[i] == '1') {
      conversion = 1;
    }
    if(hex[i] == '2') {
      conversion = 2;
    }
    if(hex[i] == '3') {
      conversion = 3;
    }
    if(hex[i] == '4') {
      conversion = 4;
    }
    if(hex[i] == '5') {
      conversion = 5;
    }
    if(hex[i] == '6') {
      conversion = 6;
    }
    if(hex[i] == '7') {
      conversion = 7;
    }
    if(hex[i] == '8') {
      conversion = 8;
    }
    if(hex[i] == '9') {
      conversion = 9;
    }
    if(hex[i] == 'a') {
      conversion = 10;
    }
    if(hex[i] == 'b') {
      conversion = 11;
    }
    if(hex[i] == 'c') {
      conversion = 12;
    }
    if(hex[i] == 'd') {
      conversion = 13;
    }
    if(hex[i] == 'e') {
      conversion = 14;
    }
    if(hex[i] == 'f') {
      conversion = 15;
    }
    // Add into number the conversion multiplied by the power function
    number += conversion * power(base, counter);
    counter += 1; // Increment exponent counter by 1 after each for loop iteration
  }

  return number;
}
