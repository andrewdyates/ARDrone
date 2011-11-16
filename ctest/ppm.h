#define WIDTH 320
#define HEIGHT 240

int read_ppm(unsigned char* buf, char* filename);
int read_ppm1(unsigned char* buf, char* filename);
int write_ppm(unsigned char* buf, char* filename);
int write_ppm1(unsigned char* buf, char* filename);
unsigned int ind3(int x, int y, char a);
unsigned int ind1(int x, int y);

