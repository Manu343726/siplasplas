import re
from clang.cindex import Cursor, CursorKind

class Annotation(object):
    """Represents an annotation in the clang AST

    Annotations are text notes aplied to a C++ entity
    (Class declaration, function, declaration, etc) through
    the clang __attribute__((annotate())) extension.

    At this level annotations are just text with no specific semantics,
    see Attribute class hierarchy for semantics.
    """

    _FULL_ANNOTATION_REGEX = re.compile('(.*)::(.+)')
    _PAREMETERIZED_BODY_REGEX = re.compile('(.+)\((.*)\)')

    def __init__(self, cursor, string):
        self.cursor = cursor
        self.text = string
        self.annotated_node = None
        matchFull = Annotation._FULL_ANNOTATION_REGEX.match(string)

        if matchFull:
            self.namespace, self.body = matchFull.groups()
        else:
            self.namespace = ''
            self.body = string

        matchBodyParams = Annotation._PAREMETERIZED_BODY_REGEX.match(self.body)

        if matchBodyParams:
            self.class_name = matchBodyParams.group(1)
            self.ctor_args = matchBodyParams.group(2)
        else:
            self.class_name = self.body
            self.ctor_args = []

    def at_global_namespace(self):
        return not self.namespace

    def cpp_namespace(self):
        return self.namespace

    def python_module(self):
        return self.namespace.replace('::', '.')

    def is_orphan(self):
        return self.annotated_node is None

    @staticmethod
    def get_node_annotations(node):
        return node.translation_unit.match_annotations(node)

    def __str__(self):
        return self.__dict__
