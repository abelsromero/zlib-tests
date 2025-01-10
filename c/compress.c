#include <stdio.h>
#include <zlib.h>

int compress_file(const char *input_file, const char *output_file) {
    FILE *source = fopen(input_file, "rb");
    if (!source) {
        perror("Error opening input file");
        return 1;
    }

    FILE *dest = fopen(output_file, "wb");
    if (!dest) {
        perror("Error opening output file");
        fclose(source);
        return 1;
    }

    z_stream stream = {0};
    deflateInit(&stream, Z_BEST_COMPRESSION);

    unsigned char in[4096];
    unsigned char out[4096];

    int flush;
    do {
        stream.avail_in = fread(in, 1, sizeof(in), source);
        if (ferror(source)) {
            perror("Error reading input file");
            deflateEnd(&stream);
            fclose(source);
            fclose(dest);
            return 1;
        }

        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        stream.next_in = in;

        do {
            stream.avail_out = sizeof(out);
            stream.next_out = out;
            deflate(&stream, flush);
            fwrite(out, 1, sizeof(out) - stream.avail_out, dest);
        } while (stream.avail_out == 0);
    } while (flush != Z_FINISH);

    deflateEnd(&stream);
    fclose(source);
    fclose(dest);
    return 0;
}