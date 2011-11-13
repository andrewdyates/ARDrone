#define WIDTH 320
#define HEIGHT 240

int read_ppm(unsigned char* buf, char* filename);
int write_ppm(unsigned char* buf, char* filename);
unsigned char ind3(unsigned char* buf, int x, int y, char a);
