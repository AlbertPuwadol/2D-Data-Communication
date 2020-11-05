def bcd(n):
    if n<10:
        str_decimal ='00' + str(n)
    
    elif n<100:
        str_decimal ='0' + str(n)
    else:    
        str_decimal = str(n)
    digit = [int(c) for c in str_decimal ]
    return [format(d,'04b') for d in digit]

print(bcd(7))
print(int(b'00000010'))