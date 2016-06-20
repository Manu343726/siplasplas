from clang.cindex import CursorKind
from ast.node import Node
from ast.enums import Enum
from ast.functions import Method, FunctionFamily
from ast.fields import Field
from ast.macroinstance import MacroInstance
from ast.unknown import Unkwnown


class Class(Node):

    @staticmethod
    def MEMBERS_MAPPING():
        return {
            CursorKind.CLASS_DECL: Class,
            CursorKind.STRUCT_DECL: Class,
            CursorKind.ENUM_DECL: Enum,
            CursorKind.CXX_BASE_SPECIFIER: BaseClass,
            CursorKind.CXX_METHOD: Method,
            CursorKind.FIELD_DECL: Field,
            CursorKind.MACRO_INSTANTIATION: MacroInstance,
            CursorKind.UNEXPOSED_DECL: Unkwnown
        }

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

    def is_forward_declaration(self):
        """ The python bindings don't seem to expose any way
            to check wether a class declaration is a forward
            declaration. (Check this tip from Don Williamson https://github.com/Celtoys/clReflect/blob/master/src/clReflectScan/ASTConsumer.cpp#L680)

            So here we are, examining tokens to see if the
            class declaration ends with a }; or a ; ...
        """

        tokens = [t for t in self.cursor.get_tokens()]

        if len(tokens) >= 2:
            closing_tokens = ''.join([t.spelling for t in tokens[-2:]])

            return closing_tokens != '};'
        else:
            return False

    def process(self):
        """ Merge common Method nodes into
            MethodFamilies

            Also skip this node if is a class forward declaration
        """
        if self.is_forward_declaration():
            self.set_skip()

        self.children['methodfamily'] = {}

        if not self.reflection_enabled:
            return

        for _, node in self.children['method'].iteritems():
            if node.spelling in self.children['methodfamily']:
                self.children['methodfamily'][node.spelling].append(node)
            else:
                self.children['methodfamily'][node.spelling] = FunctionFamily(node)

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
        return True

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
