from clang.cindex import CursorKind
from ast.node import Node
import ast.method, ast.field
import utility.namespace

class Class(Node):
    MEMBERS_MAPPING = {
        CursorKind.CLASS_DECL: Class,
        CursorKind.STRUCT_DECL: Class,
        CursorKind.CXX_BASE_SPECIFIER: BaseClass,
        CursorKind.CXX_METHOD: ast.function.Method,
        CursorKind.FIELD_DECL: ast.field.Field
    }

    def __init__(self, cursor, parent):
        Node.__init__(self, cursor, parent)

        self._generate_reflection_metadata = None

        for c in cursor.get_children():
            child = Class.create_child(c, self)
            if child is not None:
                self.children[child.node_class_kind()][child.name] = child

    def generate_reflection_metadata(self):
        if self._generate_reflection_metadata is None:
            self._generate_reflection_metadata = False

            for _, kindSet in self.members:
                for _, member in kindSet:
                    if member.generate_reflection_metadata():
                        self._generate_reflection_metadata = True
                        return True
        else:
            return self._generate_reflection_metadata



class BaseClass(Class):

    def __init__(self, cursor, parent):
        Class.__init__(self, cursor.referenced, parent)
