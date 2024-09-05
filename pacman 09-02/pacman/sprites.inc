.text 0x00430000
.eqv lineN 35
.eqv colN 35
.macro GetGridElement(%grid_table,%x_col, %y_row) 
    add $a0, $0, %x_col
    add $a1, $0, %y_row
    add $a2, $0, %grid_table
    jal get_grid_element
.end_macro
#  int colors[8];
#  sprite sprites[20];
#  char sprite[49];
#=============================================================
# void draw_sprite(sprite_id,X, Y){
#       char * ptr_sprite =  sprites[sprite_id];
#       for(int i = 0; i < 49; i++){ 
#			color = colors[*ptr_sprite];
#           sX = i % 7 ;
#			sY = i / 7 ;
#           sX = X + sX;
#           sY = Y + sY;
#        	set_pixel(sX, sY, color)
#           ptr_sprite++;
#       }
# }
#-----------
# prev_stack	-> 40($sp)
#-----------
#	<empty>		-> 36($sp)
#-----------
#	$ra			-> 32($sp)
#-----------
#	$s4			-> 28($sp)
#-----------
#	$s3			-> 24($sp)
#-----------
#	$s2			-> 20($sp)
#-----------
#	$s1			-> 16($sp)
#-----------
#	$s0			-> 12($sp)
#-----------
#	$a2			-> 8($sp)
#-----------
#	$a1			-> 4($sp)
#-----------
#	$a0			-> 0($sp)
.macro DrawSprite(%x, %y, %sprite_id)
    add $a0, $0, %x
    add $a1, $0, %y
    add $a2, $0, %sprite_id
    jal drawSprite
.end_macro
drawSprite:
	addiu $sp, $sp, -40
	sw    $ra, 32($sp)
	sw    $s4, 28($sp)		#i
	sw    $s3, 24($sp)		#&colors
	sw    $s2, 20($sp)		#y
	sw    $s1, 16($sp)		#x
	sw    $s0, 12($sp)		#&sprite[]


	la	$s0 sprites
	mul 	$t0  $a2 SPRITE_SIZE
	add	$s0 $s0 $t0
	add	$s1 $0 $a0		#x=a0
	add	$s2 $0 $a1		#y=a1
	
	la $s3 colors
	li $s4 0
	draw_sprite_L0:
	bge $s4 49 draw_sprite_L0_end

		lb $t0 0($s0)
		sll $t0 $t0 2
		add $t0 $t0 $s3
		lw $a2 0($t0)
		
		li $t1 7
		div	$s4 $t1
		mfhi $t1
		mflo $t2
	
		add $a0 $t1 $s1
		add $a1 $t2 $s2
		
		#setPixel($t1, $t2, $t0)
		jal setPixel
		add $s4 $s4 1
		add $s0 $s0 1
	j draw_sprite_L0
draw_sprite_L0_end:
	
	
	lw    $ra, 32($sp)
	lw    $s4, 28($sp)
	lw    $s3, 24($sp)
	lw    $s2, 20($sp)
	lw    $s1, 16($sp)
	lw    $s0, 12($sp)
	addiu $sp, $sp, 40
	jr   $ra
#=============================================================


# old_stack      16($sp)
#########################
#  $ra, 12($sp)
# -----------------------
#  $a2, 8($sp)
# -----------------------
#  $a1, 4($sp)
# -----------------------
#  $a0, 0($sp)
#=============================================================
# void draw_animated_sprite(animated_sprite*)
draw_animated_sprite:
	addiu $sp, $sp, -16
	sw    $ra, 12($sp)

	lw  $a2, 0($a0) 
	lw  $a1, 8($a0)
	lw  $a0, 4($a0)  #  void draw_sprite(X, Y,sprite_id,){
	jal drawSprite #  void draw_sprite(sprite_id,X, Y,){

	lw    $ra, 12($sp)
	addiu $sp, $sp, 16
	jr    $ra
#=============================================================

