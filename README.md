# Nested INI Library

This is a library be used to encode and decode data or file with Nested INI format.
The Nested INI is derived from the Microsoft INI format and support nested sections by indents.

## Example

    #include <assert.h>
    #include <stdio.h>
    #include <nini/nini.h>
    /*
     * Assume that there is a file named "sample.ini", and its contents are:
     *
     * [record]
     *     desc = "The test record."
     *     duration = 59.50  ; Seconds.
     *     [video]
     *         width = 1024
     *         height = 768
     *         framerate = 30
     *         interlaced = false
     *     [audio]
     *         encoding = pcm
     *         mode = stereo
     */
    int main(void)
    {
        nini_root_t nini;
        nini_root_init(&nini, NINI_FORMAT_NESTED_INI);

        assert( nini_root_load_file(&nini, "sample.ini", NULL) );

        printf("Description: %s\n", nini_read_string (&nini, "record/desc"            , '/', NULL));
        printf("Duration: %lf\n"  , nini_read_float  (&nini, "record/duration"        , '/', 0));
        printf("Width: %ld\n"     , nini_read_integer(&nini, "record/video/width"     , '/', 0));
        printf("Height: %ld\n"    , nini_read_integer(&nini, "record/video/height"    , '/', 0));
        printf("Frame rate: %ld\n", nini_read_integer(&nini, "record/video/framerate" , '/', 0));
        printf("Interlaced: %d\n" , nini_read_bool   (&nini, "record/video/interlaced", '/', false));
        printf("Encoding: %s\n"   , nini_read_string (&nini, "record/audio/encoding"  , '/', ""));
        printf("Mode: %s\n"       , nini_read_string (&nini, "record/audio/mode"      , '/', ""));

        nini_root_deinit(&nini);

        return 0;
    }

    /*
     * Then, the output will be:
     *
     * Description: The test record.
     * Duration: 59.500000
     * Width: 1024
     * Height: 768
     * Frame rate: 30
     * Interlaced: 0
     * Encoding: pcm
     * Mode: stereo
     */
