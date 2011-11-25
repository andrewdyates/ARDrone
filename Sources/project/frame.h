void process_frame_ball(unsigned char* frame_buf, unsigned char* mask_buf, int* mass, int* x_center, int* y_center);

void process_frame_face(unsigned char* frame_buf, int* mass, int* x_center, int* y_center, int* width, int* height);

void display_mask(unsigned char* mask_buf, unsigned char* display_buf, int x_center, int y_center);

void clear_mask(unsigned char* mask_buf);
