from clang.cindex import CursorKind
from utility.logger import GlobalLogger
from ast.attributes.annotation import Annotation
import importlib, inspect
import sys

class Attribute(object):
    """ Implements a node attribute

        On the C++ side, an attribute is any class that implements
        the cpp::attributes::Attribute interface.
        The parser takes an annotation and interprets its text as an
        instantiation of the above interface.

        For example:

            class MyAttribute : public cpp::attributes::Attribute
            {
                ...
            };

            $(std::make_shared<cpp::attributes::Attribute>(new MyAttribute()))
            void function();

        Check out the code generation template to see how that code is being used
        by different entities.
    """

    def __init__(self, annotation):
        self.annotation = annotation

    @staticmethod
    def get_node_attributes(node):
        """ Asks the translation untit for the set of annotations
            applied to a node, then interprets them as attributes
        """

        return [attr for attr in [Attribute(x)
            for x in Annotation.get_node_annotations(node)] if attr is not None]

    @property
    def code(self):
        return self.annotation.text

    def description(self):
        return '(On \'{}\') (attribute) \'{}\''.format(self.annotation.annotated_node.displayname, self.code)
