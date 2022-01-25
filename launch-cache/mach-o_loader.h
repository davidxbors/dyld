struct load_command {
	unsigned long cmd;		/* type of load command */
	unsigned long cmdsize;		/* total size of command in bytes */
};

/* Constants for the cmd field of all load commands, the type */
#define	LC_SEGMENT	0x1	/* segment of this file to be mapped */
#define	LC_SYMTAB	0x2	/* link-edit stab symbol table info */
#define	LC_SYMSEG	0x3	/* link-edit gdb symbol table info (obsolete) */
#define	LC_THREAD	0x4	/* thread */
#define	LC_UNIXTHREAD	0x5	/* unix thread (includes a stack) */
#define	LC_LOADFVMLIB	0x6	/* load a specified fixed VM shared library */
#define	LC_IDFVMLIB	0x7	/* fixed VM shared library identification */
#define	LC_IDENT	0x8	/* object identification info (obsolete) */
#define LC_FVMFILE	0x9	/* fixed VM file inclusion (internal use) */
#define LC_PREPAGE      0xa     /* prepage command (internal use) */
#define	LC_DYSYMTAB	0xb	/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB	0xc	/* load a dynamicly linked shared library */
#define	LC_ID_DYLIB	0xd	/* dynamicly linked shared lib identification */
#define LC_LOAD_DYLINKER 0xe	/* load a dynamic linker */
#define LC_ID_DYLINKER	0xf	/* dynamic linker identification */
#define	LC_PREBOUND_DYLIB 0x10	/* modules prebound for a dynamicly */
				/*  linked shared library */
#define LC_SEGMENT_64				0x19

/*
 * The 64-bit segment load command indicates that a part of this file is to be
 * mapped into a 64-bit task's address space.  If the 64-bit segment has
 * sections then section_64 structures directly follow the 64-bit segment
 * command and their size is reflected in cmdsize.
 */
typedef int		vm_prot_t;
struct segment_command_64 { /* for 64-bit architectures */
	uint32_t	cmd;		/* LC_SEGMENT_64 */
	uint32_t	cmdsize;	/* includes sizeof section_64 structs */
	char		segname[16];	/* segment name */
	uint64_t	vmaddr;		/* memory address of this segment */
	uint64_t	vmsize;		/* memory size of this segment */
	uint64_t	fileoff;	/* file offset of this segment */
	uint64_t	filesize;	/* amount to map from the file */
	vm_prot_t	maxprot;	/* maximum VM protection */
	vm_prot_t	initprot;	/* initial VM protection */
	uint32_t	nsects;		/* number of sections in segment */
	uint32_t	flags;		/* flags */
};

/*
 * The segment load command indicates that a part of this file is to be
 * mapped into the task's address space.  The size of this segment in memory,
 * vmsize, maybe equal to or larger than the amount to map from this file,
 * filesize.  The file is mapped starting at fileoff to the beginning of
 * the segment in memory, vmaddr.  The rest of the memory of the segment,
 * if any, is allocated zero fill on demand.  The segment's maximum virtual
 * memory protection and initial virtual memory protection are specified
 * by the maxprot and initprot fields.  If the segment has sections then the
 * section structures directly follow the segment command and their size is
 * reflected in cmdsize.
 */
struct segment_command {
	unsigned long	cmd;		/* LC_SEGMENT */
	unsigned long	cmdsize;	/* includes sizeof section structs */
	char		segname[16];	/* segment name */
	unsigned long	vmaddr;		/* memory address of this segment */
	unsigned long	vmsize;		/* memory size of this segment */
	unsigned long	fileoff;	/* file offset of this segment */
	unsigned long	filesize;	/* amount to map from the file */
  	vm_prot_t	maxprot;	/* maximum VM protection */
  	vm_prot_t	initprot;	/* initial VM protection */
	unsigned long	nsects;		/* number of sections in segment */
	unsigned long	flags;		/* flags */
};

/* Constants for the flags field of the segment_command */
#define	SG_HIGHVM	0x1	/* the file contents for this segment is for
				   the high part of the VM space, the low part
				   is zero filled (for stacks in core files) */
#define	SG_FVMLIB	0x2	/* this segment is the VM that is allocated by
				   a fixed VM library, for overlap checking in
				   the link editor */
#define	SG_NORELOC	0x4	/* this segment has nothing that was relocated
				   in it and nothing relocated to it, that is
				   it maybe safely replaced without relocation*/

