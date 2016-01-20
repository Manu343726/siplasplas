import clang
import clang.cindex
import sys
from clang.cindex import CursorKind

class Field:
    def __init__(self, cursor):
        self.cursor = cursor
        self.name = cursor.spelling


class Function:
    def __init__(self, cursor):
        self.cursor = cursor
        self.name = cursor.spelling


class Class:
    def __init__(self, cursor):
        self.cursor = cursor
        self.className = cursor.spelling
        self.fields = []
        self.functions = []
        self.enable_reflection_tag = None

        print 'Analyzing class \'{}\'...'.format(self.className)

        for child in cursor.get_children():
            print ' - Found \'{}\' ({})'.format(child.spelling, child.kind)

            if child.kind == CursorKind.CXX_METHOD:
                self.functions.append(Function(child))
            elif child.kind == CursorKind.FIELD_DECL:
                self.fields.append(Field(child))
            elif child.spelling == 'EnableReflection':
                print '>>>>> Found reflection tag for class \'{}\''.format(self.className)
                self.enable_reflection_tag = child
                print '>>>>> Reflection enabled: {}'.format(self.has_reflection())

    def has_reflection(self):
        return self.enable_reflection_tag is not None

    def reflection_body(self):
        fields = ',\n                '.join(['SIPLASPLAS_REFLECTION_FIELD({},{})'.format(self.className, field.name) for field in self.fields])    
        functions = ',\n                '.join(['SIPLASPLAS_REFLECTION_FUNCTION({},{})'.format(self.className, function.name) for function in self.functions])    

        body = (
"""
public:
    static ::cpp::MetaClassData& reflection() {{
        static ::cpp::MetaClassData& data = []{{
            ::cpp::MetaClass::registerMetaClass<{0}>({{ 
                {1} 
            }}, {{ 
                {2} 
            }});
            return ::cpp::MetaClass::getClass<{0}>();
        }};
        return data;
    }}

    ::cpp::BindedMetaClassData<{0}> instanceReflection() const {{
        return {{*this, reflection()}};
    }}""")

        return body.format(self.className, fields, functions)

    def __str__(self):
        return (
"""======================
class \'{0}\':
======================
 - Fields: {1}
 - Functions: {2}
"""                 ).format(self.className, 
                        ', '.join([field.name for field in self.fields]),
                        ', '.join([function.name for function in self.functions])
                )
  

def process_reflection(node):
    reflectionClass = None
    classes = []

    if node.kind == CursorKind.CLASS_DECL or node.kind == CursorKind.STRUCT_DECL:
        reflectionClass = Class(node)    

    for child in node.get_children():
        classes += process_reflection(child)

    if reflectionClass is not None and reflectionClass.has_reflection():
        print 'Class \'{}\' saved'.format(reflectionClass.className)
        classes.append(reflectionClass)

    return classes


index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])
print 'Translation unit:', tu.spelling

reflectionClasses = process_reflection(tu.cursor)
print 'Finished.\n\n'

for reflectionClass in reflectionClasses:
    print reflectionClass
    print ' Reflection code: \n{}'.format(reflectionClass.reflection_body())
