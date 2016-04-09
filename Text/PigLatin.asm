# This program is written in mips assembly
# The program only accepts ascii characters and uses memory mapped
# IO to update the text as you type

.data
app:  .asciiz " way"
buff: .space 200


.text
main:
    li $s0, 0
    sb $s0, buff($s0)
    la $s2, buff
    addi $s0 $s0, -1                    # set register $s0 to contain -1
    j repl                              # start the repl loop


isvowel:                                # checks if a given string is a vowel
    li $v0, 1
    beq $a0, 'a', ret
    beq $a0, 'e', ret
    beq $a0, 'i', ret
    beq $a0, 'o', ret
    beq $a0, 'u', ret
    li $v0, 0
    ret: jr $ra

putchar:
    li $t0, 0xFFFF0008
    put:      lw $t1, ($t0)             # wait until the device is ready to output
              beqz $t1, put             # if the device is not ready, repeat previous action
              sb $a0, 4($t0)            # save the byte into the output buffer at address 0xFFFF000C
              jr $ra                    # return the the function that called this
              
getchar:
    li $t0, 0xFFFF0000
    get:      lw $t1, ($t0)             # waits until the user has typed something
              beqz $t1, get             # if the value at that register is 0, means user has not typed
              lbu $v0, 4($t0)           # load the byte into register $v0 from address 0xFFFF0004
              jr $ra                    # return the byte read to the user
                        
repl:
    jal getchar                         # read a character from the input device
    move $a0, $v0                       # place the character in register $a0
    bne $v0, '\n', eval                 # if the character is not a newline, go to the evaluate stage
    la $a0, buff
    move $a1, $s2
    jal print_pig                       # otherwise print anything that might be in the buffer
    li $v0, 10
    syscall                             # exit with code 10 if the user typed newline

eval:
    beq $a0, ' ', loop                  # user typed a space which means the current string has ended
    ori $a0, $a0, 32                    # convert the character to lowercase
    beq $s0, -1, type                   # if this is the first character of a string
    beq $s0, 2, evalway                 # first character of current string is consonant
    jal putchar                         # otherwise just print the character
    b repl                              # get more characters from user
    
    type:               
        li $s1, 0                       # $s1 will be used to index into the buffer
        sb $s1, buff($s1)               # set the buffer to initially contain the null byte
        jal isvowel                     # $v0 = 1 means vowel, = 0 means not vowel
        xori $v0, $v0, 1
        addi $s0, $v0, 1                # $s0 = 1 means vowel, = 2 means not vowel
        la $s2, app($s0)                # string appended to the end depending on value of register $s0
        b eval
    
    evalway:
        jal isvowel                     # check if the character is vowel, store result in $v0
        seq $v1, $a0, 'y'               # check if character is 'y', store result in $v1
        add $v0, $v1, $v0               # add the results and store in $v0
        sub $s0, $s0, $v0               # subtract the result from $s0
        bne $s0, 2, eval                # if the character is a vowel or 'y', retry eval again
        
        sb $a0, buff($s1)               # otherwise store the character in a buffer
        addi $s1, $s1, 1
        li $a0, 0
        sb $a0, buff($s1)               # store the null byte in the next position of the buffer
        blt $s1, 199, repl              # placed here to prevent buffer overflow so that if the
                                        # buffer is somehow full, we just flush it and continue

loop:   
     la $a0, buff                       # move the buffer into register $a0 for printing
     move $a1, $s2                      # move the appended string to register $a1
     jal print_pig
     
     li $s0, 0
     sb $s0, buff($s0)
     la $s2, buff
     addi $s0 $s0, -1                   # set register $s0 to contain -1
     b repl

print_pig:
     move $s0, $a0                      # copy register $a0 to the save register $s0
     move $s1, $a1                      # copy register $a1 to the save register $s1
     lbu $a0, ($s0)                     # load the first byte of register $s0 into $a0
     lbu $a1, ($s1)                     # load the first byte of register $s1 into $a1
     addi $sp, $sp, -4                  # make space in the stack to store the the return address
     sw $ra, ($sp)                      # store the return address
     
     intpray: beqz $a0, igpay           # if the byte in register $a0 is null, print the next string
        jal putchar                     # print the current character
        addi $s0, $s0, 1                # increment the address of the string
        lbu $a0, ($s0)                  # load the next byte into register $a0
        b intpray                       
        
     igpay: beqz $a1, int_pigpray       # if the byte in register $a1 is null, exit the function
        move $a0, $a1                   # move the byte to register $a0 to be printed
        jal putchar
        addi $s1, $s1, 1
        lbu $a1, ($s1)
        b igpay
            
     int_pigpray: lbu $a0, app          # load the space character into register $a0
        jal putchar                     # print the space
        lw $ra, ($sp)                   # restore the return address
        addi $sp, $sp, 4
        jr $ra                          # return to whoever called us
