TRIANGULO EQU 1

SECTION TEXT
		input		B
		input		H
		load		B
		mult		H
		if TRIANGULO
		div		DOIS
		store		R
		output	R
		stop

SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
