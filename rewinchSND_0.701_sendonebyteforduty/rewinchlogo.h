#define logo_width 128
#define logo_height 64
#define logomini_width 32//56
#define logomini_height 14//24
#define sending_w 28
#define sending_h 12
#define sending_s_w 20
#define sending_s_h 10

const unsigned char rewinchlogo[]= {
// 'Rewinch Logo 003_128x64', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 
0xf0, 0x3f, 0xf0, 0x3f, 0xf0, 0x18, 0x00, 0x06, 0x0c, 0x1f, 0xfe, 0x00, 0xff, 0xe1, 0xff, 0xf8, 
0xf0, 0x7f, 0xf8, 0xff, 0xfc, 0x3c, 0x00, 0x0f, 0x1e, 0x3f, 0xff, 0x01, 0xff, 0xf1, 0xff, 0xfc, 
0xf0, 0xff, 0xf9, 0xff, 0xfc, 0x3c, 0x0c, 0x0f, 0x1e, 0x3f, 0xff, 0x83, 0xff, 0xf1, 0xff, 0xfe, 
0xf0, 0xff, 0xf1, 0xe0, 0x1e, 0x3c, 0x1e, 0x0f, 0x1e, 0x3f, 0xff, 0xc3, 0xff, 0xe1, 0xff, 0xff, 
0xf0, 0xf8, 0x01, 0xc0, 0x0e, 0x3c, 0x3f, 0x0f, 0x1e, 0x3c, 0x03, 0xe3, 0xe0, 0x01, 0xf0, 0x1f, 
0xf0, 0xf0, 0x01, 0xc0, 0x0e, 0x3c, 0x7f, 0x8f, 0x1e, 0x3c, 0x01, 0xe3, 0xc0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xc0, 0x1e, 0x3c, 0xfb, 0xcf, 0x1e, 0x3c, 0x01, 0xe3, 0xc0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xcf, 0xfe, 0x38, 0xf1, 0xe7, 0x1e, 0x3c, 0x01, 0xe3, 0xc0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xcf, 0xfc, 0x19, 0xe0, 0xe6, 0x1e, 0x3c, 0x01, 0xe3, 0xc0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xcf, 0xf0, 0x13, 0xe0, 0xf2, 0x1e, 0x3c, 0x01, 0xe3, 0xc0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xc0, 0x00, 0x03, 0xc0, 0x78, 0x1e, 0x3c, 0x01, 0xe3, 0xe0, 0x01, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xe0, 0x00, 0x07, 0x80, 0x78, 0x1e, 0x3c, 0x01, 0xe3, 0xff, 0xe1, 0xe0, 0x0f, 
0xf0, 0xf0, 0x01, 0xff, 0xfc, 0x0f, 0x80, 0x3c, 0x1e, 0x3c, 0x01, 0xe3, 0xff, 0xf1, 0xe0, 0x0f, 
0xf0, 0xf0, 0x00, 0xff, 0xfe, 0x0f, 0x00, 0x1c, 0x1e, 0x3c, 0x01, 0xe1, 0xff, 0xf1, 0xe0, 0x0f, 
0xf0, 0x60, 0x00, 0x7f, 0xfc, 0x0e, 0x0c, 0x1c, 0x0c, 0x18, 0x00, 0xc0, 0xff, 0xe0, 0xc0, 0x06, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char rewinchlogomini[]= {
  // 'rewinch_logo_supermini_32x14', 32x14px
  0x7f, 0xf8, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x80, 0x04, 0x00, 0x08, 
  0x80, 0x04, 0x00, 0x08, 0x99, 0x91, 0x5c, 0x6e, 0xa2, 0x55, 0x52, 0x89, 0xa3, 0x9b, 0x52, 0x89, 
  0xa1, 0xd1, 0x52, 0x69, 0x80, 0x04, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 
  0xc0, 0x0c, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00

  // 'Rewinch Logo 004_56x24', 56x24px
/*0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 
0x07, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 
0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 
0xc0, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0xc0, 0xcf, 0x8e, 0x23, 0x16, 0x7c, 0x1e, 0xfc, 0xdf, 
0x9f, 0x30, 0x36, 0xfe, 0x3e, 0xfe, 0xd8, 0x31, 0xb0, 0x36, 0xe7, 0x60, 0xc3, 0xd8, 0x31, 0x93, 
0x26, 0xc3, 0x60, 0xc3, 0xd8, 0x3f, 0x07, 0x86, 0xc3, 0x60, 0xc3, 0xd8, 0x30, 0x0c, 0xc6, 0xc3, 
0x60, 0xc3, 0xd8, 0x1f, 0x98, 0x66, 0xc3, 0x3e, 0xc3, 0xd8, 0x0f, 0x10, 0x26, 0xc3, 0x1e, 0xc3, 
0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x00, 
0x00, 0x00, 0x00, 0xe0, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00*/
};

// 'sending_', 28x12px
const unsigned char sending[]= {
  0x20, 0x00, 0x00, 0x40, 0x42, 0x00, 0x04, 0x20, 0x84, 0x00, 0x02, 0x10, 0x88, 0x40, 0x21, 0x10, 
  0x88, 0x86, 0x11, 0x10, 0x88, 0x8f, 0x11, 0x10, 0x88, 0x8f, 0x11, 0x10, 0x88, 0x86, 0x11, 0x10, 
  0x88, 0x40, 0x21, 0x10, 0x84, 0x00, 0x02, 0x10, 0x42, 0x00, 0x04, 0x20, 0x20, 0x00, 0x00, 0x40
};

// 'sending_small', 20x10px
const unsigned char sending_s[]= {
0x20, 0x00, 0x40, 0x40, 0x00, 0x20, 0x84, 0x02, 0x10, 0x88, 0x61, 0x10, 0x88, 0xf1, 0x10, 0x88, 
0xf1, 0x10, 0x88, 0x61, 0x10, 0x84, 0x02, 0x10, 0x40, 0x00, 0x20, 0x20, 0x00, 0x40
};