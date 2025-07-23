;FRACTAL TUTORIAL
;
;IF YOU WANT TO USE THIS CODE OR MODIFY IT IN YOUR PROGRAM
;CREDIT ME EXPLICITLY!!! 
;
;~MADE BY ARTIC A.K.A JHOPRO

[BITS    16]
[ORG 0x7C00]

WSCREEN equ 320
HSCREEN equ 200

call setup
call pattern

setup:
        ;Setup Mode 13h (VGA 256 COLORS)
        mov ah, 0x00
        mov al, 0x13
        int 0x10

        ;Setup Video Segment in ES
        push 0xA000
        pop es

        ;Setup Registers
        mov ah, 0x0C
        xor al, al ;AL = COLOR
        xor bx, bx ;BX = PAGE
        xor cx, cx ;CX = X POSITION
        xor dx, dx ;DX = Y POSITION
		
	ret	
;-------------------------------------------

reset:
        ;Start all over again
        xor cx, cx
        xor dx, dx

        ;ITERATION++
        inc word [iteration]

        pattern:
            jmp calcpixel

            setpixel:
                cmp cx, WSCREEN
                jae nextline

                cmp dx, HSCREEN
                jae reset

		;Plot the pixel
                int 0x10

		;Go to next pixel
                inc cx

		;Calculate the actual pixel
                jmp pattern
                ret
		
calcpixel:
        ;X XOR Y
        mov bx, cx
        AND bx, dx

        ;AL = X ^ Y
        mov al, bl
        AND al, [iteration]

        ;Zoom it 4x
        shr al, 2

        jmp color
		
;-------------------------------------------

color:
        ;IF AL > 55 --> SUBTRACT 16
        cmp al, 55
        ja subcolor

        ;IF AL < 32 --> ADD 32
        cmp al, 32
        jb addcolor

	;ELSE --> SET PIXEL
        jmp setpixel
		
subcolor:
        sub al, 16
        jmp color
		
addcolor:
        add al, 32
        jmp color

;-------------------------------------------
		
nextline:
        xor cx, cx
        inc dx

        jmp pattern
		
;-------------------------------------------

;INT ITERATION = 0
iteration: dw 0
		
;MBR Signature
times 510 - ($ - $$) db 0
dw 0xAA55