/*
 * A segment is made up of zero or more sections.  Non-MH_OBJECT files have
 * all of their segments with the proper sections in each, and padded to the
 * specified segment alignment when produced by the link editor.  The first
 * segment of a MH_EXECUTE and MH_FVMLIB format file contains the mach_header
 * and load commands of the object file before it's first section.  The zero
 * fill sections are always last in their segment (in all formats).  This
 * allows the zeroed segment padding to be mapped into memory where zero fill
 * sections might be.
 *
 * The MH_OBJECT format has all of it's sections in one segment for
 * compactness.  There is no padding to a specified segment boundary and the
 * mach_header and load commands are not part of the segment.
 *
 * Sections with the same section name, sectname, going into the same segment,
 * segname, are combined by the link editor.  The resulting section is aligned
 * to the maximum alignment of the combined sections and is the new section's
 * alignment.  The combined sections are aligned to their original alignment in
 * the combined section.  Any padded bytes to get the specified alignment are
 * zeroed.
 *
 * The format of the relocation entries referenced by the reloff and nreloc
 * fields of the section structure for mach object files is described in the
 * header file <reloc.h>.
 */
struct section {
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	unsigned long	addr;		/* memory address of this section */
	unsigned long	size;		/* size in bytes of this section */
	unsigned long	offset;		/* file offset of this section */
	unsigned long	align;		/* section alignment (power of 2) */
	unsigned long	reloff;		/* file offset of relocation entries */
	unsigned long	nreloc;		/* number of relocation entries */
	unsigned long	flags;		/* flags (section type and attributes)*/
	unsigned long	reserved1;	/* reserved */
	unsigned long	reserved2;	/* reserved */
};

/*
 * The flags field of a section structure is separated into two parts a section
 * type and section attributes.  The section types are mutually exclusive (it
 * can only have one type) but the section attributes are not (it may have more
 * than one attribute).
 */
#define SECTION_TYPE		 0x000000ff	/* 256 section types */
#define SECTION_ATTRIBUTES	 0xffffff00	/*  24 section attributes */

/* Constants for the type of a section */
#define	S_REGULAR		0x0	/* regular section */
#define	S_ZEROFILL		0x1	/* zero fill on demand section */
#define	S_CSTRING_LITERALS	0x2	/* section with only literal C strings*/
#define	S_4BYTE_LITERALS	0x3	/* section with only 4 byte literals */
#define	S_8BYTE_LITERALS	0x4	/* section with only 8 byte literals */
#define	S_LITERAL_POINTERS	0x5	/* section with only pointers to */
					/*  literals */
/*
 * For the two types of symbol pointers sections and the symbol stubs section
 * they have indirect symbol table entries.  For each of the entries in the
 * section the indirect symbol table entries, in corresponding order in the
 * indirect symbol table, start at the index stored in the reserved1 field
 * of the section structure.  Since the indirect symbol table entries
 * correspond to the entries in the section the number of indirect symbol table
 * entries is inferred from the size of the section divided by the size of the
 * entries in the section.  For symbol pointers sections the size of the entries
 * in the section is 4 bytes and for symbol stubs sections the byte size of the
 * stubs is stored in the reserved2 field of the section structure.
 */
#define	S_NON_LAZY_SYMBOL_POINTERS	0x6	/* section with only non-lazy
						   symbol pointers */
#define	S_LAZY_SYMBOL_POINTERS		0x7	/* section with only lazy symbol
						   pointers */
#define	S_SYMBOL_STUBS			0x8	/* section with only symbol
						   stubs, byte size of stub in
						   the reserved2 field */
#define	S_MOD_INIT_FUNC_POINTERS	0x9	/* section with only function
						   pointers for initialization*/
/*
 * Constants for the section attributes part of the flags field of a section
 * structure.
 */
#define SECTION_ATTRIBUTES_USR	 0xff000000	/* User setable attributes */
#define S_ATTR_PURE_INSTRUCTIONS 0x80000000	/* section contains only true
						   machine instructions */
#define SECTION_ATTRIBUTES_SYS	 0x00ffff00	/* system setable attributes */
#define S_ATTR_SOME_INSTRUCTIONS 0x00000400	/* section contains some
						   machine instructions */
#define S_ATTR_EXT_RELOC	 0x00000200	/* section has external
						   relocation entries */
#define S_ATTR_LOC_RELOC	 0x00000100	/* section has local
						   relocation entries */


