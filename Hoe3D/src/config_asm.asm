	;; asm autotest functions
global _testMMX
_testMMX:	
	pushfd		; save EFLAGS  
	pop	eax	; store EFLAGS in EAX  
	mov	ebx, eax ; save in EBX for later testing 
	xor	eax, 00200000h ; toggle bit 21 
	push	eax	; put to stack 
	popfd		; save changed EAX to EFLAGS 
	pushfd		; push EFLAGS to TOS 
	pop	eax	; store EFLAGS in EAX 
	cmp	eax, ebx  ; see if bit 21 has changed 
	jz	.fail ; if no change, no CPUID  
	mov	eax, 1  ; request for feature flags 
	cpuid  ; 0Fh, 0A2h   CPUID Instruction  
	test	edx, 00800000h ; is MMX technology Bit(bit 23)in feature  
		; flags equal to 1 
	jz	.fail
	mov	eax,1
	ret 
.fail:
	xor	eax,eax
	ret
			