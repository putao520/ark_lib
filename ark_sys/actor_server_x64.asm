.code
CallActorServiceGen PROC
mov rax, rcx
mov rdi, rdx
sub rsp,20h

cmp rdi, 1
jne toCall
mov rcx, rax
add rax,0fh

cmp rdi, 2
jne toCall
mov rdx, rax
add rax,0fh

cmp rdi, 3
jne toCall
mov r8, rax
add rax,0fh

cmp rdi, 4
jne toCall
mov r9, rax
add rax,0fh

cmp rdi, 5
jne toCall
push rax
add rax,0fh

cmp rdi, 6
jne toCall
push rax
add rax,0fh

cmp rdi, 7
jne toCall
push rax
add rax,0fh

cmp rdi, 8
jne toCall
push rax
add rax,0fh

cmp rdi, 9
jne toCall
push rax
add rax,0fh

cmp rdi, 10
jne toCall
push rax
add rax,0fh

cmp rdi, 11
jne toCall
push rax
add rax,0fh

cmp rdi, 12
jne toCall
push rax
add rax,0fh

cmp rdi, 13
jne toCall
push rax
add rax,0fh

cmp rdi, 14
jne toCall
push rax
add rax,0fh

cmp rdi, 15
jne toCall
push rax
add rax,0fh

cmp rdi, 16
jne toCall
push rax
add rax,0fh

cmp rdi, 17
jne toCall
push rax
add rax,0fh

cmp rdi, 18
jne toCall
push rax
add rax,0fh

cmp rdi, 19
jne toCall
push rax
add rax,0fh

cmp rdi, 20
jne toCall
push rax
add rax,0fh

toCall:
call r8
add rsp, 20h
ret
CallActorServiceGen ENDP
END