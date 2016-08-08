
def string_to_char_pack(string):
    return ', '.join(['\'{}\''.format(char) if char != '\\' else '\'\\\\\'' for char in string])
