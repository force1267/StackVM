# stackVM
### vm - assm - minimal
stackVM is what its name says.
the project contains a vm with about 50 binary commands,
an assmbler called assm, and a compiler with a language called minimal.

project goal is to prepare a base for further experiments with stack machines.

it is not yet completed ( and not expected too be completed soon ).
and it is not intended to use in production environments.

#### make :
``` bash
    $> make clean all
    $> make _vm _assm _minimal _scripts
```

## vm :

vm binaries are 32 bit integers summed with 0x40000000
registers are :
acc, tmp, sp, pc, err,
and br( bias register, sp and pc are always summed with br )
tos is top of stack, not a register

#### machine control
##### 0: halt
halts the machine
##### 27: dupl (tos) -> tos
duplicates top of stack
##### 28: detemp (tmp) -> acc
copies tmp to acc
##### 29: temp (acc) -> temp
copies acc to tmp
##### 30: push (acc) -> tos
pushes acc to stack
##### 31: pop (tos) -> acc
pops from stack and copies to acc
#### arithmetics and calculations
##### 32: add (acc, tos) -> acc
add acc and tos and pushes to stack
##### 33: sub (acc, tos) -> acc
subs
##### 34: mul (acc, tos) -> acc
muls
##### 35: div (acc, tos) -> acc
division.
divide by zero will set err to 64
##### 36: mod (acc, tos) -> acc
mod
divide by zero will set err to 64
##### 37: and (acc, tos) -> acc
bitwise and
##### 38: or (acc, tos) -> acc
bitwise or
##### 39: xor (acc, tos) -> acc
bitwise xor
##### 40: not (acc) -> acc
bitwise not
##### 41: lshift (acc, tos) -> acc
bitwise left shift of 'acc' ,'tos' times
##### 42: rshift (acc, tos) -> acc
bitwise right shift of 'acc' ,'tos' times
##### 43: eq (acc, tos) -> acc is acc == tos
== bool check
##### 44: lt (acc, tos) -> acc is acc < tos
< bool check
##### 45: gt (acc, tos) -> acc is acc > tos
> bool check
#### program flow
##### 128: skip
like pass in python.
wastes a cycle of proccesor
##### 129: goto (acc)
pc = acc - 1 (+ br)
##### 130: test (tos, acc)
if(tos) pass else goto acc
##### 131: handled ()
continue last instructions
err = 67; invalid 'handled' instruction
interrupt handled.
##### 132: handle (acc)
handle situation of 'acc' number
handle interrupt acc
##### 133: handler (acc, tos)
set a high priority task handler (interrupt handler) handler for situation of 'acc' number is handled by instructions starting from 'tos' number higher 'acc' means higher priority
##### 134: bias (acc) -> br
br = acc
##### 135: err (err) -> acc
acc = err
err = 0
##### 255: stay (acc)
stay 'acc' cycles (not implemented)
it's skip times acc !
##### 256: read (acc) -> acc
acc = memory[acc]
##### 257: write (acc, tos)
memory[acc] = tos

## assm
assmbles things like below code to binary
```
(128) :main (10) {
    .dbz push 64 handler
    2 push
    2 sub push
    12 div push
    6 sub push
    skip
    halt
}
:dbz (9) {
    err 754 push
    skip
    halt
}
```
##### :abc
makes a pointer called 'abc' to this location in code
##### .abc
puts address of 'abc' to this location in code
##### (n)
reserves n words of memory starting at this location of code
if the code is started with (n), like (128) at the start of above code,
br will also be set to n at start and the n saved words are used to point
to interrupt handlers.

## minimal
working on it !