/*
 * The names of segments and sections in them are mostly meaningless to the
 * link-editor.  But there are few things to support traditional UNIX
 * executables that require the link-editor and assembler to use some names
 * agreed upon by convention.
 *
 * The initial protection of the "__TEXT" segment has write protection turned
 * off (not writeable).
 *
 * The link-editor will allocate common symbols at the end of the "__common"
 * section in the "__DATA" segment.  It will create the section and segment
 * if needed.
 */

/* The currently known segment names and the section names in those segments */

#define	SEG_PAGEZERO	"__PAGEZERO"	/* the pagezero segment which has no */
					/* protections and catches NULL */
					/* references for MH_EXECUTE files */


#define	SEG_TEXT	"__TEXT"	/* the tradition UNIX text segment */
#define	SECT_TEXT	"__text"	/* the real text part of the text */
					/* section no headers, and no padding */
#define SECT_FVMLIB_INIT0 "__fvmlib_init0"	/* the fvmlib initialization */
						/*  section */
#define SECT_FVMLIB_INIT1 "__fvmlib_init1"	/* the section following the */
					        /*  fvmlib initialization */
						/*  section */

#define	SEG_DATA	"__DATA"	/* the tradition UNIX data segment */
#define	SECT_DATA	"__data"	/* the real initialized data section */
					/* no padding, no bss overlap */
#define	SECT_BSS	"__bss"		/* the real uninitialized data section*/
					/* no padding */
#define SECT_COMMON	"__common"	/* the section common symbols are */
					/* allocated in by the link editor */

#define	SEG_OBJC	"__OBJC"	/* objective-C runtime segment */
#define SECT_OBJC_SYMBOLS "__symbol_table"	/* symbol table */
#define SECT_OBJC_MODULES "__module_info"	/* module information */
#define SECT_OBJC_STRINGS "__selector_strs"	/* string table */
#define SECT_OBJC_REFS "__selector_refs"	/* string table */

#define	SEG_ICON	 "__ICON"	/* the NeXT icon segment */
#define	SECT_ICON_HEADER "__header"	/* the icon headers */
#define	SECT_ICON_TIFF   "__tiff"	/* the icons in tiff format */

#define	SEG_LINKEDIT	"__LINKEDIT"	/* the segment containing all structs */
					/* created and maintained by the link */
					/* editor.  Created with -seglinkedit */
					/* option to ld(1) for MH_EXECUTE and */
					/* FVMLIB file types only */

#define SEG_UNIXSTACK	"__UNIXSTACK"	/* the unix stack segment */

struct section_64 {
   char sectname[16];
   char segname[16];
   uint64_t addr;
   uint64_t size;
   uint32_t offset;
   uint32_t align;
   uint32_t reloff;
   uint32_t nreloc;
   uint32_t flags;
   uint32_t reserved1;
   uint32_t reserved2;
   uint32_t reserved3;
};

/*
 * A variable length string in a load command is represented by an lc_str
 * union.  The strings are stored just after the load command structure and
 * the offset is from the start of the load command structure.  The size
 * of the string is reflected in the cmdsize field of the load command.
 * Once again any padded bytes to bring the cmdsize field to a multiple
 * of sizeof(long) must be zero.
 */
union lc_str {
	unsigned long	offset;	/* offset to the string */
	char		*ptr;	/* pointer to the string */
};

/*
 * Dynamicly linked shared libraries are identified by two things.  The
 * pathname (the name of the library as found for execution), and the
 * compatibility version number.  The pathname must match and the compatibility
 * number in the user of the library must be greater than or equal to the
 * library being used.  The time stamp is used to record the time a library was
 * built and copied into user so it can be use to determined if the library used
 * at runtime is exactly the same as used to built the program.
 */
struct dylib {
    union lc_str  name;			/* library's path name */
    unsigned long timestamp;		/* library's build time stamp */
    unsigned long current_version;	/* library's current version number */
    unsigned long compatibility_version;/* library's compatibility vers number*/
};

/*
 * A dynamicly linked shared library (filetype == MH_DYLIB in the mach header)
 * contains a dylib_command (cmd == LC_ID_DYLIB) to identify the library.
 * An object that uses a dynamicly linked shared library also contains a
 * dylib_command (cmd == LC_LOAD_DYLIB) for each library it uses.
 */
