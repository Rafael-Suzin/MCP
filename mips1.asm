#s1=6 s2=2, s3=3 s4=5
#s0=s1*s2+s3/s4
#mul $s0 $s1 $s2	#s0=s1*s2
#div $t1 $s3 $s4	#t1=s3/s4
#add $s0 $s0 $t1	#s0=s0+t1

#s0=d s1=a=61, s2=b=6 s3=c=6
#d=a-3*(b+c+8)

li $s3 6		#s3=6
li $s2 6		#s2=6
li $s1 61		#s1=60


addi $s0 $s3 8		#s0=s3+8 #c=+8
add  $s0 $s0 $s2	#s0=+$s2	
#mul $s0 $s0 -3		#s0=*-3
addi $at $0 -3
#mul  $s0 $s0 $at	#s0=*-3
mult $s0 $at	#s0=*-3
mflo $s0 

#sub $s0 $s1 $s0	#s0=s1-$s0
add  $s0 $s1 $s0	#s0=s1+$s0

