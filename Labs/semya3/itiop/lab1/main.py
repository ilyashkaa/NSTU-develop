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
        return ".".join(["".join(reversed(r_int_part)), "".join(r_float_part)])


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


def r_sum(num1, num2, r):
    """Складывает два вещественных числа в r-ичной системе счисления."""
    num1 = r_to_ten(num1, r)
    num2 = r_to_ten(num2, r)
    ss = ten_to_r(num1 + num2, r)
    return ss


def r_min(num1, num2, r):
    """Складывает два вещественных числа в r-ичной системе счисления."""
    num1 = r_to_ten(num1, r)
    num2 = r_to_ten(num2, r)
    ss = ten_to_r(num1 - num2, r)
    return ss


def r_multy(num1, num2, r):
    """Складывает два вещественных числа в r-ичной системе счисления."""
    num1 = r_to_ten(num1, r)
    num2 = r_to_ten(num2, r)
    ss = ten_to_r(num1 * num2, r)
    return ss

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


while True:
    print("Выберите действие:")
    print("1. Перевести из десятичной в r-ичную")
    print("2. Перевести из r-ичной в десятичную")
    print("3. Сложить два числа в r-ичной системе")
    print("4. Вычесть два числа в r-ичной системе")
    print("5. Умножить два числа в r-ичной системе")
    print("6. Выход")

    choice = input("Введите номер действия: ")

    if choice == "1":
        decimal_num = float(input("Введите десятичное число: "))
        r = int(input("Введите основание системы счисления: "))
        print(f"Число в {r}-ичной системе: {ten_to_r(decimal_num, r)}")
    elif choice == "2":
        r_num = input("Введите число в r-ичной системе: ")
        r = int(input("Введите основание системы счисления: "))
        print(f"Число в десятичной системе: {r_to_ten(r_num, r)}")
    elif choice == "3":
        num1 = input("Введите первое число: ")
        num2 = input("Введите второе число: ")
        r = int(input("Введите основание системы счисления: "))
        print(f"Сумма: {r_sum(num1, num2, r)}")
    elif choice == "4":
        num1 = input("Введите первое число: ")
        num2 = input("Введите второе число: ")
        r = int(input("Введите основание системы счисления: "))
        print(f"Разность: {r_min(num1, num2, r)}")
    elif choice == "5":
        num1 = input("Введите первое число: ")
        num2 = input("Введите второе число: ")
        r = int(input("Введите основание системы счисления: "))
        print(f"Произведение: {r_multy(num1, num2, r)}")
    elif choice == "6":
        break
    else:
        print("Неверный выбор")