struct dylib_command {
	unsigned long	cmd;		/* LC_ID_DYLIB or LC_LOAD_DYLIB */
	unsigned long	cmdsize;	/* includes pathname string */
	struct dylib	dylib;		/* the library identification */
};

/*
 * A program (filetype == MH_EXECUTE) or bundle (filetype == MH_BUNDLE) that is
 * prebound to it's dynamic libraries has one of these for each library that
 * the static linker used in prebinding.  It contains a bit vector for the
 * modules in the library.  The bits indicate which modules are bound (1) and
 * which are not (0) from the library.  The bit for module 0 is the low bit
 * of the first byte.  So the bit for the Nth module is:
 * (linked_modules[N/8] >> N%8) & 1
 */
struct prebound_dylib_command {
	unsigned long	cmd;		/* LC_PREBOUND_DYLIB */
	unsigned long	cmdsize;	/* includes strings */
	union lc_str	name;		/* library's path name */
	unsigned long	nmodules;	/* number of modules in library */
	union lc_str	linked_modules;	/* bit vector of linked modules */
};

/*
 * A program that uses a dynamic linker contains a dylinker_command to identify
 * the name of the dynamic linker (LC_LOAD_DYLINKER).  And a dynamic linker
 * contains a dylinker_command to identify the dynamic linker (LC_ID_DYLINKER).
 * A file can have at most one of these.
 */
struct dylinker_command {
	unsigned long	cmd;		/* LC_ID_DYLINKER or LC_LOAD_DYLINKER */
	unsigned long	cmdsize;	/* includes pathname string */
	union lc_str    name;		/* dynamic linker's path name */
};

struct sub_framework_command {
   uint32_t cmd;
   uint32_t cmdsize;
   union lc_str umbrella;
};

#define	LC_SUB_FRAMEWORK 	                0x12

struct sub_client_command {
   uint32_t cmd;
   uint32_t cmdsize;
   union lc_str client;
};

#define	LC_SUB_CLIENT		                0x14

struct sub_umbrella_command {
   uint32_t cmd;
   uint32_t cmdsize;
   union lc_str sub_umbrella;
};

#define	LC_SUB_UMBRELLA 	                0x13

struct sub_library_command {
   uint32_t cmd;
   uint32_t cmdsize;
   union lc_str sub_library;
};

#define	LC_SUB_LIBRARY  	                0x15

struct routines_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t init_address;
   uint32_t init_module;
   uint32_t reserved1;
   uint32_t reserved2;
   uint32_t reserved3;
   uint32_t reserved4;
   uint32_t reserved5;
   uint32_t reserved6;
};

#define	LC_ROUTINES			        0x11

struct routines_command_64 {
   uint32_t cmd;
   uint32_t cmdsize;
   uint64_t init_address;
   uint64_t init_module;
   uint64_t reserved1;
   uint64_t reserved2;
   uint64_t reserved3;
   uint64_t reserved4;
   uint64_t reserved5;
   uint64_t reserved6;
};

#define	LC_ROUTINES_64				0x1a

struct symtab_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t symoff;
   uint32_t nsyms;
   uint32_t stroff;
   uint32_t strsize;
};

#define	LC_SYMTAB			        0x2

struct dysymtab_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t ilocalsym;
   uint32_t nlocalsym;
   uint32_t iextdefsym;
   uint32_t nextdefsym;
   uint32_t iundefsym;
   uint32_t nundefsym;
   uint32_t tocoff;
   uint32_t ntoc;
   uint32_t modtaboff;
   uint32_t nmodtab;
   uint32_t extrefsymoff;
   uint32_t nextrefsyms;
   uint32_t indirectsymoff;
   uint32_t nindirectsyms;
   uint32_t extreloff;
   uint32_t nextrel;
   uint32_t locreloff;
   uint32_t nlocrel;
};

#define	LC_DYSYMTAB			        0xb

/*
Original code from apple library
typedef integer_t	cpu_type_t;
typedef integer_t	cpu_subtype_t;
*/

typedef int cpu_type_t;
typedef int cpu_subtype_t;

#define CPU_STATE_MAX		4

#define CPU_STATE_USER		0
#define CPU_STATE_SYSTEM	1
#define CPU_STATE_IDLE		2
#define CPU_STATE_NICE		3

/*
 * Capability bits used in the definition of cpu_type.
 */
#define	CPU_ARCH_MASK	0xff000000		/* mask for architecture bits */
#define CPU_ARCH_ABI64	0x01000000		/* 64 bit ABI */

