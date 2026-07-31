MODEL PSPMOD           -- NB! 6 character name limit 

-- Start header. Do not modify the type-94 header. 
 comment---------------------------------------------------------------
 | First, declarations required for any type 94 Thevenin model         |
 | - these data and input values are provided to the model by ATP      |
 | - these output values are used by ATP                               |
 | - these names can be changed, except 'n', but not their order       |
 -------------------------------------------------------------endcomment

DATA  n                -- number of node pairs
      n2 {dflt: n*n}   -- number of matrix elements

INPUT vth[1..n]  -- Thev. voltage(t) across each branch
      rth[1..n2] -- Thev. resistance(t) matrix, symmetrical
      gth[1..n2] -- inverse of rth
      v0[1..n]   -- voltage(t=0) across each branch
      i0[1..n]   -- current(t=0) into each branch

VAR   v[1..n]    -- calculated voltage(t) across each branch
      i[1..n]    -- calculated current(t) into each branch

OUTPUT i[1..n]

 comment---------------------------------------------------------------
 | Next, declarations of user-defined data for this particular model   |
 | - their value is defined at the time of using the type-94 component |
 -------------------------------------------------------------endcomment
-- End header

--DATA

 comment---------------------------------------------------------------
 | Next, declarations provate to this model                            |
 -------------------------------------------------------------endcomment

VAR
    rmd[1..n]
    
DATA
    pspSS
    
MODEL PSP FOREIGN PSP_MODEL
{ixdata:4,ixin:2*n+1,ixout:n,ixvar:2}

EXEC
for i := 1 to n do
    rmd[i] := rth[(i-1)*n + i]
endfor

USE PSP AS PSP

    DATA
        xdata[1] := n
        xdata[2] := stoptime
        xdata[3] := timestep
        xdata[4] := pspSS

    INPUT
        xin[1..n] := vth[1..n]
        xin[n+1..2*n] := rmd[1..n]
        xin[2*n+1] := t

    OUTPUT
        i[1..n] := xout[1..n]

    ENDUSE

ENDEXEC

ENDMODEL