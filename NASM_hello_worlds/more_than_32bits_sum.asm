; Read 2 HEX numbers as command line arguments and store its parts under different labels in case each is longer than 32 bits (8 hex digits)
; Calculate their sum and print it out

; NASM for Linux i386 (32-bit)

global _start

; -------------------------------------------------------------------------------------------------------------------------------------------

section .data

	nl db 10, 0
	plus db ' ', '+', ' '
	;plus db 0x20, 0x2b, 0x20
	equality db " = "
	;equality db 0x20, 0x3d, 0x20
	err_arg_qty db "Please pass two command-line arguments", 10, 0
	err_arg_qty_txt_size equ $-err_arg_qty

	err_arg_fmt db "Please pass only hexadecimal numbers as command-line arguments", 10, 0
	err_arg_fmt_txt_size equ $-err_arg_fmt

	highest_in_lower_part equ 0xf0000000 ; mask to separate only the first digit of 32-bit lower part

; -------------------------------------------------------------------------------------------------------------------------------------------

section .bss

	higher_part resd 1 ; one double-word (4 bytes) is enough to store higher bits of 64-bits input value
	lower_part resd 1; the same description as above

	; 1st number:
	higher_part_val1 resd 1 ; higher part of first command-line argument
	lower_part_val1 resd 1; lower part of first command-line argument

	; 2nd number:
	higher_part_val2 resd 1 ; higher part of second command-line argument
	lower_part_val2 resd 1; lower part of second command-line argument

	; their sum
	higher_part_sum resd 1
	lower_part_sum resd 1

; -------------------------------------------------------------------------------------------------------------------------------------------

section .text
_start:
main:

	; need to check that we indeed run the program correctly, with expected number of command-line arguments

	mov eax, [esp] ; on top of the stack should be number of arguments

	; check if we indeed passed only two arguments:
	cmp eax, 3
	jnz quit_arg_qty


	; take first argument
	push dword [esp + 8] ; put first command line argument into stack
	call convert_string_to_hex_num
	add esp, 4 ; remove argument from stack once function execution is over

	mov eax, [higher_part]
	mov [higher_part_val1], eax
	mov eax, [lower_part]
	mov [lower_part_val1], eax

	; take second argument
	push dword [esp + 12] ; put second command line argument into stack
	call convert_string_to_hex_num
	add esp, 4 ; remove argument from stack once function execution is over

	mov eax, [higher_part]
	mov [higher_part_val2], eax
	mov eax, [lower_part]
	mov [lower_part_val2], eax

	; calculte sum of these values:
	xor eax, eax
	xor ebx, ebx
	mov eax, [lower_part_val1]
	mov ebx, [higher_part_val1]
	add eax, [lower_part_val2]
	adc ebx, [higher_part_val2]
	mov [lower_part_sum], eax
	mov [higher_part_sum], ebx

	; Print first hex number as ASCII sequence
	push dword [higher_part_val1]
	push dword [lower_part_val1]
	call print_ASCII_from_HEX
	add esp, 8 ; removed two arguments

	; print PLUS sign
	mov eax, 4
	mov ebx, 1
	mov ecx, plus
	mov edx, 3
	int 80h

	; Print second hex number as ASCII sequence
	push dword [higher_part_val2]
	push dword [lower_part_val2]
	call print_ASCII_from_HEX
	add esp, 8 ; removed two arguments

	; print EQUALITY sign
	mov eax, 4
	mov ebx, 1
	mov ecx, equality
	mov edx, 3
	int 80h

	; Print two numbers sum as ASCII sequence
	push dword [higher_part_sum]
	push dword [lower_part_sum]
	call print_ASCII_from_HEX
	add esp, 8 ; removed two arguments

	; print NEW-LINE sign
	mov eax, 4
	mov ebx, 1
	mov ecx, nl
	mov edx, 2
	int 80h

	; exit program:
	jmp quit_no_err

