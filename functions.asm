section .rodata
    f1_k: dt 0.6
    f1_b: dt 3.0
    f2_c: dt 2.0
    f2d_k: dt 3.0
    f3_k: dt 3.0

section .text
; x: ld (ebp+8)
; -> ld

; 0.6x + 3
global f1
f1:
    enter 8,0
    fld tword [ebp+8]
    fld tword [f1_k]
    fmulp
    fld tword [f1_b]
    faddp
    leave
    ret

; 0.6
global f1d
f1d:
    enter 8,0
    fld tword [f1_k]
    leave
    ret

; (x-2)^3 - 1
global f2
f2:
    enter 8,0
    fld tword [ebp+8]
    fld tword [f2_c]
    fsubp
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
    fld tword [ebp+8]
    fld tword [f2_c]
    fsubp
    fld st0
    fmulp
    fld tword [f2d_k]
    fmulp
    leave
    ret

; 3/x
global f3
f3:
    enter 8,0
    fld tword [f3_k]
    fld tword [ebp+8]
    fdivp
    leave
    ret

; -3/x^2
global f3d
f3d:
    enter 8,0
    fld tword [f3_k]
    fld tword [ebp+8]
    fdivp
    fld tword [ebp+8]
    fdivp
    fchs
    leave
    ret
