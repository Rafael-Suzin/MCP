# Mapeamento dos registradores:
# a: $t0, b: $t1, c: $t2, d: $t3, res: $t4
#########################################################

######################################
# res = a + b + c

add  $t4 $t1 $t2	# res=b+c
add  $t4 $t4 $t0	# res+=a
######################################
# res = a - b - c

add  $t4 $t1 $t2	# res=b+c
sub  $t4 $t0 $t4	# res=a-res

######################################
# res = a * b - c

mul  $t4 $t0 $t1	# res=a*b
sub  $t4 $t0 $t4	# res-=c

######################################
# res = a * (b + c)

add  $t4 $t1 $t2	# res=b+c
mul  $t4 $t0 $t4	# res=a*b

######################################
# res = a + (b - 5)

addi $t2 $t1 -5		# t2=b-5
add  $t4 $t0 $t2	# res=a+t2

######################################
# res = ((b % 2) == 0)

andi $t4 $t1 1	#pega apenas o valor do bit 1
seq  $t4 $t4 0

######################################
# res = (a < b) && (((a+b) % 3) == 2)  #senpre vai dar 0, verdadeiro não é igual a 2


addi  $t0 $0 1	 #a=10
addi  $t1 $0 3	 #b=3

add  $t3 $t0 $t1 #t3=a+b
addi $t4 $0 3	 #t4=3

#t3=t3%3
div  $t3 $t2	
mfhi $t3 	

slt $t4 $t0 $t1	 #t2=a<b
and $t4 $t4 $t3  #res=t4&&t1
seq  $t5 $t4 2

######################################
# res = (a >= b) && (c != d)

sge $t4 $t0 $t1  # a>=b
sne $t5 $t2 $t3	 # c!=d
and $t4 $t4 $t5  # c=t4&&t5

######################################
# res = (((a/2)+1) > b) || (d == (b-a))

sub $t $t1 $t0
seq $t $t3 $t



