#include <stdio.h>
typedef struct exe Exe;

struct exe {
  char m; /* == 0x5a4D */
  char z;
  unsigned short bytes_in_last_block;
  unsigned short blocks_in_file;
  unsigned short num_relocs;
  unsigned short header_paragraphs;
  unsigned short min_extra_paragraphs;
  unsigned short max_extra_paragraphs;
  unsigned short ss;
  unsigned short sp;
  unsigned short checksum;
  unsigned short ip;
  unsigned short cs;
  unsigned short reloc_table_offset;
  unsigned short overlay_number;
};

typedef struct coff_head CoffHead;

struct coff_head {
  unsigned short f_magic;         /* magic number    0x14c    */
  unsigned short f_nscns;         /* number of sections       */
  unsigned int  f_timdat;        /* time & date stamp        */
  unsigned int  f_symptr;        /* file pointer to symtab   */
  unsigned int  f_nsyms;         /* number of symtab entries */
  unsigned short f_opthdr;        /* sizeof(optional hdr)     */
  unsigned short f_flags;         /* flags                    */
};


struct EXE_RELOC {
  unsigned short offset;
  unsigned short segment;
};

typedef struct aouthdr Aouthdr;
struct aouthdr {
  unsigned short magic;          /* type of file                         */
  unsigned short vstamp;         /* version stamp                        */
  unsigned int  tsize;          /* text size in bytes, padded to FW bdry*/
  unsigned int  dsize;          /* initialized data    "  "             */
  unsigned int  bsize;          /* uninitialized data  "  "             */
  unsigned int  entry;          /* entry pt.                            */
  unsigned int  text_start;     /* base of text used for this file      */
  unsigned int  data_start;     /* base of data used for this file      */
};



typedef struct scnhdr Scnhdr;

struct scnhdr{
  char           s_name[8];  /* section name                     */
  unsigned int  s_paddr;    /* physical address, aliased s_nlib */
  unsigned int  s_vaddr;    /* virtual address                  */
  unsigned int  s_size;     /* section size                     */
  unsigned int  s_scnptr;   /* file ptr to raw data for section */
  unsigned int  s_relptr;   /* file ptr to relocation           */
  unsigned int  s_lnnoptr;  /* file ptr to line numbers         */
  unsigned short s_nreloc;   /* number of relocation entries     */
  unsigned short s_nlnno;    /* number of line number entries    */
  unsigned int  s_flags;    /* flags                            */
};

