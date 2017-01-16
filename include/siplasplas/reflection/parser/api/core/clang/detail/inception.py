"""
This script generates a "cursorkindenum.hpp" header with a
C++11 version of the libclang CXCursorKind enumeration
(A 'Kind' enum class with no "CXCursor_" prefix in the
enumeration values)

The generated header has no include guards not namespace,
and is intended to be included directly at the point where
the enumeration is going to be declared (A namespace, a class, etc)

Invoke as follows:

    python inception.py path/to/clang/includes

where "path/to/clang/includes" is the full path to the clang
include directory (The one with 'clang' and 'clang-c' subdirectories)
"""


from clang.cindex import Index, Cursor, CursorKind
import os, sys, re

def align_doc(doc):
    processed_lines = []

    for line in doc.split('\n'):
        stripped = line.strip()
        aligned  = '{}{}'.format(' '*(0 if stripped.startswith('/') else 5), stripped)

        processed_lines.append(aligned)

    return '\n'.join(processed_lines)

def parse_cursorkind(cxcursorkind):
    print 'CXCursorKind found at {}:{}'.format(cxcursorkind.location.file, cxcursorkind.location.line)
    values = {}
    values['max_value'] = -sys.maxint - 1
    values['min_value'] = sys.maxint
    values['elements'] = {}

    name_pattern = re.compile('CXCursor_(.*)')

    for c in cxcursorkind.get_children():
        if c.kind != CursorKind.ENUM_CONSTANT_DECL:
            continue

        print ' - At line {}: {} (Kind={})'.format(c.location.line, c.spelling, c.kind)
        print '   "{} = {}"'.format(c.spelling, c.enum_value)

        match = name_pattern.match(c.spelling)

        if match:
            name = match.group(1)
            element = {}
            element['name'] = name
            element['spelling'] = c.spelling
            element['value'] = c.enum_value
            element['cursor'] = c

            if c.raw_comment is not None:
                element['doc'] = align_doc(c.raw_comment)
            else:
                element['doc'] = '// No documentation available'

            values['elements'][c.enum_value] = element

            if values['max_value'] < c.enum_value:
                print 'New max value: {}'.format(c.enum_value)
                values['max_value'] = c.enum_value

            if values['min_value'] > c.enum_value:
                print 'New min value: {}'.format(c.enum_value)
                values['min_value'] = c.enum_value
        else:
            print '   ERROR: Name match not found, skipping...'

    return values

def generate_serialization(values):
    with open('cursorkindenumserialization.hpp', mode='w+') as file:
        file.write("""
/*
 * Generated from clang-c/Index.h
 * See inception.py script at include/siplasplas/reflection/parser/api/core/clang/
 */

/**
 * \\brief returns An string representation of the given libclsng
 * ::CXCursorKind value, i.e. "CXCursor_ClassDecl"
 */
constexpr const char* kindString(const ::CXCursorKind kind)
{
    constexpr const char* strings[] = {""")
        for i in range(values['min_value'], values['max_value'] + 1):

            if i in values['elements']:
                element = values['elements'][i]
                slot = '"{}", // {} ({}:{}: name=\'{}\', value={})'.format(
                    element['spelling'],
                    i,
                    element['cursor'].location.file,
                    element['cursor'].location.line,
                    element['cursor'].spelling,
                    element['cursor'].enum_value
                )
            else:
                slot = '"<unknown ::CXCursorKind value ({index})>", // {index}'.format(index=i)

            file.write("""
        {}""".format(slot))

        file.write("""
    }};

    return strings[static_cast<std::size_t>(kind) - {min}];
}}

/**
 * \\brief Returns a simplified string representation of the given libclang
 * ::CXCursorKind value, i.e. "ClassDecl"
 */
constexpr const char* simpleKindString(const ::CXCursorKind kind)
{{
    constexpr const char* strings[] = {{""".format(min=values['min_value']))
        for i in range(values['min_value'], values['max_value'] + 1):

            if i in values['elements']:
                element = values['elements'][i]
                slot = '"{}", // {} ({}:{}: name=\'{}\', value={})'.format(
                    element['name'],
                    i,
                    element['cursor'].location.file,
                    element['cursor'].location.line,
                    element['cursor'].spelling,
                    element['cursor'].enum_value
                )
            else:
                slot = '"<unknown ::CXCursorKind value ({index})>", // {index}'.format(index=i)

            file.write("""
        {}""".format(slot))

        file.write("""
    }};

    return strings[static_cast<std::size_t>(kind) - {min}];
}}""".format(min=values['min_value']))


def generate_enum(values):
    with open('cursorkindenum.hpp', mode='w+') as file:
        file.write("""
/*
 * Generated from clang-c/Index.h
 * See inception.py at include/siplasplas/reflection/parser/api/core/clang/detail/
 */

/**
 * \\brief Describes the kind of entity that a cursor refers to
 */
enum class Kind
{""")
        for element in values['elements'].itervalues():
            slot = '{}\n    {} = ::{}, // ({}:{}: name=\'{}\', value={})'.format(
                element['doc'],
                element['name'],
                element['spelling'],
                element['cursor'].location.file,
                element['cursor'].location.line,
                element['cursor'].spelling,
                element['cursor'].enum_value
            )

            file.write("""

    {}""".format(slot))

        file.write("""
};""")


def parse(clang_includedir):
    index_h = os.path.join(clang_includedir, 'clang-c', 'Index.h')
    index = Index.create()
    tu = index.parse(index_h)

    def visitor(cursor):
        if cursor.kind == CursorKind.ENUM_DECL and cursor.spelling == "CXCursorKind":
            values = parse_cursorkind(cursor)
            generate_serialization(values)
            generate_enum(values)
        else:
            for c in cursor.get_children():
                visitor(c)

    visitor(tu.cursor)


def main():
    clang_includedir = sys.argv[1]

    print 'Clang includedir: {}'.format(clang_includedir)

    parse(clang_includedir)

if __name__ == '__main__':
    main()