/*
 *	Machine types known by all.
 */
 
#define CPU_TYPE_ANY		((cpu_type_t) -1)

#define CPU_TYPE_VAX		((cpu_type_t) 1)
/* skip				((cpu_type_t) 2)	*/
/* skip				((cpu_type_t) 3)	*/
/* skip				((cpu_type_t) 4)	*/
/* skip				((cpu_type_t) 5)	*/
#define	CPU_TYPE_MC680x0	((cpu_type_t) 6)
#define CPU_TYPE_X86		((cpu_type_t) 7)
#define CPU_TYPE_I386		CPU_TYPE_X86		/* compatibility */
#define	CPU_TYPE_X86_64		(CPU_TYPE_X86 | CPU_ARCH_ABI64)

/* skip CPU_TYPE_MIPS		((cpu_type_t) 8)	*/
/* skip 			((cpu_type_t) 9)	*/
#define CPU_TYPE_MC98000	((cpu_type_t) 10)
#define CPU_TYPE_HPPA           ((cpu_type_t) 11)
#define CPU_TYPE_ARM		((cpu_type_t) 12)
#define CPU_TYPE_ARM64          (CPU_TYPE_ARM | CPU_ARCH_ABI64)
#define CPU_TYPE_MC88000	((cpu_type_t) 13)
#define CPU_TYPE_SPARC		((cpu_type_t) 14)
#define CPU_TYPE_I860		((cpu_type_t) 15)
/* skip	CPU_TYPE_ALPHA		((cpu_type_t) 16)	*/
/* skip				((cpu_type_t) 17)	*/
#define CPU_TYPE_POWERPC		((cpu_type_t) 18)
#define CPU_TYPE_POWERPC64		(CPU_TYPE_POWERPC | CPU_ARCH_ABI64)

/*
 *	Machine subtypes (these are defined here, instead of in a machine
 *	dependent directory, so that any program can get all definitions
 *	regardless of where is it compiled).
 */

/*
 * Capability bits used in the definition of cpu_subtype.
 */
#define CPU_SUBTYPE_MASK	0xff000000	/* mask for feature flags */
#define CPU_SUBTYPE_LIB64	0x80000000	/* 64 bit libraries */


/*
 *	Object files that are hand-crafted to run on any
 *	implementation of an architecture are tagged with
 *	CPU_SUBTYPE_MULTIPLE.  This functions essentially the same as
 *	the "ALL" subtype of an architecture except that it allows us
 *	to easily find object files that may need to be modified
 *	whenever a new implementation of an architecture comes out.
 *
 *	It is the responsibility of the implementor to make sure the
 *	software handles unsupported implementations elegantly.
 */
#define	CPU_SUBTYPE_MULTIPLE		((cpu_subtype_t) -1)
#define CPU_SUBTYPE_LITTLE_ENDIAN	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_BIG_ENDIAN		((cpu_subtype_t) 1)

/*
 *     Machine threadtypes.
 *     This is none - not defined - for most machine types/subtypes.
 */
#define CPU_THREADTYPE_NONE		((cpu_threadtype_t) 0)

/*
 *	VAX subtypes (these do *not* necessary conform to the actual cpu
 *	ID assigned by DEC available via the SID register).
 */

#define	CPU_SUBTYPE_VAX_ALL	((cpu_subtype_t) 0) 
#define CPU_SUBTYPE_VAX780	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_VAX785	((cpu_subtype_t) 2)
#define CPU_SUBTYPE_VAX750	((cpu_subtype_t) 3)
#define CPU_SUBTYPE_VAX730	((cpu_subtype_t) 4)
#define CPU_SUBTYPE_UVAXI	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_UVAXII	((cpu_subtype_t) 6)
#define CPU_SUBTYPE_VAX8200	((cpu_subtype_t) 7)
#define CPU_SUBTYPE_VAX8500	((cpu_subtype_t) 8)
#define CPU_SUBTYPE_VAX8600	((cpu_subtype_t) 9)
#define CPU_SUBTYPE_VAX8650	((cpu_subtype_t) 10)
#define CPU_SUBTYPE_VAX8800	((cpu_subtype_t) 11)
#define CPU_SUBTYPE_UVAXIII	((cpu_subtype_t) 12)

