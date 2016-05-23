from clang.cindex import CursorKind
from ast.classes import Class
from ast.enums import Enum
from ast.node import Node
import utility.namespace

class Namespace(Node):

    @staticmethod
    def MEMBERS_MAPPING():
        return {
            CursorKind.NAMESPACE: Namespace,
            CursorKind.CLASS_DECL: Class,
            CursorKind.STRUCT_DECL: Class,
            CursorKind.ENUM_DECL: Enum
        }

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

        if self.parent is None:
            self._namespace = utility.namespace.Namespace.global_namespace()
        else:
            self._namespace = self.parent._namespace.child(self.displayname)

    @staticmethod
    def global_namespace(translation_unit):
        """ Returns the node representing the global (outermost)
            namespace of a given translation unit
        """

        return Namespace.create_node(cursor = translation_unit.cursor, translation_unit = translation_unit, file = translation_unit.file)
