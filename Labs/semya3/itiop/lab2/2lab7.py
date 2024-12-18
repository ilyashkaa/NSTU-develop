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

def hex_to_float(number):
    number = ten_to_r(r_to_ten(number, 16), 2)
    if len(number) == 63 or len(number) == 31:
        number = "0" + number
    fl = number[0]
    if len(number) == 64:
        exp = number[1:12]
        power = r_to_ten(exp, 2) - 1023
        mantiss = "1" + number[12:]
        mantiss = mantiss[:mantiss.rfind("1") + 1]
        mantiss = mantiss[:power + 1] + "." + mantiss[power + 1:]
        if fl == "1":
            mantiss = -1 * r_to_ten(mantiss, 2)
        else:
            mantiss = r_to_ten(mantiss, 2)
        return mantiss

    if len(number) == 32:
        exp = number[1:9]
        power = r_to_ten(exp, 2) - 127
        mantiss = "1" + number[9:]
        mantiss = mantiss[:mantiss.rfind("1") + 1]
        mantiss = mantiss[:power + 1] + "." + mantiss[power + 1:]
        if fl == "1":
            mantiss = -1 * r_to_ten(mantiss, 2)
        else:
            mantiss = r_to_ten(mantiss, 2)
        return mantiss

n = "405FA70000000000"
print(f"{n} как число с плавающей запятой: {hex_to_float(n)}")

    