#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED
class Image {
public:
	Image(char* ps, int w, int h);
	~Image();
	char* pixels;
	int width;
	int height;
};
Image* loadBMP(const char* filename);
#endif