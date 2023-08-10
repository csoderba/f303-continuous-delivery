/* memory layout for an STM32F446RE */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
}
 
 ENTRY(_start)
 
/* output sections */
SECTIONS
{
    /* Program code into FLASH */
    .text : ALIGN(4)
    {
        *(.isr_vector)    /* Vector table */
        *(.text .text*)          /* Program code */
        KEEP(*(.isr_vector))
        
        /*Page 517: .init and .fini: these sections will contain 
        initialization and de-initialization code added by the C 
        run-time library. Those sections ensure proper initialization 
        of the C run-time library and they are needed unless the 
        options -nodefaultlibs and -nostdlib are set in project settings
		and so no C standard library functions are used. */
		/* However -nodefaultlibs and -nostdlib are not set in this
		project but it still works without .init and .fini ?*/
        /*KEEP (*(.init))*/
    	/*KEEP (*(.fini))*/
    } >FLASH
 
    /* Initialized global and static variables (which 
       we don't have any in this example) into SRAM */

    /* Used by the startup to initialize data */
    _sidata = LOADADDR(.data);     
       
    .data : ALIGN(4)
    {
        _sdata = .;        /* create a global symbol at data start */
        
        *(.data)
        *(.data*)
        
        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end */        
    } >SRAM AT>FLASH
    
     /* Uninitialized data section */
		.bss (NOLOAD) : ALIGN(4)
		{
		  _sbss = .;         /* define a global symbol at bss start */
	  
		  *(.bss)
		  *(.bss*)
		  		
		  . = ALIGN(4);
		  _ebss = .;         /* define a global symbol at bss end */
		} >SRAM
		
	  /* Constant data goes into FLASH */
	  .rodata : ALIGN(4)
	  {
	    *(.rodata)         /* .rodata sections (constants) */
	    *(.rodata*)        /* .rodata* sections (strings, etc.) */
	  } >FLASH
}    