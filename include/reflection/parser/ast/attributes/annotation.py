import re
from clang.cindex import Cursor, CursorKind

class Annotation(object):
    """Represents an annotation in the clang AST

    Annotations are text notes aplied to a C++ entity
    (Class declaration, function, declaration, etc) through
    the clang __attribute__((annotate())) extension.

    Our annotations try to mimic C++11 generalized attributes
    syntax. An annotation has both a namespace (global by default)
    and a body:

        "namespace::body"

    At this level annotations are just text with no specific semantics,
    see Attribute class hierarchy for semantics.
    """

    _FULL_ANNOTATION_REGEX = re.compile('(.*)::(.+)')
    _PAREMETERIZED_BODY_REGEX = re.compile('(.+)\((.*)\)')

    def __init__(self, cursor, string):
        self.cursor = cursor
        self.text = string
        matchFull = Annotation._FULL_ANNOTATION_REGEX.match(string)

        if matchFull:
            self.namespace, self.body = matchFull.groups()
        else:
            self.namespace = ''
            self.body = string

        matchBodyParams = Annotation._PAREMETERIZED_BODY_REGEX.match(self.body)

        if matchBodyParams:
            self.class_name = matchBodyParams.group(1)
            self.ctor_args = eval('[{}]'.format(matchBodyParams.group(2)))

            if self.ctor_args and self.ctor_args[0] == '':
                self.ctor_args = []
        else:
            self.class_name = self.body
            self.ctor_args = []

    def at_global_namespace(self):
        return not self.namespace

    def cpp_namespace(self):
        return self.namespace

    def python_module(self):
        return self.namespace.replace('::', '.')

    @staticmethod
    def get_cursor_annotations(cursor):
        return [Annotation(cursor, c.displayname) for c in cursor.get_children()
                if c.kind == CursorKind.ANNOTATE_ATTR]

    def __str__(self):
        return self.__dict__
