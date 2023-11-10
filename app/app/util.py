def convert_bool_array_to_int(bool_array):
    return int("".join("1" if x else "0" for x in bool_array), 2)
