#include <stdio.h>
#include <bmpfile.h>

int
main(int argc, char **argv)
{
  bmpfile_t *bmp;
  int i, j;
  rgb_pixel_t pixel = {128, 64, 0, 0};

  if (argc < 5) {
    printf("Usage: %s filename width height depth.\n", argv[0]);
    return 1;
  }

  if ((bmp = bmp_create(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]))) == NULL) {
    printf("Invalid depth value: %s.\n", argv[4]);
    return 1;
  }

  for (i = 10, j = 10; j < atoi(argv[3]); ++i, ++j) {
    bmp_set_pixel(bmp, i, j, pixel);
    pixel.red++;
    pixel.green++;
    pixel.blue++;
    bmp_set_pixel(bmp, i + 1, j, pixel);
    bmp_set_pixel(bmp, i, j + 1, pixel);
  }

  bmp_save(bmp, argv[1]);
  bmp_destroy(bmp);

  return 0;
}
