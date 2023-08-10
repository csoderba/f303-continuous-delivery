/*
 * Mastering STM32 - Second Edition
 * 20.2.3 .rodata Section
 * Nucleo-F446RE/CH20/
 * main-ex4.c and ldscript4.ld
 * RCC_APB1ENR corrected to RCC_AHB1ENR
 * */

typedef unsigned long uint32_t;

/* memory and peripheral start addresses */
#define FLASH_BASE      0x08000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000

/* Work out end of RAM address as initial stack pointer */
#define SRAM_SIZE       128*1024     // STM32F446RE has 128 KB of RAM
#define SRAM_END        (SRAM_BASE + SRAM_SIZE)

/* RCC peripheral addresses applicable to GPIOA */
#define RCC_BASE        (PERIPH_BASE + 0x23800)
#define RCC_AHB1ENR     ((uint32_t*)(RCC_BASE + 0x30))

/* GPIOA peripheral addresses */
#define GPIOA_BASE      (PERIPH_BASE + 0x20000)
#define GPIOA_MODER     ((uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       ((uint32_t*)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR		((uint32_t*)(GPIOA_BASE + 0x18))

/* User functions */
void _start (void);
int main(void);
void delay(uint32_t count);

/* Minimal vector table */
uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
    (uint32_t *)SRAM_END,   // initial stack pointer
    (uint32_t *)_start        // main as Reset_Handler
};

// Begin address for the initialisation values of the .data section.
// defined in linker script
extern uint32_t _sidata;
// Begin address for the .data section; defined in linker script
extern uint32_t _sdata;
// End address for the .data section; defined in linker script
extern uint32_t _edata;
// Begin address for the .bss section; defined in linker script
extern uint32_t _sbss;
// End address for the .bss section; defined in linker script
extern uint32_t _ebss;

inline void
__attribute__((always_inline))
__initialize_data (uint32_t* from, uint32_t* region_begin, uint32_t* region_end)
{
  // Iterate and copy word by word.
  // It is assumed that the pointers are word aligned.
  uint32_t *p = region_begin;
  while (p < region_end)
    *p++ = *from++;
}

inline void
__attribute__((always_inline))
__initialize_bss (uint32_t* region_begin, uint32_t* region_end)
{
  // Iterate and copy word by word.
  // It is assumed that the pointers are word aligned.
  uint32_t *p = region_begin;
  while (p < region_end)
    *p++ = 0;
}

void __attribute__ ((noreturn,weak))
_start (void)
{
	__initialize_data(&_sidata, &_sdata, &_edata);
	__initialize_bss(&_sbss, &_ebss);
	main();

	for(;;);
}

/* Page 511: Pointer to a const array, which implies that a word will be
allocated inside the .data section to store the location in flash
memory of the string "Hello" */
// If commented out, .data section becomes zero bytes
const char *m = "Hello";

/* No .rodata section is created unless the pointer above is used ->
 * and only "Hello" moves into .rodata  */
/* "There is no portable way to put data into the .rodata section because even
the existence of such a section is implementation-dependent."*/
/* Changing the size of the string/array does not affect the size
 * of the .text section, so where does the string go? Apparently
 * it is not stored in ascii format in flash. */
const char msg[] = "Hello World!";
const float vals[] = {3.14, 0.43, 1.414};

int main() {
    /* enable clock on GPIOA peripheral */
    *RCC_AHB1ENR = 0x1;
    *GPIOA_MODER |= 0x400; // Sets MODER[11:10] = 0x1
    *GPIOA_BSRR = 0x00000020;

    while(vals[0] >= 3.14 && msg[0] == 'H' && *m == 'H') {
      *GPIOA_BSRR = 0x00200000;
      delay(200000);
      *GPIOA_BSRR = 0x00000020;
      delay(300000);
   	}
}

void delay(uint32_t count) {
    while(count--);
}

