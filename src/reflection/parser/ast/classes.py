from clang.cindex import CursorKind
from ast.node import Node
from ast.functions import Method
from ast.fields import Field
from ast.macroinstance import MacroInstance
from ast.unknown import Unkwnown


class Class(Node):

    @staticmethod
    def MEMBERS_MAPPING():
        return {
            CursorKind.CLASS_DECL: Class,
            CursorKind.STRUCT_DECL: Class,
            CursorKind.CXX_BASE_SPECIFIER: BaseClass,
            CursorKind.CXX_METHOD: Method,
            CursorKind.FIELD_DECL: Field,
            CursorKind.MACRO_INSTANTIATION: MacroInstance,
            CursorKind.UNEXPOSED_DECL: Unkwnown
        }

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

    def reflection_enabled(self):
        """ Checks whether this class has reflection support enabled

            Class reflection is enabled by setting an 'enable_reflection'
            attribute to the class, like this:

            $(enable_reflection)
            class MyClassWithReflection
            {
               ...
            };
        """

        for attr in self.attributes:
            if attr.code == 'enable_reflection':
                return True

        return False

    @property
    def className(self):
        return self.spelling

    @property
    def declarationKind(self):
        if self.kind == CursorKind.STRUCT_DECL:
            return 'struct'
        else:
            return 'class'


class BaseClass(Class):

    def __init__(self, cursor, parent):
        Class.__init__(self, cursor = cursor.referenced, parent = parent, translation_unit = None)
