from clang.cindex import CursorKind
from ast.class import Class
from ast.node import Node
import utility.namespace

class Namespace(Node):
    MEMBERS_MAPPING = {
        CursorKind.NAMESPACE_DECL: Namespace,
        CursorKind.CLASS_DECL: Class,
        CursorKind.STRUCT_DECL: Class
    }

    def __init__(self, cursor, parent):
        Node.__init__(cursor, parent)

        self._namespace = parent._namespace.append(self.displayname)

        for c in cursor.get_children():
            child = Class.create_child(c, self)
            if child is not None:
                self.children[child.node_class_kind()][child.name] = child

