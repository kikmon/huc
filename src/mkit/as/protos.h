#if defined(_MSC_VER)
#	define strcasecmp _stricmp
#endif

/* ASSEMBLE.C */
void assemble(int do_label);
int  oplook(int *idx);
void addinst(struct t_opcode *optbl);
int  check_eol(int *ip);
void save_if_expr(int *ip);
void do_if(int *ip);
void do_else(int *ip);
void do_endif(int *ip);
void do_ifdef(int *ip);

/* CODE.C */
void classC(int *ip);
void classR(int *ip);
void class1(int *ip);
void class2(int *ip);
void class3(int *ip);
void class4(int *ip);
void class5(int *ip);
void class6(int *ip);
void class7(int *ip);
void class8(int *ip);
void class9(int *ip);
void class10(int *ip);
int  getoperand(int *ip, int flag, int last_char);
int  getstring(int *ip, char *buffer, int size);
int  branchopt(void);

/* COMMAND.C */
void do_pseudo(int *ip);
void do_list(int *ip);
void do_mlist(int *ip);
void do_nolist(int *ip);
void do_nomlist(int *ip);
void do_db(int *ip);
void do_dw(int *ip);
void do_dd(int *ip);
void do_dwl(int *ip);
void do_dwh(int *ip);
void do_equ(int *ip);
void do_page(int *ip);
void do_org(int *ip);
void do_bank(int *ip);
void do_incbin(int *ip);
void do_mx(char *fname);
void forget_included_files(void);
void do_include(int *ip);
void do_rsset(int *ip);
void do_rs(int *ip);
void do_ds(int *ip);
void do_fail(int *ip);
void do_section(int *ip);
void do_incchr(int *ip);
void do_opt(int *ip);
void do_align(int *ip);
void do_kickc(int *ip);
void do_cpu(int *ip);
void do_segment(int *ip);
void do_star(int *ip);
void do_label(int *ip);
void do_encoding(int *ip);
void do_struct(int *ip);
void do_ends(int *ip);
int  htoi(char *str, int nb);

/* CRC.C */
void         crc_init(void);
unsigned int crc_calc(unsigned char *data, int len);

/* EXPR.C */
int  evaluate(int *ip, char flag, char allow_bank);
int  push_val(int type);
int  getsym(struct t_symbol * curscope);
int  check_keyword(void);
int  push_op(int op);
int  do_op(void);
int  check_func_args(char *func_name);

/* FUNC.C */
void do_func(int *ip);
int  func_look(void);
int  func_install(int ip);
int  func_extract(int ip);
int  func_getargs(void);

/* INPUT.C */
int   add_path(char*, int);
void  cleanup_path(void);
int   init_path(void);
int   readline(void);
int   open_input(char *name);
int   close_input(void);
FILE *open_file(char *fname, char *mode);

/* MACRO.C */
void do_macro(int *ip);
void do_endm(int *ip);
struct t_macro *macro_look(int *ip);
int  macro_getargs(int ip);
int  macro_install(void);
int  macro_getargtype(char *arg);

/* MAIN.C */
int  main(int argc, char **argv);
int  calc_bank_base(void);
void help(void);
void show_bnk_usage(int which_bank);
void show_seg_usage(void);

/* MAP.C */
int pce_load_map(char *fname, int mode);
int pce_load_stm(char *fname, int mode);

/* OUTPUT.C */
void println(void);
void clearln(void);
void loadlc(int offset, int f);
void hexcon(int digit, int num);
void putbyte(int offset, int data);
void putword(int offset, int data);
void putdword(int offset, int data);
void putbuffer(void *data, int size);
void write_srec(char *fname, char *ext, int base);
void error(char *stptr);
void warning(char *stptr);
void fatal_error(char *stptr);

/* PCX.C */
int  pcx_pack_8x8_tile(unsigned char *buffer, int x, int y);
int  pcx_pack_16x16_tile(unsigned char *buffer, int x, int y);
int  pcx_pack_16x16_sprite(unsigned char *buffer, int x, int y);
int  pcx_set_tile(struct t_symbol *ref, unsigned int offset);
int  pcx_search_tile(unsigned char *data, int size);
int  pcx_get_args(int *ip);
int  pcx_parse_args(int i, int nb, int *a, int *b, int *c, int *d, int size);
int  pcx_load(char *name);
void decode_256(FILE *fp, int w, int h);
void decode_16(FILE *fp, int w, int h);
int  png_load(char *name);
int  bmp_load(char *name);

/* PROC.C */
void do_call(int *ip);
void do_leave(int *ip);
void do_proc(int *ip);
void do_endp(int *ip);
void proc_reloc(void);
void proc_reloc(void);
int check_trampolines(void);

/* SYMBOL.C */
int  symhash(void);
int  addscope(struct t_symbol * curscope, int i);
int  colsym(int *ip, int flag);
struct t_symbol *stlook(int flag);
struct t_symbol *stinstall(int hash, int type);
int  labldef(int lval, int lbnk, int lsrc);
void lablset(char *name, int val);
int  lablexists(char *name);
void lablremap(void);
void labldump(FILE *fp);
void lablresetdefcnt(void);
