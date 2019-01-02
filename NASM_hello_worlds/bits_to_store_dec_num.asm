; Program evaluates, how many bits are required to store a given decimal number

; NASM for Linux i386 (32-bit)

global _start

section .data

	nl db 10, 0 ; first byte to contain the new-line character, the second - end-of-string character

section .bss

	result resd 1 ; reserve one double word to store number of bits necessary for initial value storage
				  ; why double word (4 bytes)? actually, no idea, we are not using stack for this.
	value resb 10 ; how many byte-cells to leave for value storage?? how can we tell?
	buf_len equ $ - value

	res_str resb 1 ; label for easier access to first byte of resulting string

section .text

_start:

	;mov eax, "4097"; want to have it from keyboard - need to comment these two lines out and uncomment following with read system call
	;mov [value], eax

	 mov eax, 3 ; 3 stands for read system call number
	 mov ebx, 0 ; 0 stands for standard input stream
	 mov ecx, value ; address of a memory area where we are going to put what is read from stdin
	 mov edx, buf_len ; size 
	 int 80h ; program interrupt - 80h for system call

	; it is passed from keyboard as ASCII code, not a decimal number
	; need to convert it to decimal first

	push dword value
	call str_to_num
	add esp, 4 ; remove argument of str_to_num from top of the stack once function execution is over

	mov eax, ecx ; as result of str_to_num function is stored in ECX

	xor ecx, ecx ; going to use it as a quantity of HEX digits of given number

.loop:
	cmp eax, 0
	jz .end_of_loop
	inc ecx
	shr eax, 4 ; four bits right to shift on one hexadecimal digit right
	jmp .loop

.end_of_loop:

	mov eax, ecx
	mov edx, 4 ; MUL cannot have explicit value as an opernand, thus we need to put a value into register
	mul edx ; thus we will count number of bits which are necessary to store initial value

	mov [result], eax

	; now need to turn it (number of bits) from the decimal number into ASCII string
	xor ebx, ebx ; will use EBX as counter - and shifting of [res_str]
.loop_div:
	mov ecx, 10 ; will use ECX register as divisor, as, once again, DIV command doesn't take explicit value in its operand
	div ecx
	; EDX will contain the remainder, EAX will contain quotient
	add edx, '0'
	;mov [res_str + ebx], edx
	push edx ; put it in the stack instead - we need to print in a reverse order
	inc ebx
	xor edx, edx ; WHY ARE WE DOING THAT?! DIV thinks that divident is very big as it uses EDX to store higher bits of it!! 
	cmp eax, 0
	jnz .loop_div

.end_of_loop_div:
	;mov eax, ebx ; descending counter for loop_pop_stack, EBX needs to be saved
	xor ecx, ecx ; ascending counter, is needed to shift address from [res_str]
.loop_pop_stack:
	pop edx
	mov [res_str + ecx], edx
	inc ecx
	;dec eax
	;cmp eax, 0
	cmp ecx, ebx ; repeat until ECX (counter) is not the same as EBX value (calculated quantity of digits in previous loop_div)
	jnz .loop_pop_stack

	inc ebx
	mov byte [res_str + ebx], 0
	mov edx, ebx ; look above into the loop - for each digit of a number we used a separate byte cell
	mov ecx, res_str ; passed address of res_str (it is a label, marking memory area, not the value inside)

	; print on the screen
	mov eax, 4
	mov ebx, 1
	int 80h

	; print new-line character
	mov eax, 4
	mov ebx, 1
	mov ecx, nl
	mov edx, 2
	int 80h

	jmp quit_no_err

quit_no_err:

	mov eax, 1
	mov ebx, 0
	int 80h

str_to_num:

	push ebp ; save previous value of EBP, prior to function call
	mov ebp, esp ; save return address of this function to EBP

	mov eax, [esp + 8] ; ESP - previous EBP, ESP + 4 - return address, ESP + 8 - argument of a function
	; now EAX contains address of an input number (in ASCII)

	xor ecx, ecx ; let it be the resulting integer decimal number (converted from original ASCII string)
	xor edx, edx ; let it be a counter - to shift from original address of first byte until we stalk zero-byte

.loop:
	mov bl, byte [eax + edx]
	
	; no other characters but decimal digits in ASCII - that was crucial - apparently, some other characters were read from stdin, like new-line
	cmp bl, '0'
	jl .end_of_loop
	cmp bl, '9'
	jg .end_of_loop

	; cmp bl, 0
	; jz .end_of_loop

	sub bl, '0' ; we suppose that each character is actually an ASCII character of respective decimal digit

	; Multiply to ten - shift the number to the left - and add another digit (until the string is over)
	push eax
	push ebx
	push edx
	mov eax, ecx
	xor edx, edx
	mov ebx, 10
	mul ebx ; depending on operand size the result might be located in different registers
	mov ecx, eax
	pop edx
	pop ebx
	pop eax

	add ecx, ebx;
	inc edx
	jmp .loop

.end_of_loop:

	; restore ESP and EBP values as they were prior to function call
	mov esp, ebp
	pop ebp

	ret
