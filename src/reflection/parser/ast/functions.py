from ast.node import Node

class Method(Node):

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)


class OverloadedMethod(Method):

    def __init__(self, **kwargs):
        Method.__init__(self, **kwargs)


class FunctionFamily(Node):

    def __init__(self, function):
        self.cursor = function.cursor
        self.functions = [function]
        self.children = {}

    def append(self, function):
        self.functions.append(function)

    def print_ast_node(self):
        return '(function family) {}{}'.format(
            self.spelling,
            '\n - '.join([''] + [f.displayname for f in self.functions])
        )
