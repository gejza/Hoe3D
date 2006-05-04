
#ifdef _LINUX

typedef struct tagBITMAPFILEHEADER {
        word    bfType;
        dword   bfSize;
        word    bfReserved1;
        word    bfReserved2;
        dword   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
        dword      biSize;
        long       biWidth;
        long       biHeight;
        word       biPlanes;
        word       biBitCount;
        dword      biCompression;
        dword      biSizeImage;
        long       biXPelsPerMeter;
        long       biYPelsPerMeter;
        dword      biClrUsed;
        dword      biClrImportant;
} BITMAPINFOHEADER;

#endif
