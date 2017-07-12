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
     *     duration = 59.50  # Seconds.
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
        nini_root_init(&nini, &NINI_FORMAT_MSINI_NESTED);

        assert( nini_root_load_file(&nini, "sample.ini", NULL);

        printf("Description: %s\n", nini_read_string(&nini, "record/desc", '/', NULL));

        nini_root_deinit(&nini);

        return 0;
    }

    /*
     * The output will be:
     * Description: The test record.
     */
