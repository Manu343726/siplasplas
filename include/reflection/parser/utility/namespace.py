
class Namespace(object):
    SCOPE_OPERATOR = '::'
    GLOBAL_NAMESPACE = Namespace()

    def __init__(self, string = '', splitted = []):
        if string and not splitted:
            self._string = string
            self._splitted = string.split(Namespace.SCOPE_OPERATOR)
        else:
            self._splitted = splitted
            self._string = splitted.join(Namespace.SCOPE_OPERATOR)

    @property
    def parent(self):
        if self.is_global_namespace():
            return self
        else:
            return Namespace(self._splitted[:-1]

    def child(self, namespace):
        return Namespace(self._splitted + namespace)

    def get_text(self):
        return self._string


