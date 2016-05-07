from clang.cindex import Cursor, CursorKind
from ast.node import Node
from utility.code_generation import string_to_char_pack

class Enum(Node):

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

        self.constants = [cursor.spelling for cursor in self.cursor.get_children() if cursor.kind == CursorKind.ENUM_CONSTANT_DECL]

    @property
    def constants_as_charpacks(self):
        return [string_to_char_pack(constant) for constant in self.constants]