/*
 * 	680x0 subtypes
 *
 * The subtype definitions here are unusual for historical reasons.
 * NeXT used to consider 68030 code as generic 68000 code.  For
 * backwards compatability:
 * 
 *	CPU_SUBTYPE_MC68030 symbol has been preserved for source code
 *	compatability.
 *
 *	CPU_SUBTYPE_MC680x0_ALL has been defined to be the same
 *	subtype as CPU_SUBTYPE_MC68030 for binary comatability.
 *
 *	CPU_SUBTYPE_MC68030_ONLY has been added to allow new object
 *	files to be tagged as containing 68030-specific instructions.
 */

#define	CPU_SUBTYPE_MC680x0_ALL		((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MC68030		((cpu_subtype_t) 1) /* compat */
#define CPU_SUBTYPE_MC68040		((cpu_subtype_t) 2) 
#define	CPU_SUBTYPE_MC68030_ONLY	((cpu_subtype_t) 3)

/*
 *	I386 subtypes
 */

#define CPU_SUBTYPE_INTEL(f, m)	((cpu_subtype_t) (f) + ((m) << 4))

#define	CPU_SUBTYPE_I386_ALL			CPU_SUBTYPE_INTEL(3, 0)
#define CPU_SUBTYPE_386					CPU_SUBTYPE_INTEL(3, 0)
#define CPU_SUBTYPE_486					CPU_SUBTYPE_INTEL(4, 0)
#define CPU_SUBTYPE_486SX				CPU_SUBTYPE_INTEL(4, 8)	// 8 << 4 = 128
#define CPU_SUBTYPE_586					CPU_SUBTYPE_INTEL(5, 0)
#define CPU_SUBTYPE_PENT	CPU_SUBTYPE_INTEL(5, 0)
#define CPU_SUBTYPE_PENTPRO	CPU_SUBTYPE_INTEL(6, 1)
#define CPU_SUBTYPE_PENTII_M3	CPU_SUBTYPE_INTEL(6, 3)
#define CPU_SUBTYPE_PENTII_M5	CPU_SUBTYPE_INTEL(6, 5)
#define CPU_SUBTYPE_CELERON				CPU_SUBTYPE_INTEL(7, 6)
#define CPU_SUBTYPE_CELERON_MOBILE		CPU_SUBTYPE_INTEL(7, 7)
#define CPU_SUBTYPE_PENTIUM_3			CPU_SUBTYPE_INTEL(8, 0)
#define CPU_SUBTYPE_PENTIUM_3_M			CPU_SUBTYPE_INTEL(8, 1)
#define CPU_SUBTYPE_PENTIUM_3_XEON		CPU_SUBTYPE_INTEL(8, 2)
#define CPU_SUBTYPE_PENTIUM_M			CPU_SUBTYPE_INTEL(9, 0)
#define CPU_SUBTYPE_PENTIUM_4			CPU_SUBTYPE_INTEL(10, 0)
#define CPU_SUBTYPE_PENTIUM_4_M			CPU_SUBTYPE_INTEL(10, 1)
#define CPU_SUBTYPE_ITANIUM				CPU_SUBTYPE_INTEL(11, 0)
#define CPU_SUBTYPE_ITANIUM_2			CPU_SUBTYPE_INTEL(11, 1)
#define CPU_SUBTYPE_XEON				CPU_SUBTYPE_INTEL(12, 0)
#define CPU_SUBTYPE_XEON_MP				CPU_SUBTYPE_INTEL(12, 1)

#define CPU_SUBTYPE_INTEL_FAMILY(x)	((x) & 15)
#define CPU_SUBTYPE_INTEL_FAMILY_MAX	15

#define CPU_SUBTYPE_INTEL_MODEL(x)	((x) >> 4)
#define CPU_SUBTYPE_INTEL_MODEL_ALL	0

/*
 *	X86 subtypes.
 */

#define CPU_SUBTYPE_X86_ALL		((cpu_subtype_t)3)
#define CPU_SUBTYPE_X86_64_ALL		((cpu_subtype_t)3)
#define CPU_SUBTYPE_X86_ARCH1		((cpu_subtype_t)4)
#define CPU_SUBTYPE_X86_64_H		((cpu_subtype_t)8)	/* Haswell feature subset */


#define CPU_THREADTYPE_INTEL_HTT	((cpu_threadtype_t) 1)

/*
 *	Mips subtypes.
 */

