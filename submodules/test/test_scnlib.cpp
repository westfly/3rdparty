#include <scn/scn.h>
#include <cstdio>

int main() {
    int i;
    // Read an integer from stdin
    // with an accompanying message
    scn::prompt("What's your favorite number? ", "{}", i);
    printf("Oh, cool, %d!", i);
}

