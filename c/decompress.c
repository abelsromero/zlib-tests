#include <stdio.h>
#include <zlib.h>

int decompress_file(const char *input_file, const char *output_file) {
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
    inflateInit(&stream);

    unsigned char in[4096];
    unsigned char out[4096];

    int ret;
    do {
        stream.avail_in = fread(in, 1, sizeof(in), source);
        if (ferror(source)) {
            perror("Error reading input file");
            inflateEnd(&stream);
            fclose(source);
            fclose(dest);
            return 1;
        }

        if (stream.avail_in == 0) {
            break;
        }
        stream.next_in = in;

        do {
            stream.avail_out = sizeof(out);
            stream.next_out = out;
            ret = inflate(&stream, Z_NO_FLUSH);
            switch (ret) {
                case Z_STREAM_ERROR:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&stream);
                    fclose(source);
                    fclose(dest);
                    fprintf(stderr, "Decompression error: %d\n", ret);
                    return 1;
            }
            fwrite(out, 1, sizeof(out) - stream.avail_out, dest);
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(source);
    fclose(dest);
    return 0;
}
