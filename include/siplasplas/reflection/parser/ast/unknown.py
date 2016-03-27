from ast.node import Node, text_in_cursor, print_extent
from clang.cindex import Cursor

class Unkwnown(Node):

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

    def print_ast_node(self):
        return '{} unknown node \'{}\''.format(
                print_extent(self.cursor.extent),
                text_in_cursor(self.cursor)
        )
