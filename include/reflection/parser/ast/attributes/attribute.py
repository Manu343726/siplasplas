from clang.cindex import CursorKind
from utility.logger import GlobalLogger
from ast.attributes.annotation import Annotation
import importlib, inspect
import sys

class Attribute(object):
    def __init__(self, annotation):
        self.annotation = annotation

    @staticmethod
    def create(annotation):
        def error_exit():
            GlobalLogger.error().step('Cannot load \'{}\' attribute class from module \'{}\'. Attribute will be ignored.'.format(annotation.class_name, annotation.python_module()))
            return None

        try:
            if not annotation.at_global_namespace:
                module = importlib.import_module(annotation.python_module())
                class_ = getattr(module, annotation.class_name)
            else:
                imported_classes = inspect.getmembers(sys.modules[__name__], inspect.isclass)
                for name, class_ in imported_classes:
                    if name == annotation.class_name:
                        break;
                    else:
                        class_ = None

        except AttributeError, ImportError:
            return error_exit()

        if inspect.isclass(class_):
            return class_(annotation, *annotation.ctor_args)
        else:
            return error_exit()

    @staticmethod
    def get_cursor_attributes(cursor):
        return [attr for attr in [Attribute.create(x)
            for x in Annotation.get_cursor_annotations(cursor)] if attr is not None]

    def description(self):
        return 'Line {} (On \'{}\' {}) Attribute \'{}\'. Params: {}'.format(self.annotation.cursor.location.line, self.annotation.cursor.displayname, self.annotation.cursor.kind, self.annotation.class_name, self.annotation.ctor_args)


class ExampleAttributeClass(Attribute):
    def __init__(self, annotation, message, number):
        Attribute.__init__(self, annotation)
        GlobalLogger.info().info(message)
