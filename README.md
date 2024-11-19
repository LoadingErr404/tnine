# tnine
C program simulating old phone systems

## What is it?
simple C code with simple decoding of contacts and numbers

## How does it work?
The program takes simple number sequence input nad text file with contacts -> searches within contacts

Returns contacts that:

a) contain input number in their phone number

b) input number after decoding is part of contact's name

## Example
### Input file:

Petr Dvorak

603123456

Jana Novotna

777987654

Bedrich Smetana ml.

541141120


### Input number: 12
### Return:
petr dvorak, 603123456

bedrich smetana ml., 541141120

## How to run
In console:

./tnine NUMBER <file.txt

NUMBER = number we want to search for/decode

file.txt = file with contacts (example file: testFile.txt)

## Decoding table
2 (abc), 3 (def), 4 (ghi), 5 (jkl), 6 (mno), 7 (pqrs), 8 (tuv), 9 (wxyz), 0 (+)

222 -> c

3 -> d

77 -> q
