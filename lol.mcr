SECTION TEXT
INPUT A
INPUT B
INPUT C
LOAD A
SUB B
JMPZ CHECK_B
LOAD A
SUB C
JMPZ CHECK_B
JMP A_WINS
CHECK_B: LOAD ZERO
LOAD B
SUB A
JMPZ CHECK_C
LOAD B
SUB C
JMPZ CHECK_C
JMP B_WINS
CHECK_C: LOAD ZERO
LOAD C
SUB A
JMPZ TIE
LOAD C
SUB B
JMPZ TIE
JMP C_WINS
TIE: LOAD ZERO
OUTPUT ZERO
STOP
A_WINS: LOAD ZERO
OUTPUT ONE
STOP
B_WINS: LOAD ZERO
OUTPUT TWO
STOP
C_WINS: LOAD ZERO
OUTPUT THREE
STOP
SECTION DATA
A: SPACE
B: SPACE
C: SPACE
ZERO: CONST 0
ONE: CONST 1
TWO: CONST 2
THREE: CONST 3
