.code
CallActorServiceGen PROC
mov rax, rcx
mov rdi, rdx
sub rsp,0x20

cmp rdi, 1
jne toCall
mov rcx, rax
add rax,0xf

cmp rdi, 2
jne toCall
mov rdx, rax
add rax,0xf

cmp rdi, 3
jne toCall
mov r8, rax
add rax,0xf

cmp rdi, 4
jne toCall
mov r9, rax
add rax,0xf

cmp rdi, 5
jne toCall
push rax
add rax,0xf

cmp rdi, 6
jne toCall
push rax
add rax,0xf

cmp rdi, 7
jne toCall
push rax
add rax,0xf

cmp rdi, 8
jne toCall
push rax
add rax,0xf

cmp rdi, 9
jne toCall
push rax
add rax,0xf

cmp rdi, 10
jne toCall
push rax
add rax,0xf

cmp rdi, 11
jne toCall
push rax
add rax,0xf

cmp rdi, 12
jne toCall
push rax
add rax,0xf

cmp rdi, 13
jne toCall
push rax
add rax,0xf

cmp rdi, 14
jne toCall
push rax
add rax,0xf

cmp rdi, 15
jne toCall
push rax
add rax,0xf

cmp rdi, 16
jne toCall
push rax
add rax,0xf

cmp rdi, 17
jne toCall
push rax
add rax,0xf

cmp rdi, 18
jne toCall
push rax
add rax,0xf

cmp rdi, 19
jne toCall
push rax
add rax,0xf

cmp rdi, 20
jne toCall
push rax
add rax,0xf

toCall:
call r8
add rsp, 0x20
retn
CallActorServiceGeb ENDP
END