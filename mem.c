#include "defs.h"

u8 mem[MEMSIZE];
u8 pal[PALSIZE];
u8 *mem_ptr = mem;

int mem_load(u8 *at, char *fname) {

  FILE *f = fopen(fname, "rb");
  if (!f) {
    fprintf(stderr,
      "couldn't open %s\n", fname);
    return -1;
  }

  fseek(f, 0, SEEK_END);
  u16 len = ftell(f);
  rewind(f);
  u16 n = fread(at, 1, len, f);

  printf("%s : %u/%u bytes read\n",
    fname, n, len);

  fclose(f);
  return 0;
}

void mem_clear(u8 *a, u32 len) {

  for (u32 i=0; i<len; i++)
    a[i] = 0x55;
}

void mem_print(u8 *a, u32 len) {

  for (u16 i=0; i<len; i++) {
    if (i % 8 == 0) printf("[0x%04x] ", i);
    printf("0x%02x%s", a[i],
      i % 8 != 7 ? " " : "\n");
  }
}



void mem_1bpp(u8 *s, u8 *m, u16 w, u16 h) {

  u16 lines=w;
  u16 cols=h/8;
  u32 offset=0;
  for (u16 l=0; l<lines; l++) {
    for (u16 c=0; c<cols; c++) {
      u16 addr = cols*l+c;
      u8 v = m[addr];
      for (u16 i=0; i<8; i++) {
        u32 off = addr*8+i;
   u32 color_address = (offset>> 8 << 5) | (offset& 0x1f) + 0x80;   
   u32 color = pal[color_address] & 0x07;
        int val = (v>>i) &0x1;
	val=0xff;
//if (val) printf("val:%x color: %x color_address: %x\n",val,color,color_address);
        s[4*(off)+0] = val ? color&0x01 ? 0xFF:0x00:0x00;
        s[4*(off)+1] = val ? color&0x04 ? 0xFF :0x00:0x00;
        s[4*(off)+2] = val ? color&0x02 ? 0XFF :0x00:0x00;
        //s[4*(off)+0] = ((v>>i) & 0x1) ? 0xff : 0x00;
        //s[4*(off)+1] = ((v>>i) & 0x1) ? 0xff : 0x00;
        //s[4*(off)+2] = ((v>>i) & 0x1) ? 0xff : 0x00;
        //s[4*(off)+3] = ((v>>i) & 0x1) ? 0xff : 0x00;
        s[4*(off)+3] = ((v>>i) & 0x1) ? 0xff : 0x00; // doesn't seem used
      }
  	offset++;
    }
  }
}

void reg_print(cpu *c) {
  printf("%04x: [BC] [%02x%02x] [DE] [%02x%02x] "
         "[HL] [%02x%02x] [AF] [%02x%02x %c%c%c%c%c%c%c%c] "
         "[SP] [%04x] #[%08llu] OP=%02x : %-8s\n",
           PC, B,  C,  D,  E,
           H,  L,  A,  F,
           fS ? 'S' : '-',
           fZ ? 'Z' : '-',
           '0',
           fH ? 'H' : '-',
           '0',
           fP ? 'P' : '-',
           '1',
           fC ? 'C' : '-',
           SP, c->cycl, c->op, optxt[c->op]);
}
