from clang.cindex import Cursor, CursorKind, TokenGroup
from ast.attributes.attribute import Attribute
from ast.attributes.annotation import Annotation
from utility.namespace import Namespace

def print_extent(extent):
    return '(l: {} c: {}, l: {}, c: {})'.format(extent.start.line, extent.start.column,
            extent.end.line, extent.end.column)

def text_in_cursor(cursor):
    txt = ''
    for t in cursor.get_tokens():
        if t.spelling:
            txt += ' ' + t.spelling
        else:
            txt += ' ' + t.displayname

    return '{}: \'{}\''.format(print_extent(cursor.extent), txt)


class Node(object):
    """Base class of all processed AST nodes

    An AST node has the original AST node (libclang.cindex.Cursor)
    info plus parsed attributes associated with it
    """

    def __init__(self, cursor, parent = None):
        self.cursor = cursor
        self.parent = parent
        self.initialize_children() 

        self.attributes = Attribute.get_cursor_attributes(cursor)

    def print_ast_node(self):
        if self.cursor.kind == CursorKind.UNEXPOSED_DECL:
            short = '(Unexposed decl) \'{}\''.format(text_in_cursor(self.cursor))
        else:
            short = "{} ({})".format(self.fullname, str(self.kind).split(".")[1])

        if self.attributes:
            return short + '\n' + '\n =>'.join([a.description() for a in self.attributes])
        else:
            return short

    def get_children(self):
        for c in self.cursor.get_children():
            yield Node(c)

    @classmethod
    def create_child(nodeClass, cursor, parent = None):
        if hasattr(nodeclass, 'MEMBERS_MAPPING'):
            mapping = nodeClass.MEMBERS_MAPPING

            if cursor.kind in mapping:
                class_ = mapping[cursor.kind]
                return class_(cursor, parent)
        else:
            return None

    def initialize_children(self):
        self.children = {}
        if not hasattr(self.__class__, 'MEMBERS_MAPPING'):
            return

        mapping = self.__class__.MEMBERS_MAPPING

        for childKind in [class_.node_class_kind() for class_ in set(mapping.values())]:
            self.children[childKind] = {}

    @property
    def spelling(self):
        return self.cursor.spelling

    @property
    def displayname(self):
        return self.cursor.displayname

    @property
    def name(self):
        if self.spelling:
            return self.spelling
        else:
            return '[unknown spelling (displayname: \'{}\')'.format(self.displayname)

    @property
    def kind(self):
        return self.cursor.kind

    @classmethod
    def node_class_kind(nodeClass):
        return nodeClass.__name__.lower()

    @property
    def fullname(self):
        if self.parent is None:
            return self.name
        else:
            return Namespace.SCOPE_OPERATOR.join(self.parent.fullname, self.name)
