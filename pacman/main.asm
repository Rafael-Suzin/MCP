.include "system.inc"

mov_vector(input_move)
animated_sprite(pacman, 3, 119, 140, 0, 0)

animated_sprite(ghost1, 2, 105, 105, 0, 0)
animated_sprite(ghost2, 2, 133, 105, 0, 0)
animated_sprite(ghost3, 2, 112, 112, 0, 0)
animated_sprite(ghost4, 2, 126, 112, 0, 0)
animated_sprite(ghost5, 2, 105, 119, 0, 0)
animated_sprite(ghost6, 2, 133, 119, 0, 0)

.text 0x00401000
main:
	addiu $sp, $sp, -40
	sw    $ra, 32($sp)
	jal draw_grid
	#li $s1 0x65
	#li $t0 7
	
main_L0:	
	## Desenha o sprite animado	
	j drawAnimated
	AnimatedEnd:
	## Verifica se posso mover ele
   	## Se não puder, para o sprite
	##la  $a0, pacman
	##jal check_movement
	
	## Aplica o vetor de movimento no sprite
	j ApplyMovement
	ApplyMovementEnd:
	## Busca no teclado se há comando do user
	la  $a0, input_move
	jal process_input
	
	## Tenta mudar o movimento se for possível
	la  $a0, pacman
	la  $a1, input_move
	jal try_change_movement
	
main_sleep:
	### Dorme um pouquinho (usabilidade)
	syscall_sleep(20) 
	
	j main_L0
	
	lw 	  $ra, 12($sp)
	addiu $sp, $sp, 16
	jr $ra
drawAnimated:
	la $s0, pacman
	li $s1, 7
	drawAnimatedLoop:
	beqz $s1 AnimatedEnd
	add $a0 $s0 $0
	jal draw_animated_sprite
	add $s0 $s0 20
	add $s1 $s1 -1
	j drawAnimatedLoop
	
ApplyMovement:
	la $s0, pacman
	li $s1, 7
	ApplyMovementLoop:
	beqz $s1 ApplyMovementEnd
	add $a0 $s0 $0
	jal apply_movement
	add $s0 $s0 20
	add $s1 $s1 -1
	j ApplyMovementLoop