#define	CPU_SUBTYPE_MIPS_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MIPS_R2300	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MIPS_R2600	((cpu_subtype_t) 2)
#define CPU_SUBTYPE_MIPS_R2800	((cpu_subtype_t) 3)
#define CPU_SUBTYPE_MIPS_R2000a	((cpu_subtype_t) 4)	/* pmax */
#define CPU_SUBTYPE_MIPS_R2000	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_MIPS_R3000a	((cpu_subtype_t) 6)	/* 3max */
#define CPU_SUBTYPE_MIPS_R3000	((cpu_subtype_t) 7)

/*
 *	MC98000 (PowerPC) subtypes
 */
#define	CPU_SUBTYPE_MC98000_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MC98601	((cpu_subtype_t) 1)

/*
 *	HPPA subtypes for Hewlett-Packard HP-PA family of
 *	risc processors. Port by NeXT to 700 series. 
 */

#define	CPU_SUBTYPE_HPPA_ALL		((cpu_subtype_t) 0)
#define CPU_SUBTYPE_HPPA_7100		((cpu_subtype_t) 0) /* compat */
#define CPU_SUBTYPE_HPPA_7100LC		((cpu_subtype_t) 1)

/*
 *	MC88000 subtypes.
 */
#define	CPU_SUBTYPE_MC88000_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MC88100	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MC88110	((cpu_subtype_t) 2)

/*
 *	SPARC subtypes
 */
#define	CPU_SUBTYPE_SPARC_ALL		((cpu_subtype_t) 0)

/*
 *	I860 subtypes
 */
#define CPU_SUBTYPE_I860_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_I860_860	((cpu_subtype_t) 1)

/*
 *	PowerPC subtypes
 */
#define CPU_SUBTYPE_POWERPC_ALL		((cpu_subtype_t) 0)
#define CPU_SUBTYPE_POWERPC_601		((cpu_subtype_t) 1)
#define CPU_SUBTYPE_POWERPC_602		((cpu_subtype_t) 2)
#define CPU_SUBTYPE_POWERPC_603		((cpu_subtype_t) 3)
#define CPU_SUBTYPE_POWERPC_603e	((cpu_subtype_t) 4)
#define CPU_SUBTYPE_POWERPC_603ev	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_POWERPC_604		((cpu_subtype_t) 6)
#define CPU_SUBTYPE_POWERPC_604e	((cpu_subtype_t) 7)
#define CPU_SUBTYPE_POWERPC_620		((cpu_subtype_t) 8)
#define CPU_SUBTYPE_POWERPC_750		((cpu_subtype_t) 9)
#define CPU_SUBTYPE_POWERPC_7400	((cpu_subtype_t) 10)
#define CPU_SUBTYPE_POWERPC_7450	((cpu_subtype_t) 11)
#define CPU_SUBTYPE_POWERPC_970		((cpu_subtype_t) 100)

/*
 *	ARM subtypes
 */
#define CPU_SUBTYPE_ARM_ALL             ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM_V4T             ((cpu_subtype_t) 5)
#define CPU_SUBTYPE_ARM_V6              ((cpu_subtype_t) 6)
#define CPU_SUBTYPE_ARM_V5TEJ           ((cpu_subtype_t) 7)
#define CPU_SUBTYPE_ARM_XSCALE		((cpu_subtype_t) 8)
#define CPU_SUBTYPE_ARM_V7		((cpu_subtype_t) 9)
#define CPU_SUBTYPE_ARM_V7F		((cpu_subtype_t) 10) /* Cortex A9 */
#define CPU_SUBTYPE_ARM_V7S		((cpu_subtype_t) 11) /* Swift */
#define CPU_SUBTYPE_ARM_V7K		((cpu_subtype_t) 12)
#define CPU_SUBTYPE_ARM_V6M		((cpu_subtype_t) 14) /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7M		((cpu_subtype_t) 15) /* Not meant to be run under xnu */
#define CPU_SUBTYPE_ARM_V7EM		((cpu_subtype_t) 16) /* Not meant to be run under xnu */

#define CPU_SUBTYPE_ARM_V8		((cpu_subtype_t) 13)

/*
 *  ARM64 subtypes
 */
#define CPU_SUBTYPE_ARM64_ALL           ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM64_V8            ((cpu_subtype_t) 1)

/*
 *	CPU families (sysctl hw.cpufamily)
 *
 * These are meant to identify the CPU's marketing name - an
 * application can map these to (possibly) localized strings.
 * NB: the encodings of the CPU families are intentionally arbitrary.
 * There is no ordering, and you should never try to deduce whether
 * or not some feature is available based on the family.
 * Use feature flags (eg, hw.optional.altivec) to test for optional
 * functionality.
 */