# struct animated_sprite {
# 	int ID;      ## 0
# 	int pos_X;   ## 4
#	int pos_Y;   ## 8
#	int mov_X;   ## 12
#	int mov_Y;   ## 16
# }
#=============================================================
# void apply_movement(animated_sprite*)
apply_movement:

	addiu	$sp, $sp, -40
	sw	$ra, 32($sp)
	sw	$s4, 28($sp)		#y_row
	sw	$s3, 24($sp)		#x_col
	sw	$s2, 20($sp)		#y
	sw	$s1, 16($sp)		#x
	sw	$s0, 12($sp)		#animated_sprite*

	add $s0, $a0, $0
	lw 	$s1, 4($s0)    ## ptr.pos_X
	lw 	$s2, 8($s0)    ## ptr.pos_Y
	lw  $s3, 12($s0)   ## ptr.mov_X
	lw  $s4, 16($s0)   ## ptr.mov_X
	add $s3, $s3, $s1  ## pos_X += mov_X
	add $s4, $s4, $s2  ## pos_Y += mov_Y
	
	li $t0 7		#converte para x e y para row e col do proximo elemento da grid
	div $s3 $t0
	mflo $t1
	div $s4 $t0
	mflo $t2
	la  $t0 grid
	GetGridElement($t0,$t1,$t2)	#pega o id do proximo elemento da grid
	bge $v0 5 apply_movement_parede #Se o id for maior que 4 então é parede
		sw  $s3, 4($s0)    ## ptr.pos_X <- pos_X += mov_X
		sw  $s4, 8($s0)    ## ptr.pos_Y <- pos_Y += mov_Y
	j apply_movement_end
	apply_movement_parede:
		add $a0, $s0, $0
		jal stop_animated_sprite
	apply_movement_end:
	
	lw	$ra, 32($sp)
	lw	$s4, 28($sp)
	lw	$s3, 24($sp)
	lw	$s2, 20($sp)
	lw	$s1, 16($sp)
	lw	$s0, 12($sp)
	addiu	$sp, $sp, 40
	jr	$ra
#=============================================================

#=============================================================
# void set_move_animated_sprite(animated_sprite* sp, int vx, int vy)
set_move_animated_sprite:
	sw   $a1, 12($a0)
	sw   $a2, 16($a0)
	jr   $ra
#=============================================================

#=============================================================   
# void stop_animated_sprite(animated_sprite* sp)
stop_animated_sprite:
	sw   $0, 12($a0)
	sw   $0, 16($a0)
	jr   $ra
#=============================================================

#=============================================================
# char get_grid_element(column_x, row_y, grid_table*)
get_grid_element:
	## Le o elemento na grid table e retorna.
	mul $a1 $a1 35
	add $a0 $a1 $a0
	add $a2 $a2 $a0
	lb $v0 0($a2)
	## Pegar o id_do_sprite (get_grid_element)
	## converter de ASCII para ID (-64)
	add $v0 $v0 -0x40
	jr  $ra
#=============================================================
.macro GetGridElement(%grid_table,%x_col, %y_row) 
    add $a0, $0, %x_col
    add $a1, $0, %y_row
    add $a2, $0, %grid_table
    jal get_grid_element
.end_macro
#=============================================================
# draw_grid(width, height, grid_table*)
draw_grid:

	addiu $sp, $sp, -40
	sw    $ra, 32($sp)
	#sw    $s4, 28($sp)		#
	sw    $s3, 24($sp)		#y
	sw    $s2, 20($sp)		#x
	#sw    $s1, 16($sp)		#
	sw    $s0, 12($sp)		#&sprite


	la $s0 grid	#&grid
	draw_grid_col:
	bge $s3 colN draw_grid_col_end
		li $s2 0 #i
		draw_grid_line:

		beq $s2 lineN draw_grid_line_end
		#Percorre a matriz da grid, para cada elemento

			#get_grid_element(column_x, row_y, grid_table*)
			add $a0 $s2 $0
			add $a1 $s3 $0
			add $a2 $s0 $0
			jal get_grid_element
			## Chamar draw_sprite(grid pos -> pixel pos - multiplica {X|Y}_SCALE)
			mul $a0 $s2 7
			mul $a1 $s3 7
			DrawSprite($a0,$a1,$v0)
			add $s2 $s2 1	#i++
			j draw_grid_line
		draw_grid_line_end:
		add $s3 $s3 1	#j++
		j draw_grid_col
	draw_grid_col_end:

	lw	$ra, 32($sp)
	#lw	$s4, 28($sp)
	lw	$s3, 24($sp)
	lw	$s2, 20($sp)
	#lw	$s1, 16($sp)
	lw	$s0, 12($sp)
	addiu	$sp, $sp, 40
	jr	$ra
#=============================================================


#########################
# old_stack ($a0)  8($sp)
#########################
#  < empty >       4($sp)
# -----------------------
#  $ra             0($sp)
#=============================================================
# process_input(mov_vector *)
process_input:
    ## Verificar se key_rb está vazio
    ## Se estiver, retornar
	
	## Senao
	## Le valor do ringbuffer
	
	## Processa valor com teclas associadas ao
	## movimento. Setar mov_vector e tornar
	## válido	
	addiu $sp, $sp, -8
	sw $ra, 0($sp)     # save return address ($ra)
	sw $a0, 8($sp)     # save process_input parameter ($a0)

	jal get_nonblocking_char
	beq $v0, -1, process_input_exit
	
	bne $v0, ' ', process_input_a
	li $t0, 0 # mov_x
	li $t1, 0 # mov_y
	j 	process_input_end
	
