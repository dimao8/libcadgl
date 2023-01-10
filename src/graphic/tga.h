#ifndef TGA_H
#define TGA_H

#define IMAGE_TYPE_UNUSED                   (0x00)
#define IMAGE_TYPE_UNCOMPRESSED_COLORMAP    (0x01)
#define IMAGE_TYPE_UNCOMPRESSED_TRUE_COLOR  (0x02)
#define IMAGE_TYPE_UNCOMPRESSED_GRAYSCALE   (0x03)
#define IMAGE_TYPE_RLE_COLORMAP             (0x0A)
#define IMAGE_TYPE_RLE_TRUE_COLOR           (0x0B)
#define IMAGE_TYPE_RLE_GRAYSCALE            (0x0C)

#define IMAGE_DESCRIPTOR_VERTICAL_BIT       (0x20)
#define IMAGE_DESCRIPTOR_HORIZONTAL_BIT     (0x10)
#define IMAGE_DESCRIPTOR_BOTTOM_LEFT        (0x00)
#define IMAGE_DESCRIPTOR_BOTTOM_RIGHT       (IMAGE_DESCRIPTOR_HORIZONTAL_BIT)
#define IMAGE_DESCRIPTOR_TOP_LEFT           (IMAGE_DESCRIPTOR_VERTICAL_BIT)
#define IMAGE_DESCRIPTOR_TOP_RIGHT          (IMAGE_DESCRIPTOR_VERTICAL_BIT | IMAGE_DESCRIPTOR_HORIZONTAL_BIT)

#pragma pack(push, 1)
typedef struct color_map_specification_tag
{
  uint16_t first_entry_index;
  uint16_t color_map_length;
  uint8_t color_map_entry_size;
} color_map_specification_t;

typedef struct tga_header_tag
{
  uint8_t id_length;
  uint8_t color_map_type;
  uint8_t image_type;
  color_map_specification_t color_map_specification;
  uint16_t x_origin;
  uint16_t y_origin;
  uint16_t image_width;
  uint16_t image_height;
  uint8_t pixel_depth;
  uint8_t image_descriptor;
} tga_header_t;
#pragma pack(pop)

#endif // TGA_H
