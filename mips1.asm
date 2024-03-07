# res = ((b % 2) == 0)

addi  $t0 $0 1	 #a=2
addi  $t1 $0 2	 #b=2

addi  $t2 $0 5	 #c=2
addi  $t3 $0 3	 #d=2

# res = (a >= b) && (c != d)

seq $t4 $t0 $t1  # a>=b
sne $t5 $t2 $t3	 # c!=d
and $t4 $t4 $t5  # c=t4&&t5