int main (int argc, char *argv[])
{
  FILE *fp;  
  Exe my_exe;
  CoffHead my_coff_head;
  Aouthdr my_aouthdr;
  size_t amt_read;
  
  unsigned int exe_data_start;
  unsigned int extra_data_start;
  Scnhdr my_scnhdr[10];
  unsigned int scni;
  
  printf("sizeof(short)=%d\n", sizeof(short));
  printf("sizeof(int)=%d\n", sizeof(int));
  printf("sizeof(long)=%d\n", sizeof(long));
  
  if(argc < 2) {
    printf("\nusage: %s [file]\n", argv[0]);
    return 0;
  }
  printf("\ntest\n");
  fp = fopen(argv[1], "r");
  if(!fp) {/*zero if couldn't open */
    printf("\ncould not open file '%s'\n", argv[1]);
    return 0;
  }
  
  
  fread(&my_exe, sizeof(Exe), 1, fp);
  printf("\nsizeof(Exe)=%d\n", sizeof(Exe));
  
  printf("\nmagic number = %c%c\n", my_exe.m, my_exe.z);
  printf("bytes_in_last_block = 0x%x\n", my_exe.bytes_in_last_block);
  printf("blocks_in_file = 0x%x\n", my_exe.blocks_in_file);
  printf("num_relocs = 0x%x\n", my_exe.num_relocs);
  printf("header_paragraphs = 0x%x\n", my_exe.header_paragraphs);
  printf("min_extra_paragraphs = 0x%x\n", my_exe.min_extra_paragraphs);
  printf("max_extra_paragraphs = 0x%x\n", my_exe.max_extra_paragraphs);
  printf("ss = 0x%x\n", my_exe.ss);
  printf("sp = 0x%x\n", my_exe.sp);
  printf("checksum = 0x%x\n", my_exe.checksum);
  printf("ip = 0x%x\n", my_exe.ip);
  printf("cs = 0x%x\n", my_exe.cs);
  printf("reloc_table_offset = 0x%x\n", my_exe.reloc_table_offset);
  printf("overlay_number = 0x%x\n", my_exe.overlay_number);
  
  exe_data_start = my_exe.header_paragraphs * 16L;
  printf("exe_data_start = 0x%x\n", exe_data_start);
  
  extra_data_start = my_exe.blocks_in_file * 512L;
  if (my_exe.bytes_in_last_block)
    extra_data_start -= (512 - my_exe.bytes_in_last_block);
  printf("extra_data_start = 0x%x\n", extra_data_start);
  
  fseek(fp, extra_data_start, 0);
  
  fread(&my_coff_head, sizeof(CoffHead), 1, fp);
  printf("\nsizeof(CoffHead)=%d", sizeof(CoffHead));
  printf("\ncoff head magic number = 0x%x\n", my_coff_head.f_magic);
  printf("number of sections = 0x%x\n", my_coff_head.f_nscns);
  printf("time and date stamp = 0x%x\n", my_coff_head.f_timdat);
  printf("file pointer to symtab = 0x%x\n", my_coff_head.f_symptr);
  printf("number of symtab entries = 0x%x\n", my_coff_head.f_nsyms);
  printf("sizeof optional hdr = 0x%x\n", my_coff_head.f_opthdr);
  printf("flags = 0x%x\n", my_coff_head.f_flags);
  
  fread(&my_aouthdr, sizeof(Aouthdr), 1, fp);
  printf("\nsizeof(Aouthdr)=%d\n", sizeof(Aouthdr));
  printf("\naouthdr magic number = 0x%x\n", my_aouthdr.magic);
  printf("version stamp = 0x%x\n", my_aouthdr.vstamp);
  printf("text size (text section) = 0x%x\n", my_aouthdr.tsize);
  printf("initialized data (data section) = 0x%x\n", my_aouthdr.dsize);
  printf("uninitialized data (bss section) = 0x%x\n", my_aouthdr.bsize);
  printf("entry point = 0x%x\n", my_aouthdr.entry);
  printf("base of text (text section) = 0x%x\n", my_aouthdr.text_start);
  printf("base of data (data section) = 0x%x\n", my_aouthdr.data_start);
         
  fread(my_scnhdr, sizeof(Scnhdr), my_coff_head.f_nscns, fp);
  for(scni=0;scni<my_coff_head.f_nscns;scni++){
    printf("\nsection '%s'\n", my_scnhdr[scni].s_name);
    printf("s_paddr = 0x%x\n", my_scnhdr[scni].s_paddr);
    printf("s_vaddr = 0x%x\n", my_scnhdr[scni].s_vaddr);
    printf("s_size = 0x%x\n", my_scnhdr[scni].s_size);
    printf("s_scnptr = 0x%x\n", my_scnhdr[scni].s_scnptr);
    printf("s_relptr = 0x%x\n", my_scnhdr[scni].s_relptr);
    printf("s_lnnoptr = 0x%x\n", my_scnhdr[scni].s_lnnoptr);
    printf("s_nreloc = 0x%x\n", my_scnhdr[scni].s_nreloc);
    printf("s_nlnno = 0x%x\n", my_scnhdr[scni].s_nlnno);
    printf("s_flags = 0x%x\n", my_scnhdr[scni].s_flags);
  }
         
         
  /*
     end 
  */
  if(fclose(fp)) {/*zero if closed properly, non-zero if not */
    printf("\ncould not properly close file '%s'\n", argv[1]);    
  }
  return 0;
}