#define CPUFAMILY_UNKNOWN   		0
#define CPUFAMILY_POWERPC_G3		0xcee41549
#define CPUFAMILY_POWERPC_G4		0x77c184ae
#define CPUFAMILY_POWERPC_G5		0xed76d8aa
#define CPUFAMILY_INTEL_6_13		0xaa33392b
#define CPUFAMILY_INTEL_PENRYN		0x78ea4fbc
#define CPUFAMILY_INTEL_NEHALEM		0x6b5a4cd2
#define CPUFAMILY_INTEL_WESTMERE	0x573b5eec
#define CPUFAMILY_INTEL_SANDYBRIDGE	0x5490b78c
#define CPUFAMILY_INTEL_IVYBRIDGE	0x1f65e835
#define CPUFAMILY_INTEL_HASWELL		0x10b282dc
#define CPUFAMILY_INTEL_BROADWELL	0x582ed09c
#define CPUFAMILY_INTEL_SKYLAKE		0x37fc219f
#define CPUFAMILY_ARM_9			0xe73283ae
#define CPUFAMILY_ARM_11		0x8ff620d8
#define CPUFAMILY_ARM_XSCALE		0x53b005f5
#define CPUFAMILY_ARM_12                0xbd1b0ae9
#define CPUFAMILY_ARM_13		0x0cc90e64
#define CPUFAMILY_ARM_14		0x96077ef1
#define CPUFAMILY_ARM_15		0xa8511bca
#define CPUFAMILY_ARM_SWIFT 		0x1e2d6381
#define CPUFAMILY_ARM_CYCLONE		0x37a09642
#define CPUFAMILY_ARM_TYPHOON		0x2c91a47e
#define CPUFAMILY_ARM_TWISTER		0x92fb37c8
#define CPUFAMILY_ARM_HURRICANE		0x67ceee93

/* The following synonyms are deprecated: */
#define CPUFAMILY_INTEL_6_23	CPUFAMILY_INTEL_PENRYN
#define CPUFAMILY_INTEL_6_26	CPUFAMILY_INTEL_NEHALEM

/*
 * The mach header appears at the very beginning of the object file.
 */
struct mach_header {
	unsigned long	magic;		/* mach magic number identifier */
	cpu_type_t	cputype;	/* cpu specifier */
	cpu_subtype_t	cpusubtype;	/* machine specifier */
	unsigned long	filetype;	/* type of file */
	unsigned long	ncmds;		/* number of load commands */
	unsigned long	sizeofcmds;	/* the size of all the load commands */
	unsigned long	flags;		/* flags */
};

/* Constant for the magic field of the mach_header */
#define	MH_MAGIC	0xfeedface	/* the mach magic number */
#define MH_CIGAM	0xcefaedfe

struct mach_header_64 {
   uint32_t magic;
   cpu_type_t cputype;
   cpu_subtype_t cpusubtype;
   uint32_t filetype;
   uint32_t ncmds;
   uint32_t sizeofcmds;
   uint32_t flags;
   uint32_t reserved;
};

#define MH_MAGIC_64 0xfeedfacf
#define MH_CIGAM_64 0xcffaedfe

struct twolevel_hints_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t offset;
   uint32_t nhints;
};

#define	LC_TWOLEVEL_HINTS 	                0x16

struct thread_command {
   uint32_t cmd;
   uint32_t cmdsize;
   /* uint32_t flavor;*/
   /* uint32_t count; */
   /* struct cpu_thread_state state;*/
};

#define	LC_THREAD			        0x4
#define	LC_UNIXTHREAD		                0x5

 struct linkedit_data_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t dataoff;
   uint32_t datasize;
 };

struct dyld_info_command {
   uint32_t cmd;
   uint32_t cmdsize;
   uint32_t rebase_off;
   uint32_t rebase_size;
   uint32_t bind_off;
   uint32_t bind_size;
   uint32_t weak_bind_off;
   uint32_t weak_bind_size;
   uint32_t lazy_bind_off;
   uint32_t lazy_bind_size;
   uint32_t export_off;
   uint32_t export_size;
 };

#define	LC_DYLD_INFO 				0x22
#define	LC_DYLD_INFO_ONLY 			(0x22| 0x80000000)
