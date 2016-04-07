from clang.cindex import Cursor, CursorKind
from ast.namespaces import Namespace
from ast.attributes.annotation import Annotation

class TranslationUnit(object):
    """ Represents a processed translation unit

        Unlike libclang does, a translation unit is not part of the AST itself
        but its container. The root level of the processed AST is the global
        namespace.
    """

    def __init__(self, cursor):
        self.cursor = cursor
        self.annotations = []

        # Maybe the most tricky part of the parser:
        #
        # libclang doesn't add preprocessor data to the AST by default
        # (Makes sense since, as the name suggest, pre-processing doesn't
        # form part of compilation itself).
        #
        # You can however explicitly ask libclang to leave translation unit
        # preprocessing info in the AST, so you can see include directives, macros,
        # etc. But since preprocessing is done before generating the AST itself,
        # libclang has no contextual information for preprocessor entities. In other
        # words, it doesn't know in what scope (What class, function, etc) a macro was instanced.
        # So it puts all the preprocessor things at the top level of the AST.
        #
        #
        # How do you find the context where a macro was instanced? Here's a trick:
        #
        # Store all macro instantiations, and when processing the AST down, remember
        # to match the current visited node with the list of "still not matched" macros.
        # My criteria was that a macro is applied to a node (i.e. "matches") if it
        # was instanced the line before the node.
        #
        # It was done this way since the primary goal of processing macros was to find annotations
        # used to implement attributes:
        #
        #     $(an attribute)
        #     void function();
        #

        for macro in self.cursor.get_children():
            if macro.kind == CursorKind.MACRO_INSTANTIATION and \
               macro.spelling in ('$', 'SIPLASPLAS_ANNOTATION'):
                text = ' '.join([t.spelling for t in macro.get_tokens()][2:-2])
                self.annotations.append(Annotation(macro, text))

        self.mismatched_annotations = list(self.annotations)
        self.mismatched_annotations.sort(key = lambda a: a.cursor.location.line)

        self.root = Namespace.global_namespace(self)


    def match_annotations(self, node):
        """ Returns the list of annotations associated with a node """

        if self.mismatched_annotations:
            for macro in self.mismatched_annotations:
                if str(macro.cursor.location.file) == str(node.cursor.location.file) and \
                    macro.cursor.extent.end.line == node.cursor.location.line - 1: # note we match extent.end.line, not location.line
                                                                                   # Beware of multiline annotations
                    self.mismatched_annotations.remove(macro)
                    macro.annotated_node = node
                    return [macro] # Only one annotation per node is supported (yet)

            return []
        else:
            return []

    def nodes(self):
        """ Gives an iterable to visit all AST nodes recursively """

        class nodeiter:
            def __init__(self, node):
                self.node = node

            def __iter__(self):
                childs = [nodeiter(c) for c in self.node.get_children()]

                for child in childs:
                    for c in child:
                        yield c

                yield self.node

        return nodeiter(self.root)
