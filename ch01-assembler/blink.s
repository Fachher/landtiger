/* Select the target processor. */
.cpu cortex-m3

/* Generate Thumb instructions. This performs the same action as .code 16 */
.thumb

/* nvic -- vector interrupt table */
.section ".isr_vector"
.word   0x10008000  /* stack top address */
.word   _start      /* 1  Reset */
.word   hang        /* 2  NMI */
.word   hang        /* 3  HardFault */
.word   hang        /* 4  MemManage */
.word   hang        /* 5  BusFault */
.word   hang        /* 6  UsageFault */
.word   hang        /* 7  RESERVED */
.word   hang        /* 8  RESERVED */
.word   hang        /* 9  RESERVED*/
.word   hang        /* 10 RESERVED */
.word   hang        /* 11 SVCall */
.word   hang        /* 12 Debug Monitor */
.word   hang        /* 13 RESERVED */
.word   hang        /* 14 PendSV */
.word   hang        /* 15 SysTick */

/* This directive indicates to assemble the following code into a the section
name. See the flash.ld file for section layout. */
.section ".text"

/* The .thumb_func directive specifies that the following symbol is the name
of a Thumb encoded function. */
.thumb_func
hang:
   b    .                  // ~ while( true )

/* Perform a busy waiting. */
.thumb_func
dowait:
   ldr  r7, =0xA0000       // store 0xA0000 in the r7 register
dowaitloop:
   sub  r7, #1             // substract 1 from the value in r7
   bne  dowaitloop         // if r7 != 0, goto dowaitloop
   bx   lr                 // return

/* .globl makes the symbol visible to ld */
.thumb_func
.globl _start
_start:
   ldr  r0, =0x2009C040    // load memory address  FIO1DIR2
   ldrb r1, [r0]           // load in r1 the value store in the memory address
                           // [r0], with immediate offset (unsigned byte).
   mov  r2, #0xFF          // store ‭the value 10110100‬ in r2, this value is used
                           // to change the direction mode of the GPIOs pins
                           // into which the mbed LPC1768 LEDs are connected to
                           // output.
   orr  r1, r2             // logical OR between r1 and r2 registers.
   strb r1, [r0]           // store register r1 value into memory adress [r0].

   ldr  r0, =0x2009C058    // set gpio (FIO1SET2, see page 135 in LPC17xx manual)
   ldr  r1, =0x2009C05C    // clear gpio (FIO1CLR2, see page 136 in LPC17xx manual)
   ldr  r2, =0x1          // store the value 0100 in r2

mainloop:
   cmp r2, #0x80
   bcc noreset
   beq noreset
   mov r2, #0x1
noreset:
   strb r2, [r0]           // store the value in r2 in the memory address [r0]
   bl dowait               // execute dowait
   strb r2, [r1]           // store the value in r2 in the memory address [r1]
   lsl r2, r2, #0x1
   b    mainloop           // goto mainloop

.end
