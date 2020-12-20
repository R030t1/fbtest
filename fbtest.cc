#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <iostream>
using namespace std;

int fb_get_data(const char* path, uint8_t **data);

int main(int argc, char *argv[]) {
	uint8_t *data = nullptr;
	cout << fb_get_data(argv[1], &data) << endl;
	free(data);
}

int fb_get_data(const char* path, uint8_t **data) {
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	int fd = open(path, O_RDONLY);
	if (fd < 0) return fd;

	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
		cout << "error: ioctl(FBIOGET_VSCREENINFO)" << endl;
	}
	
	if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
		cout << "error: ioctl(FBIOGET_FSCREENINFO)" << endl;
	}

	ssize_t raw_length = vinfo.yres * finfo.line_length;
	cout << "raw_length: " << raw_length << endl;
	*data = (uint8_t *)malloc(raw_length);

	ssize_t num_buffers = vinfo.yoffset / vinfo.yres;
	cout << "num_buffers: " << num_buffers << endl;

	// TODO: ???
	ssize_t active_buffer_offset = 0;//raw_length * num_buffers;
	cout << "active_buffer_offset: " << active_buffer_offset << endl;

	lseek(fd, active_buffer_offset, SEEK_SET);
	ssize_t n = read(fd, *data, raw_length);
	if (n < raw_length) {
		cout << "error: read(fd): incomplete or other error" << endl;
	}

	close(fd);
	return 0;
}