; -------------------------------------------------------------------------------------------------------------------------------------------
convert_string_to_hex_num:

	push ebp
	mov ebp, esp

	mov eax, [esp + 8]
	xor ecx, ecx ; ECX register will be a counter of bytes of command-line argument
	; number of bytes will help us to identify, how many hex digits are there in the number. Once there are more than 8 digits (one byte can store two hex digits)
	; then we will need to use another memory-cell to store the rest, shifting digit-by-digit from previous one (from lower to higher part if digits keep coming.

	xor ebx, ebx
	mov dword [lower_part], 0
	mov dword [higher_part], 0

.convert_loop:

	cmp byte [eax + ecx], 0
	jz .end_of_loop

	mov dl, byte [eax + ecx]
	; check if it is a digit in range from zero to nine:
	cmp dl, '0'
	jl .letter_digit_check
	cmp dl, '9'
	jg .letter_digit_check
	sub dl, '0' ; if we are here, then we have a character in current byte, which symbolizes a decimal digit - convert it into an actual value
	jmp .digit_identified

	.letter_digit_check:
		cmp dl, 'a'
		jl quit_arg_fmt
		cmp dl, 'f'
		jg quit_arg_fmt
		sub dl, 'a'
		add dl, 10 

	.digit_identified:
	inc ecx ; number has one digit more

	cmp ecx, 8 ; if we just reached the border of 8 digits, 32-bits cell or register is not enough anymore to store the number!
	jle .enough_digits

	; need to shift higher_part 4 bits left to move to freed space the left-most digit from the lower_part
	shl dword [higher_part], 4
	mov ebx, [lower_part]
	and ebx, highest_in_lower_part ; EBX now will contain the first digit of the lower_part
	shr ebx, 28
	add [higher_part], bl 

	; the stuff below should be done anyway, regardless of current value of digits counter
	.enough_digits:
	shl dword [lower_part], 4 ; shift to the left on one hexadecimal digit
	add [lower_part], dl ; added the last digit to the resulting number

	xor ebx, ebx ; make it zero again
	jmp .convert_loop

.end_of_loop:

	mov esp, ebp
	pop ebp
	ret

; -------------------------------------------------------------------------------------------------------------------------------------------
print_ASCII_from_HEX: 

	push ebp
	mov ebp, esp

	mov eax, [ebp + 8] ; start with the second argument - the lower part
	mov ecx, eax
	; put each new digit in stack, starting from the end of the number - thus we will print from beginning with POP
	xor edx, edx ; will serve as counter - we will need it doing POP while printing each byte of the number 

	.HEX_to_ASCII_loop:
		and eax, 0xf ; only last HEX digit is saved: 0xf = 1111
		shr ecx, 4 ; 4 bits to the right - current last digit removed
		inc edx ; increase counter of number of digits

		cmp al, 0xa
		jl .decimal_digit_conv
		add al, 'a'
		sub al, 10
		push eax
		jmp .proceed_loop

		.decimal_digit_conv:
		add al, '0'
		push eax

		.proceed_loop:
		mov eax, ecx ; put shifted value in EAX
		test eax, eax ; compare EAX with zero - check if we put all digits of a number (its lower or higher part) into stack already
	jnz .HEX_to_ASCII_loop

	cmp edx, 8 ; check if it is 8 digits or less considered
	jg .higher_part_minded_already ; if more - no need to check higher part second time
	mov eax, [ebp + 12] ; now lets try the same with the higher part (first check if there is one)
	mov ecx, eax
	test eax, eax
	jnz .HEX_to_ASCII_loop ; higher part does exist

	.higher_part_minded_already:
	section .bss

		curr_char resb 1 ; reserve one-byte cell to store current character popped from stack

	section .text

	.print_loop:
		pop eax
		mov [curr_char], eax

		push edx ; write system call will overwrite our counter EDX!! need to save it as it indicates how many times we should repeat the loop!

		mov eax, 4
		mov ebx, 1
		mov ecx, curr_char
		mov edx, 1 ; size is one byte - just fine for one ASCII character
		int 80h

		pop edx

		dec edx
		test edx, edx
	jnz .print_loop

	mov esp, ebp ; restore return address of external function (the one from which we called the current)
	pop ebp ; restore base pointer of external function's frame
	ret

; -------------------------------------------------------------------------------------------------------------------------------------------

quit_arg_qty:

	call print_arg_qty

	mov eax, 1
	mov ebx, -1
	int 80h

; -------------------------------------------------------------------------------------------------------------------------------------------

quit_arg_fmt:
	
	call print_arg_fmt

	mov eax, 1
	mov ebx, -2
	int 80h

; -------------------------------------------------------------------------------------------------------------------------------------------

quit_no_err:
	
	mov eax, 1
	mov ebx, 0
	int 80h

; -------------------------------------------------------------------------------------------------------------------------------------------

print_arg_fmt:

	push ebp
	mov ebp, esp

	push dword err_arg_fmt
	push dword err_arg_fmt_txt_size
	call print_err

	add esp, 8 ; remove two arguments from stack top
	mov esp, ebp
	pop ebp
	ret

; -------------------------------------------------------------------------------------------------------------------------------------------

print_arg_qty:

	push ebp
	mov ebp, esp

	push dword err_arg_qty
	push dword err_arg_qty_txt_size
	call print_err

	add esp, 8 ; remove two arguments from stack top
	mov esp, ebp
	pop ebp
	ret

; -------------------------------------------------------------------------------------------------------------------------------------------

print_err:

	push ebp
	mov ebp, esp

	mov eax, 4
	mov ebx, 1
	mov ecx, [ebp + 12] ; address of err string is deeper in stack
	mov edx, [ebp + 8] ; than its size (pls, look into print_arg_qty and print_arg_fmt functions)
	int 80h

	mov esp, ebp
	pop ebp
	ret

; -------------------------------------------------------------------------------------------------------------------------------------------