#res = (a<b) &&((a+b)%3)

addi  $t0 $0 1	 #a=10
addi  $t1 $0 3	 #b=3

add  $t3 $t0 $t1 #t3=a+b
addi $t4 $0 3	 #t4=3

#t3=t3%3
div  $t3 $t2	
mfhi $t3 	

slt $t2 $t0 $t1	 #t2=a<b
and $t4 $t2 $t3  #c=t2&&t3
