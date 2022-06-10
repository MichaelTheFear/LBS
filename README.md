# LBS interpreter

## List of commands

### Generate values

- [ ] $n -> delcaration of any explicit number
- [ ] v[0-4] -> variables (max number of variables are 5)
- [ ] p0 -> argument of function

### Opeators

- [ ] = -> assign
- [ ] - -> minus
- [ ] + -> sum
- [ ] * -> multiplicate

### Function related

- [ ] p0 -> argument of function
- [ ] function -> where function starts
- [ ] call n -> calls function number n (n can't be a variable)
- [ ] ret -> return of function
- [ ] zret -> returns if 0

### Examples of assigning

```LBS
v0 = $100 - $20

v1 = p0 + v0

p0 = v1 * v1

p0 = call 0 p0
```

### Examples of returning

```LBS
ret $100

ret p0 + $100

zret $0 call 0 p0 

zret call 2 call 1 call 0 call 3 p0 call 0 call 3 call 2 call 1 p0 + $2
```

### Examples of call

```LBS
call 0 $10
call 1 $30 + p0
call 2 call 1 call 0 $3
```

## Full code LBS examples

Function that returns 100

```LBS
function
ret $100
end
```

Function that adds one to its input

```LBS
function
v0 = p0 + $1
ret v0
end
```
