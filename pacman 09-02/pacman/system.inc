.include "macros.inc"
#.include "stdlib.inc"
.include "pacman.inc"
.include "sprites.inc"
.include "ringbuffer.inc"
.include "kernel.inc"

.eqv STACK_INIT 0x7FFFEFFC
.eqv ROW_SIZE 0x100
.eqv FRAMEBUFFER 0x10040000
.data
.align 2
key_rb: .space 28

.text 0x00400000 ## System Init Code
init:
	la $sp, STACK_INIT
	
	jal main
	syscall_exit
  		
.text 0x00420000  ## System lib
#=============================================================
# void enable_int
enable_int:

	jr 		$ra
#=============================================================

#=============================================================
# void disable_int
disable_int:

	jr 		$ra
#=============================================================

#=============================================================
# void enable_irq(int irq_num)
enable_irq:

	jr 		$ra
#=============================================================

#=============================================================
# void disable_irq(int irq_num)
disable_irq:

	jr 		$ra
#=============================================================

#=============================================================
# void enable_keyboard_int
enable_keyboard_int:

	jr 		$ra	
#=============================================================

#=============================================================
# void set_pixel(X, Y, color)
setPixel:
	la  $t0, FRAMEBUFFER
	mul $a1, $a1, ROW_SIZE  ## y * numero de colunas
	add $a0, $a0, $a1       ## + x
	sll $a0, $a0, 2         ## multiplico por 4
	add $a0, $a0, $t0		## endereco final do pixel
	sw  $a2, 0($a0)
	jr	$ra
#=============================================================
.macro setPixel(%x, %y, %color)
    add $a0, $0, %x
    add $a1, $0, %y
    add $a2, $0, %color
    jal setPixel
.end_macro
#=============================================================
# char getchar();
getchar:
	la $t0, 0xFFFF0000
getchar_L0:
	lw $t1, 0($t0)			# load control register in $t0
	andi $t1, $t1, 1		# isolo o bit de "ready"
	beqz $t1, getchar_L0
		lb $v0, 4($t0)           # load data register in $v0
	jr $ra
#=============================================================

#=============================================================
# int get_nonblocking_char();
get_nonblocking_char:
	li $v0, -1 # return -1 if no char available
	la $t0, 0xFFFF0000
	lw $t1, 0($t0)     	# load control register (0xffff0000)
	andi $t1, $t1, 1    # isolate the bit ready
	beqz $t1, get_nonblocking_char_end
	lb $v0, 4($t0)      # read data register (0xffff0004)
get_nonblocking_char_end:
    jr $ra
#=============================================================


#=============================================================
# void putchar(char value);
putchar:
	la $t0, 0xFFFF0008
putchar_L0:
	lw $t1, 0($t0)				# load control register in $t0
	andi $t1, $t1, 1
	beqz $t1, putchar_L0
		sb $a0, 4($t0)			# write data register with $a0 (func arg)
	jr $ra

#=============================================================
