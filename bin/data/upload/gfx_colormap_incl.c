/* colormap defs and pattern id's.  parsed by cmdseq.pl */
enum {
    COLORMAP_DEFAULT  = 0,
    COLORMAP_NETSCAPE = 1,
    COLORMAP_332LOW   = 2,
    COLORMAP_332MED   = 3,
    COLORMAP_332HIGH  = 4,
};

// pattern name to pattern number mapping. 
// pattern names are "demo", "image2s", "rainbow", etc.
// parsing requires the PATTERN_ID_* format
#define PATTERN_ID_DEMO 1
#define PATTERN_ID_TEST 2
#define PATTERN_ID_DEAD_BATTERY 3
#define PATTERN_ID_IMAGE 4
#define PATTERN_ID_IMAGE2S 5
#define PATTERN_ID_XMODEM 6
#define PATTERN_ID_HALLTIMING 7
#define PATTERN_ID_CALIBRATE 8
#define PATTERN_ID_RAINBOW 9

