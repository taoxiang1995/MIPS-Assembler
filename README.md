Description: 
This is a simpler version of MIPS assembler which translates a subset of MIPS instructions to machine code. The assembler is a two-pass assembler. The first pass takes off comments, and records the labels' address in the symbol table. In the second pass, MIPS instructions are translated to machine code. The last step is writing a linker in MIPS to combine code and relocate symbols. 
This assembler supports the translation of instructions including:addu, or, slt, sltu, jr, sll, addiu, ori, lui, lb, lbu, lw, sb, sw, beq, bne, j, jal, li, move, blt, bgt. 


