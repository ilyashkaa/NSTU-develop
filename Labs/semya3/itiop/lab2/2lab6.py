def ten_to_r(decimal_num, r):
    """Преобразует вещественное десятичное число в r-ичную систему счисления."""
    int_part = int(decimal_num)
    float_part = decimal_num - int_part
    r_int_part = []
    r_float_part = []
    fl = 0
    if float_part == 0:
        fl = 1

    while int_part > 0:
        r_int_part.append(convert_n_to_sym(int_part % r))
        int_part //= r

    while float_part > 0:
        float_part *= r
        r_float_part.append(convert_n_to_sym(int(float_part)))
        float_part -= int(float_part)

    if fl:
        return "".join(reversed(r_int_part))
    else:
        return ["".join(reversed(r_int_part)), "".join(r_float_part)]

def r_to_ten(r_num, r):
    """Преобразует вещественное число в r-ичной системе счисления в десятичную."""
    if r_num.find(".") != -1:
        int_part, float_part = r_num.split(".")

        decimal_int_part = 0
        for i, digit in enumerate(reversed(int_part)):
            decimal_int_part += convert_sym_to_n(digit) * (r ** i)

        decimal_float_part = 0
        for i, digit in enumerate(float_part):
            decimal_float_part += convert_sym_to_n(digit) * (r ** (-i - 1))

        return decimal_int_part + decimal_float_part
    else:
        int_part = r_num
        decimal_int_part = 0
        for i, digit in enumerate(reversed(int_part)):
            decimal_int_part += convert_sym_to_n(digit) * (r ** i)
        return decimal_int_part

def convert_n_to_sym(n):
  """Преобразует цифру в символ (для систем счисления больше 10)."""
  if n < 10:
    return str(n)
  else:
    return chr(ord('A') + n - 10)

def convert_sym_to_n(sym):
  """Преобразует символ в цифру (для систем счисления больше 10)."""
  if sym.isdigit():
    return int(sym)
  else:
    return ord(sym) - ord('A') + 10

def normal(ln, cur):
    if len(ln) < cur:
        ln += "0" * (cur - len(ln))
    elif len(ln) > cur:
        ln = ln[:cur]
    return ln

def float_to_hex(number, byte_length):
    if number < 0:
        number = ten_to_r(abs(number), 2)
        fl = "1"
    else:
        number = ten_to_r(number, 2)
        fl = "0"
    power = len(number[0]) - 1
    if byte_length == 4:
        exp = normal(ten_to_r(power + 127, 2), 8)
        mantiss = normal(number[0][1:] + number[1], 23)
    elif byte_length == 8:
        exp = normal(ten_to_r(power + 1023, 2), 11)
        print(number[0][1:] + number[1])
        mantiss = normal(number[0][1:] + number[1], 52)
    n = fl + exp + mantiss
    return ten_to_r(r_to_ten(n, 2), 16)

n = -123.5
a = float_to_hex(n, 8)
print(a)