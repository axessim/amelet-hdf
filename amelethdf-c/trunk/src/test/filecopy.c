#include <stdio.h>



void filecopy(const char *input, const char *output)
{
    signed char buf[100];
    void *pb = (void *) buf;
    FILE *fpi, *fpo;
    size_t n;

    fpi = fopen(input, "rb");
    fpo = fopen(output, "wb");

    if (fpi && fpo)
    {
        while ((n = fread(pb, sizeof(char), 100, fpi)))
            fwrite(pb, sizeof(char), n, fpo);
    }
    fclose(fpi);
    fclose(fpo);
}
