from clang.cindex import Cursor, CursorKind
from ast.attributes.attribute import Attribute
from ast.attributes.annotation import Annotation

class Node(object):
    """Base class of all processed AST nodes

    An AST node has the original AST node (libclang.cindex.Cursor)
    info plus parsed attributes associated with it
    """

    def __init__(self, cursor):
        self.cursor = cursor

        self.attributes = Attribute.get_cursor_attributes(cursor)

    def print_ast_node(self):
        short = "{} ({})".format(self.name or self.full_name, str(self.kind).split(".")[1])

        if self.attributes:
            return short + '\n' + '\n =>'.join([a.description() for a in self.attributes])
        else:
            return short

    def get_children(self):
        for c in self.cursor.get_children():
            yield Node(c)

    @property
    def name(self):
        return self.cursor.spelling

    @property
    def full_name(self):
        return self.cursor.displayname

    @property
    def kind(self):
        return self.cursor.kind
