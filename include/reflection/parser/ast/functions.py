from ast.node import Node

class Method(Node):

    def __init__(self, **kwargs):
        Node.__init__(self, **kwargs)


class OverloadedMethod(Method):

    def __init__(self, **kwargs):
        Method.__init__(self, **kwargs)


class MethodFamily(Node):

    def __init__(self, **kwargs):
        cursor = kwargs['cursor']
        parent = kwargs['parent']

        if cursor.spelling in parent.children['methodfamily']:
            family = parent.children['methodfamily'][cursor.spelling]
            family[cursor.displayname] = OverloadedMethod.create_node(cursor, parent)
        else:
            if cursor.spelling in parent.children['method']:
                parent.children['methodfamily'][cursor.spelling] = {}
                parent.children['methodfamily'][cursor.spelling][cursor.displayname] = OverloadedMethod.create_node(cursor, parent)
            else:
                if parent.children['methodfamily']
                    parent.children['method'][cursor.spelling] = Method.create_node(cursor, parent)
