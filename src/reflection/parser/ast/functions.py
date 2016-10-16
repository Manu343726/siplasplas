from ast.node import Node

class Method(Node):

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)

    @property
    def argument_types(self):
        return self.displayname[(len(self.spelling) + 1):-1]

    def process(self):
        # Skip static methods and the assignment operator:
        # Because we not consider statics to be member functions
        # and operator= breaks the generator code
        if self.cursor.is_static_method() or self.cursor.spelling == 'operator=':
            self.set_skip()


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

    def __getitem__(self, index):
        return self.functions[index]

    def print_ast_node(self):
        return '(function family) {}{}'.format(
            self.spelling,
            '\n - '.join([''] + [f.displayname for f in self.functions])
        )
