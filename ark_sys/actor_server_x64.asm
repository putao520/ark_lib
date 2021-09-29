.code
CallActorServiceGen PROC
mov rax, rcx
mov rdi, rdx
sub rsp,20h
push r8
push r10
push r11
push rdi
test rdi, rdi
je toCall

mov rcx, qword ptr[rax]
add rax,18h
cmp rdi, 1
je toCall

mov rdx, qword ptr[rax]
add rax,18h
cmp rdi, 2
je toCall

mov r8, qword ptr[rax]
add rax,18h
cmp rdi, 3
je toCall

mov r9, qword ptr[rax]
add rax,18h
cmp rdi, 4
je toCall

mov r10, 5

ToStack:
cmp r10, rdi
jb toCall
mov r11, qword ptr[rax]
push r11
add rax,18h
inc r10
jmp ToStack

toCall:
pop rdi
pop r11
pop r10
pop rax
call rax
add rsp, 20h
ret
CallActorServiceGen ENDP

GetProcessPeb PROC
mov rax,gs:[60h]
ret
GetProcessPeb ENDP
END