process_input_a:
	bne $v0, 'a', process_input_s
	li $t0, -1 # mov_x
	li $t1, 0 # mov_y
	j 	process_input_end
	
process_input_s:
	bne $v0, 's', process_input_w
	li $t0, 0 # mov_x
	li $t1, 1 # mov_y
	j 	process_input_end
	
process_input_w:
	bne $v0, 'w', process_input_d
	li $t0, 0 # mov_x
	li $t1, -1 # mov_y
	j 	process_input_end
		
process_input_d:
	bne $v0, 'd', process_input_exit
	li $t0, 1 # mov_x
	li $t1, 0 # mov_y	
	
process_input_end:
	lw $a0, 8($sp) # restauro valor do $a0
	sw $t0, 4($a0) # seta mov_x
	sw $t1, 8($a0) # seta mov_y
	li $t0, 1
	sw $t0, 0($a0) # seta valid
	
process_input_exit:
	lw $ra, 0($sp)
	addiu $sp, $sp, 8
	jr   $ra
#=============================================================
# struct animated_sprite {
# 	int ID;      ## 0
# 	int pos_X;   ## 4
#	int pos_Y;   ## 8
#	int mov_X;   ## 12
#	int mov_Y;   ## 16
# }
# struct mov_vector {
#    int valid;
#    int mov_x;
#    int mov_y;
# }
#########################
# old_stack ($a1)  20($sp)
# ------------------------
# old_stack ($a0)  16($sp)
#########################
#  $ra             12($sp)
# -----------------------
#  $a2             8($sp)
# -----------------------
#  $a1             4($sp)
# -----------------------
#  $a0             0($sp)
#=============================================================   
# void try_change_movement(animated_sprite* sp, mov_vector* mv)
try_change_movement:
	lw $t0, 0($a1) # valid 
	beqz $t0, try_change_movement_end
	
	addiu $sp, $sp, -28
	sw    $s4, 8($sp)
	sw    $s0, 12($sp)
	sw    $s1, 16($sp)
	sw    $s2, 20($sp)
	sw    $ra, 24($sp)
	
	add $s0, $a0, $0
	lw $s1, 4($a0)		#x
	lw $s2, 8($a0)		#y
	add $s4, $a1, $0
	#checa se esta alinhado ao grid
	li $t0 7
	div $s1 $t0
	mfhi $t1
	mflo $t3
	div $s2 $t0
	mfhi $t2
	mflo $t4
	bne $t1 $0 try_change_movement_end2
	bne $t2 $0 try_change_movement_end2
		sw $0, 0($s4) # zero valid
		la $t0 grid
		lw $t1 4($a1)
		lw $t2 8($a1)
		add $t1, $t1, $t3
		add $t2, $t2, $t4
		GetGridElement($t0,$t1,$t2)	#pega o id do proximo elemento da grid
		bge $v0 5 try_change_movement_end3 #Se o id for maior que 5 então é parede

		
			lw $t0, 4($s4)
			lw $t1, 8($s4)

			# call set_move_animated_sprite(animated_sprite* sp, int vx, int vy)
			add $a0, $s0, $0
			move $a1, $t0
			move $a2, $t1
			jal  set_move_animated_sprite
		j try_change_movement_end2
	try_change_movement_end3: #se for parede não faz nada?
	
	try_change_movement_end2:
	lw    $s0, 12($sp)
	lw    $s1, 16($sp)
	lw    $s2, 20($sp)
	lw    $ra, 24($sp)
	lw    $s4, 8($sp)
	addiu $sp, $sp, 28

try_change_movement_end:
	jr   $ra
	## Verifica se mov_vector é válido
	## Se não, retorna.

	## Converte posição pixel para posicao grid
	## Lembrete - se resto divisao == 0, está alinhado
	
	## Se não está alinhado, não muda o movimento.
	## Retorna função.
	
	## Se alinhado, verifica se pode se mover para nova
	## direção
	
	## Calculo nova posicao somando vetor movimento
	## com posição do grid e pego elemento do grid
	## para ver se não é parede.
	
	## Se não for parede, aplica vetor de movimento
	## no sprite, caso contrário não faz nada
	## Não esquecer de tornar o mov_vector inválido
	## (valid = 0)	
#=============================================================

#=============================================================
# bool check_movement(animated_sprite*)
check_movement:
	## Converte posição pixel para posicao grid
	## Lembrete - se resto divisao == 0, está alinhado
	
	## Se não está alinhado, pode se mover.
	## Retorna verdadeiro
	
	## Calculo nova posicao somando vetor movimento
	## com posição do grid e pego elemento do grid
	## para ver se não é parede.
	
	## Se não é parede retorna verdadeiro
	
	## Se for parede, para o sprite (stop_animated_sprite)
	## e retorna falso.
	jr   $ra
#=============================================================

