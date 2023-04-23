section .rodata
    f1_k: dq 0.6
    f1_b: dq 3.0
    f2_c: dq 2.0
    f2d_k: dq 3.0
    f3_k: dq 3.0

section .text
; x: double (ebp+8)
; -> double

; 0.6x + 3
global f1
f1:
    enter 8,0
    fld qword [ebp+8]
    fmul qword [f1_k]
    fadd qword [f1_b]
    leave
    ret

; 0.6
global f1d
f1d:
    enter 8,0
    fld qword [f1_k]
    leave
    ret

; (x-2)^3 - 1
global f2
f2:
    enter 8,0
    fld qword [ebp+8]
    fsub qword [f2_c]
    fld st0
    fld st0
    fmulp
    fmulp
    fld1
    fsubp
    leave
    ret

; 3*(x-2)^2
global f2d
f2d:
    enter 8,0
    fld qword [ebp+8]
    fsub qword [f2_c]
    fld st0
    fmulp
    fmul qword [f2d_k]
    leave
    ret

; 3/x
global f3
f3:
    enter 8,0
    fld qword [f3_k]
    fdiv qword [ebp+8]
    leave
    ret

; -3/x^2
global f3d
f3d:
    enter 8,0
    fld qword [f3_k]
    fdiv qword [ebp+8]
    fdiv qword [ebp+8]
    fchs
    leave
    ret