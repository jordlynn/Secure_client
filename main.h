#define DEBUG 0 // Set this equal to 1 if you want more verbose output.

// Globals
unsigned long MOD = 17;
unsigned long GENERATOR = 3;
unsigned long peerKey = 99;

unsigned long Calculate( unsigned long key );
unsigned long IntPow( unsigned long x, unsigned long pow, unsigned long mod );
