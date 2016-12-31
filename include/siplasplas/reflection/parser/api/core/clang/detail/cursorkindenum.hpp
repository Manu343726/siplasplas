
/*
 * Generated from clang-c/Index.h
 * See inception.py at include/siplasplas/reflection/parser/api/core/clang/detail/
 */

/**
 * \brief Describes the kind of entity that a cursor refers to
 */
enum class Kind
{

    /** \brief An access specifier. */
    FirstDecl = ::CXCursor_FirstDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1634: name='CXCursor_FirstDecl', value=1)

    /** \brief A C or C++ struct. */
    StructDecl = ::CXCursor_StructDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1555: name='CXCursor_StructDecl', value=2)

    /** \brief A C or C++ union. */
    UnionDecl = ::CXCursor_UnionDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1557: name='CXCursor_UnionDecl', value=3)

    /** \brief A C++ class. */
    ClassDecl = ::CXCursor_ClassDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1559: name='CXCursor_ClassDecl', value=4)

    /** \brief An enumeration. */
    EnumDecl = ::CXCursor_EnumDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1561: name='CXCursor_EnumDecl', value=5)

    /**
     * \brief A field (in C) or non-static data member (in C++) in a
     * struct, union, or C++ class.
     */
    FieldDecl = ::CXCursor_FieldDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1566: name='CXCursor_FieldDecl', value=6)

    /** \brief An enumerator constant. */
    EnumConstantDecl = ::CXCursor_EnumConstantDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1568: name='CXCursor_EnumConstantDecl', value=7)

    /** \brief A function. */
    FunctionDecl = ::CXCursor_FunctionDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1570: name='CXCursor_FunctionDecl', value=8)

    /** \brief A variable. */
    VarDecl = ::CXCursor_VarDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1572: name='CXCursor_VarDecl', value=9)

    /** \brief A function or method parameter. */
    ParmDecl = ::CXCursor_ParmDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1574: name='CXCursor_ParmDecl', value=10)

    /** \brief An Objective-C \@interface. */
    ObjCInterfaceDecl = ::CXCursor_ObjCInterfaceDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1576: name='CXCursor_ObjCInterfaceDecl', value=11)

    /** \brief An Objective-C \@interface for a category. */
    ObjCCategoryDecl = ::CXCursor_ObjCCategoryDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1578: name='CXCursor_ObjCCategoryDecl', value=12)

    /** \brief An Objective-C \@protocol declaration. */
    ObjCProtocolDecl = ::CXCursor_ObjCProtocolDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1580: name='CXCursor_ObjCProtocolDecl', value=13)

    /** \brief An Objective-C \@property declaration. */
    ObjCPropertyDecl = ::CXCursor_ObjCPropertyDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1582: name='CXCursor_ObjCPropertyDecl', value=14)

    /** \brief An Objective-C instance variable. */
    ObjCIvarDecl = ::CXCursor_ObjCIvarDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1584: name='CXCursor_ObjCIvarDecl', value=15)

    /** \brief An Objective-C instance method. */
    ObjCInstanceMethodDecl = ::CXCursor_ObjCInstanceMethodDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1586: name='CXCursor_ObjCInstanceMethodDecl', value=16)

    /** \brief An Objective-C class method. */
    ObjCClassMethodDecl = ::CXCursor_ObjCClassMethodDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1588: name='CXCursor_ObjCClassMethodDecl', value=17)

    /** \brief An Objective-C \@implementation. */
    ObjCImplementationDecl = ::CXCursor_ObjCImplementationDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1590: name='CXCursor_ObjCImplementationDecl', value=18)

    /** \brief An Objective-C \@implementation for a category. */
    ObjCCategoryImplDecl = ::CXCursor_ObjCCategoryImplDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1592: name='CXCursor_ObjCCategoryImplDecl', value=19)

    /** \brief A typedef. */
    TypedefDecl = ::CXCursor_TypedefDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1594: name='CXCursor_TypedefDecl', value=20)

    /** \brief A C++ class method. */
    CXXMethod = ::CXCursor_CXXMethod, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1596: name='CXCursor_CXXMethod', value=21)

    /** \brief A C++ namespace. */
    Namespace = ::CXCursor_Namespace, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1598: name='CXCursor_Namespace', value=22)

    /** \brief A linkage specification, e.g. 'extern "C"'. */
    LinkageSpec = ::CXCursor_LinkageSpec, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1600: name='CXCursor_LinkageSpec', value=23)

    /** \brief A C++ constructor. */
    Constructor = ::CXCursor_Constructor, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1602: name='CXCursor_Constructor', value=24)

    /** \brief A C++ destructor. */
    Destructor = ::CXCursor_Destructor, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1604: name='CXCursor_Destructor', value=25)

    /** \brief A C++ conversion function. */
    ConversionFunction = ::CXCursor_ConversionFunction, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1606: name='CXCursor_ConversionFunction', value=26)

    /** \brief A C++ template type parameter. */
    TemplateTypeParameter = ::CXCursor_TemplateTypeParameter, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1608: name='CXCursor_TemplateTypeParameter', value=27)

    /** \brief A C++ non-type template parameter. */
    NonTypeTemplateParameter = ::CXCursor_NonTypeTemplateParameter, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1610: name='CXCursor_NonTypeTemplateParameter', value=28)

    /** \brief A C++ template template parameter. */
    TemplateTemplateParameter = ::CXCursor_TemplateTemplateParameter, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1612: name='CXCursor_TemplateTemplateParameter', value=29)

    /** \brief A C++ function template. */
    FunctionTemplate = ::CXCursor_FunctionTemplate, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1614: name='CXCursor_FunctionTemplate', value=30)

    /** \brief A C++ class template. */
    ClassTemplate = ::CXCursor_ClassTemplate, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1616: name='CXCursor_ClassTemplate', value=31)

    /** \brief A C++ class template partial specialization. */
    ClassTemplatePartialSpecialization = ::CXCursor_ClassTemplatePartialSpecialization, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1618: name='CXCursor_ClassTemplatePartialSpecialization', value=32)

    /** \brief A C++ namespace alias declaration. */
    NamespaceAlias = ::CXCursor_NamespaceAlias, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1620: name='CXCursor_NamespaceAlias', value=33)

    /** \brief A C++ using directive. */
    UsingDirective = ::CXCursor_UsingDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1622: name='CXCursor_UsingDirective', value=34)

    /** \brief A C++ using declaration. */
    UsingDeclaration = ::CXCursor_UsingDeclaration, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1624: name='CXCursor_UsingDeclaration', value=35)

    /** \brief A C++ alias declaration */
    TypeAliasDecl = ::CXCursor_TypeAliasDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1626: name='CXCursor_TypeAliasDecl', value=36)

    /** \brief An Objective-C \@synthesize definition. */
    ObjCSynthesizeDecl = ::CXCursor_ObjCSynthesizeDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1628: name='CXCursor_ObjCSynthesizeDecl', value=37)

    /** \brief An Objective-C \@dynamic definition. */
    ObjCDynamicDecl = ::CXCursor_ObjCDynamicDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1630: name='CXCursor_ObjCDynamicDecl', value=38)

    /** \brief An access specifier. */
    LastDecl = ::CXCursor_LastDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1635: name='CXCursor_LastDecl', value=39)

    /** \brief An access specifier. */
    ObjCSuperClassRef = ::CXCursor_ObjCSuperClassRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1639: name='CXCursor_ObjCSuperClassRef', value=40)

    /** \brief An access specifier. */
    ObjCProtocolRef = ::CXCursor_ObjCProtocolRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1640: name='CXCursor_ObjCProtocolRef', value=41)

    /** \brief An access specifier. */
    ObjCClassRef = ::CXCursor_ObjCClassRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1641: name='CXCursor_ObjCClassRef', value=42)

    /**
     * \brief A reference to a type declaration.
     *
     * A type reference occurs anywhere where a type is named but not
     * declared. For example, given:
     *
     * \code
     * typedef unsigned size_type;
     * size_type size;
     * \endcode
     *
     * The typedef is a declaration of size_type (CXCursor_TypedefDecl),
     * while the type of the variable "size" is referenced. The cursor
     * referenced by the type of size is the typedef for size_type.
     */
    TypeRef = ::CXCursor_TypeRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1657: name='CXCursor_TypeRef', value=43)

    /**
     * \brief A reference to a type declaration.
     *
     * A type reference occurs anywhere where a type is named but not
     * declared. For example, given:
     *
     * \code
     * typedef unsigned size_type;
     * size_type size;
     * \endcode
     *
     * The typedef is a declaration of size_type (CXCursor_TypedefDecl),
     * while the type of the variable "size" is referenced. The cursor
     * referenced by the type of size is the typedef for size_type.
     */
    CXXBaseSpecifier = ::CXCursor_CXXBaseSpecifier, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1658: name='CXCursor_CXXBaseSpecifier', value=44)

    /**
     * \brief A reference to a class template, function template, template
     * template parameter, or class template partial specialization.
     */
    TemplateRef = ::CXCursor_TemplateRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1663: name='CXCursor_TemplateRef', value=45)

    /**
     * \brief A reference to a namespace or namespace alias.
     */
    NamespaceRef = ::CXCursor_NamespaceRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1667: name='CXCursor_NamespaceRef', value=46)

    /**
     * \brief A reference to a member of a struct, union, or class that occurs in
     * some non-expression context, e.g., a designated initializer.
     */
    MemberRef = ::CXCursor_MemberRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1672: name='CXCursor_MemberRef', value=47)

    /**
     * \brief A reference to a labeled statement.
     *
     * This cursor kind is used to describe the jump to "start_over" in the
     * goto statement in the following example:
     *
     * \code
     *   start_over:
     *     ++counter;
     *
     *     goto start_over;
     * \endcode
     *
     * A label reference cursor refers to a label statement.
     */
    LabelRef = ::CXCursor_LabelRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1688: name='CXCursor_LabelRef', value=48)

    /**
     * \brief A reference to a set of overloaded functions or function templates
     * that has not yet been resolved to a specific function or function template.
     *
     * An overloaded declaration reference cursor occurs in C++ templates where
     * a dependent name refers to a function. For example:
     *
     * \code
     * template<typename T> void swap(T&, T&);
     *
     * struct X { ... };
     * void swap(X&, X&);
     *
     * template<typename T>
     * void reverse(T* first, T* last) {
     *   while (first < last - 1) {
     *     swap(*first, *--last);
     *     ++first;
     *   }
     * }
     *
     * struct Y { };
     * void swap(Y&, Y&);
     * \endcode
     *
     * Here, the identifier "swap" is associated with an overloaded declaration
     * reference. In the template definition, "swap" refers to either of the two
     * "swap" functions declared above, so both results will be available. At
     * instantiation time, "swap" may also refer to other functions found via
     * argument-dependent lookup (e.g., the "swap" function at the end of the
     * example).
     *
     * The functions \c clang_getNumOverloadedDecls() and
     * \c clang_getOverloadedDecl() can be used to retrieve the definitions
     * referenced by this cursor.
     */
    OverloadedDeclRef = ::CXCursor_OverloadedDeclRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1726: name='CXCursor_OverloadedDeclRef', value=49)

    /**
     * \brief A reference to a variable that occurs in some non-expression
     * context, e.g., a C++ lambda capture list.
     */
    LastRef = ::CXCursor_LastRef, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1734: name='CXCursor_LastRef', value=50)

    /**
     * \brief A reference to a variable that occurs in some non-expression
     * context, e.g., a C++ lambda capture list.
     */
    InvalidFile = ::CXCursor_InvalidFile, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1738: name='CXCursor_InvalidFile', value=70)

    /**
     * \brief A reference to a variable that occurs in some non-expression
     * context, e.g., a C++ lambda capture list.
     */
    NoDeclFound = ::CXCursor_NoDeclFound, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1739: name='CXCursor_NoDeclFound', value=71)

    /**
     * \brief A reference to a variable that occurs in some non-expression
     * context, e.g., a C++ lambda capture list.
     */
    NotImplemented = ::CXCursor_NotImplemented, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1740: name='CXCursor_NotImplemented', value=72)

    /**
     * \brief A reference to a variable that occurs in some non-expression
     * context, e.g., a C++ lambda capture list.
     */
    LastInvalid = ::CXCursor_LastInvalid, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1742: name='CXCursor_LastInvalid', value=73)

    /**
     * \brief A module import declaration.
     */
    FirstExtraDecl = ::CXCursor_FirstExtraDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2331: name='CXCursor_FirstExtraDecl', value=600)

    /**
     * \brief A module import declaration.
     */
    LastExtraDecl = ::CXCursor_LastExtraDecl, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2332: name='CXCursor_LastExtraDecl', value=601)

    /**
     * \brief An expression whose specific kind is not exposed via this
     * interface.
     *
     * Unexposed expressions have the same operations as any other kind
     * of expression; one can extract their location information,
     * spelling, children, etc. However, the specific kind of the
     * expression is not reported.
     */
    UnexposedExpr = ::CXCursor_UnexposedExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1756: name='CXCursor_UnexposedExpr', value=100)

    /**
     * \brief An expression that refers to some value declaration, such
     * as a function, variable, or enumerator.
     */
    DeclRefExpr = ::CXCursor_DeclRefExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1762: name='CXCursor_DeclRefExpr', value=101)

    /**
     * \brief An expression that refers to a member of a struct, union,
     * class, Objective-C class, etc.
     */
    MemberRefExpr = ::CXCursor_MemberRefExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1768: name='CXCursor_MemberRefExpr', value=102)

    /** \brief An expression that calls a function. */
    CallExpr = ::CXCursor_CallExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1771: name='CXCursor_CallExpr', value=103)

    /** \brief An expression that sends a message to an Objective-C
     object or class. */
    ObjCMessageExpr = ::CXCursor_ObjCMessageExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1775: name='CXCursor_ObjCMessageExpr', value=104)

    /** \brief An expression that represents a block literal. */
    BlockExpr = ::CXCursor_BlockExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1778: name='CXCursor_BlockExpr', value=105)

    /** \brief An integer literal.
     */
    IntegerLiteral = ::CXCursor_IntegerLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1782: name='CXCursor_IntegerLiteral', value=106)

    /** \brief A floating point number literal.
     */
    FloatingLiteral = ::CXCursor_FloatingLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1786: name='CXCursor_FloatingLiteral', value=107)

    /** \brief An imaginary number literal.
     */
    ImaginaryLiteral = ::CXCursor_ImaginaryLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1790: name='CXCursor_ImaginaryLiteral', value=108)

    /** \brief A string literal.
     */
    StringLiteral = ::CXCursor_StringLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1794: name='CXCursor_StringLiteral', value=109)

    /** \brief A character literal.
     */
    CharacterLiteral = ::CXCursor_CharacterLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1798: name='CXCursor_CharacterLiteral', value=110)

    /** \brief A parenthesized expression, e.g. "(1)".
     *
     * This AST node is only formed if full location information is requested.
     */
    ParenExpr = ::CXCursor_ParenExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1804: name='CXCursor_ParenExpr', value=111)

    /** \brief This represents the unary-expression's (except sizeof and
     * alignof).
     */
    UnaryOperator = ::CXCursor_UnaryOperator, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1809: name='CXCursor_UnaryOperator', value=112)

    /** \brief [C99 6.5.2.1] Array Subscripting.
     */
    ArraySubscriptExpr = ::CXCursor_ArraySubscriptExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1813: name='CXCursor_ArraySubscriptExpr', value=113)

    /** \brief A builtin binary operation expression such as "x + y" or
     * "x <= y".
     */
    BinaryOperator = ::CXCursor_BinaryOperator, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1818: name='CXCursor_BinaryOperator', value=114)

    /** \brief Compound assignment such as "+=".
     */
    CompoundAssignOperator = ::CXCursor_CompoundAssignOperator, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1822: name='CXCursor_CompoundAssignOperator', value=115)

    /** \brief The ?: ternary operator.
     */
    ConditionalOperator = ::CXCursor_ConditionalOperator, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1826: name='CXCursor_ConditionalOperator', value=116)

    /** \brief An explicit cast in C (C99 6.5.4) or a C-style cast in C++
     * (C++ [expr.cast]), which uses the syntax (Type)expr.
     *
     * For example: (int)f.
     */
    CStyleCastExpr = ::CXCursor_CStyleCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1833: name='CXCursor_CStyleCastExpr', value=117)

    /** \brief [C99 6.5.2.5]
     */
    CompoundLiteralExpr = ::CXCursor_CompoundLiteralExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1837: name='CXCursor_CompoundLiteralExpr', value=118)

    /** \brief Describes an C or C++ initializer list.
     */
    InitListExpr = ::CXCursor_InitListExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1841: name='CXCursor_InitListExpr', value=119)

    /** \brief The GNU address of label extension, representing &&label.
     */
    AddrLabelExpr = ::CXCursor_AddrLabelExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1845: name='CXCursor_AddrLabelExpr', value=120)

    /** \brief This is the GNU Statement Expression extension: ({int X=4; X;})
     */
    StmtExpr = ::CXCursor_StmtExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1849: name='CXCursor_StmtExpr', value=121)

    /** \brief Represents a C11 generic selection.
     */
    GenericSelectionExpr = ::CXCursor_GenericSelectionExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1853: name='CXCursor_GenericSelectionExpr', value=122)

    /** \brief Implements the GNU __null extension, which is a name for a null
     * pointer constant that has integral type (e.g., int or long) and is the same
     * size and alignment as a pointer.
     *
     * The __null extension is typically only used by system headers, which define
     * NULL as __null in C++ rather than using 0 (which is an integer that may not
     * match the size of a pointer).
     */
    GNUNullExpr = ::CXCursor_GNUNullExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1863: name='CXCursor_GNUNullExpr', value=123)

    /** \brief C++'s static_cast<> expression.
     */
    CXXStaticCastExpr = ::CXCursor_CXXStaticCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1867: name='CXCursor_CXXStaticCastExpr', value=124)

    /** \brief C++'s dynamic_cast<> expression.
     */
    CXXDynamicCastExpr = ::CXCursor_CXXDynamicCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1871: name='CXCursor_CXXDynamicCastExpr', value=125)

    /** \brief C++'s reinterpret_cast<> expression.
     */
    CXXReinterpretCastExpr = ::CXCursor_CXXReinterpretCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1875: name='CXCursor_CXXReinterpretCastExpr', value=126)

    /** \brief C++'s const_cast<> expression.
     */
    CXXConstCastExpr = ::CXCursor_CXXConstCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1879: name='CXCursor_CXXConstCastExpr', value=127)

    /** \brief Represents an explicit C++ type conversion that uses "functional"
     * notion (C++ [expr.type.conv]).
     *
     * Example:
     * \code
     *   x = int(0.5);
     * \endcode
     */
    CXXFunctionalCastExpr = ::CXCursor_CXXFunctionalCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1889: name='CXCursor_CXXFunctionalCastExpr', value=128)

    /** \brief A C++ typeid expression (C++ [expr.typeid]).
     */
    CXXTypeidExpr = ::CXCursor_CXXTypeidExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1893: name='CXCursor_CXXTypeidExpr', value=129)

    /** \brief [C++ 2.13.5] C++ Boolean Literal.
     */
    CXXBoolLiteralExpr = ::CXCursor_CXXBoolLiteralExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1897: name='CXCursor_CXXBoolLiteralExpr', value=130)

    /** \brief [C++0x 2.14.7] C++ Pointer Literal.
     */
    CXXNullPtrLiteralExpr = ::CXCursor_CXXNullPtrLiteralExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1901: name='CXCursor_CXXNullPtrLiteralExpr', value=131)

    /** \brief Represents the "this" expression in C++
     */
    CXXThisExpr = ::CXCursor_CXXThisExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1905: name='CXCursor_CXXThisExpr', value=132)

    /** \brief [C++ 15] C++ Throw Expression.
     *
     * This handles 'throw' and 'throw' assignment-expression. When
     * assignment-expression isn't present, Op will be null.
     */
    CXXThrowExpr = ::CXCursor_CXXThrowExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1912: name='CXCursor_CXXThrowExpr', value=133)

    /** \brief A new expression for memory allocation and constructor calls, e.g:
     * "new CXXNewExpr(foo)".
     */
    CXXNewExpr = ::CXCursor_CXXNewExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1917: name='CXCursor_CXXNewExpr', value=134)

    /** \brief A delete expression for memory deallocation and destructor calls,
     * e.g. "delete[] pArray".
     */
    CXXDeleteExpr = ::CXCursor_CXXDeleteExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1922: name='CXCursor_CXXDeleteExpr', value=135)

    /** \brief A unary expression.
     */
    UnaryExpr = ::CXCursor_UnaryExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1926: name='CXCursor_UnaryExpr', value=136)

    /** \brief An Objective-C string literal i.e. @"foo".
     */
    ObjCStringLiteral = ::CXCursor_ObjCStringLiteral, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1930: name='CXCursor_ObjCStringLiteral', value=137)

    /** \brief An Objective-C \@encode expression.
     */
    ObjCEncodeExpr = ::CXCursor_ObjCEncodeExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1934: name='CXCursor_ObjCEncodeExpr', value=138)

    /** \brief An Objective-C \@selector expression.
     */
    ObjCSelectorExpr = ::CXCursor_ObjCSelectorExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1938: name='CXCursor_ObjCSelectorExpr', value=139)

    /** \brief An Objective-C \@protocol expression.
     */
    ObjCProtocolExpr = ::CXCursor_ObjCProtocolExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1942: name='CXCursor_ObjCProtocolExpr', value=140)

    /** \brief An Objective-C "bridged" cast expression, which casts between
     * Objective-C pointers and C pointers, transferring ownership in the process.
     *
     * \code
     *   NSString *str = (__bridge_transfer NSString *)CFCreateString();
     * \endcode
     */
    ObjCBridgedCastExpr = ::CXCursor_ObjCBridgedCastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1951: name='CXCursor_ObjCBridgedCastExpr', value=141)

    /** \brief Represents a C++0x pack expansion that produces a sequence of
     * expressions.
     *
     * A pack expansion expression contains a pattern (which itself is an
     * expression) followed by an ellipsis. For example:
     *
     * \code
     * template<typename F, typename ...Types>
     * void forward(F f, Types &&...args) {
     *  f(static_cast<Types&&>(args)...);
     * }
     * \endcode
     */
    PackExpansionExpr = ::CXCursor_PackExpansionExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1966: name='CXCursor_PackExpansionExpr', value=142)

    /** \brief Represents an expression that computes the length of a parameter
     * pack.
     *
     * \code
     * template<typename ...Types>
     * struct count {
     *   static const unsigned value = sizeof...(Types);
     * };
     * \endcode
     */
    SizeOfPackExpr = ::CXCursor_SizeOfPackExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1978: name='CXCursor_SizeOfPackExpr', value=143)

    // No documentation available
    LambdaExpr = ::CXCursor_LambdaExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1992: name='CXCursor_LambdaExpr', value=144)

    /** \brief Objective-c Boolean Literal.
     */
    ObjCBoolLiteralExpr = ::CXCursor_ObjCBoolLiteralExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:1996: name='CXCursor_ObjCBoolLiteralExpr', value=145)

    /** \brief Represents the "self" expression in an Objective-C method.
     */
    ObjCSelfExpr = ::CXCursor_ObjCSelfExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2000: name='CXCursor_ObjCSelfExpr', value=146)

    /** \brief OpenMP 4.0 [2.4, Array Section].
     */
    LastExpr = ::CXCursor_LastExpr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2006: name='CXCursor_LastExpr', value=147)

    /**
     * \brief A code completion overload candidate.
     */
    OverloadCandidate = ::CXCursor_OverloadCandidate, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2337: name='CXCursor_OverloadCandidate', value=700)

    /**
     * \brief A statement whose specific kind is not exposed via this
     * interface.
     *
     * Unexposed statements have the same operations as any other kind of
     * statement; one can extract their location information, spelling,
     * children, etc. However, the specific kind of the statement is not
     * reported.
     */
    UnexposedStmt = ::CXCursor_UnexposedStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2019: name='CXCursor_UnexposedStmt', value=200)

    /** \brief A labelled statement in a function.
     *
     * This cursor kind is used to describe the "start_over:" label statement in
     * the following example:
     *
     * \code
     *   start_over:
     *     ++counter;
     * \endcode
     *
     */
    LabelStmt = ::CXCursor_LabelStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2032: name='CXCursor_LabelStmt', value=201)

    /** \brief A group of statements like { stmt stmt }.
     *
     * This cursor kind is used to describe compound statements, e.g. function
     * bodies.
     */
    CompoundStmt = ::CXCursor_CompoundStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2039: name='CXCursor_CompoundStmt', value=202)

    /** \brief A case statement.
     */
    CaseStmt = ::CXCursor_CaseStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2043: name='CXCursor_CaseStmt', value=203)

    /** \brief A default statement.
     */
    DefaultStmt = ::CXCursor_DefaultStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2047: name='CXCursor_DefaultStmt', value=204)

    /** \brief An if statement
     */
    IfStmt = ::CXCursor_IfStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2051: name='CXCursor_IfStmt', value=205)

    /** \brief A switch statement.
     */
    SwitchStmt = ::CXCursor_SwitchStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2055: name='CXCursor_SwitchStmt', value=206)

    /** \brief A while statement.
     */
    WhileStmt = ::CXCursor_WhileStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2059: name='CXCursor_WhileStmt', value=207)

    /** \brief A do statement.
     */
    DoStmt = ::CXCursor_DoStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2063: name='CXCursor_DoStmt', value=208)

    /** \brief A for statement.
     */
    ForStmt = ::CXCursor_ForStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2067: name='CXCursor_ForStmt', value=209)

    /** \brief A goto statement.
     */
    GotoStmt = ::CXCursor_GotoStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2071: name='CXCursor_GotoStmt', value=210)

    /** \brief An indirect goto statement.
     */
    IndirectGotoStmt = ::CXCursor_IndirectGotoStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2075: name='CXCursor_IndirectGotoStmt', value=211)

    /** \brief A continue statement.
     */
    ContinueStmt = ::CXCursor_ContinueStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2079: name='CXCursor_ContinueStmt', value=212)

    /** \brief A break statement.
     */
    BreakStmt = ::CXCursor_BreakStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2083: name='CXCursor_BreakStmt', value=213)

    /** \brief A return statement.
     */
    ReturnStmt = ::CXCursor_ReturnStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2087: name='CXCursor_ReturnStmt', value=214)

    /** \brief A GCC inline assembly statement extension.
     */
    AsmStmt = ::CXCursor_AsmStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2092: name='CXCursor_AsmStmt', value=215)

    /** \brief Objective-C's overall \@try-\@catch-\@finally statement.
     */
    ObjCAtTryStmt = ::CXCursor_ObjCAtTryStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2096: name='CXCursor_ObjCAtTryStmt', value=216)

    /** \brief Objective-C's \@catch statement.
     */
    ObjCAtCatchStmt = ::CXCursor_ObjCAtCatchStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2100: name='CXCursor_ObjCAtCatchStmt', value=217)

    /** \brief Objective-C's \@finally statement.
     */
    ObjCAtFinallyStmt = ::CXCursor_ObjCAtFinallyStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2104: name='CXCursor_ObjCAtFinallyStmt', value=218)

    /** \brief Objective-C's \@throw statement.
     */
    ObjCAtThrowStmt = ::CXCursor_ObjCAtThrowStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2108: name='CXCursor_ObjCAtThrowStmt', value=219)

    /** \brief Objective-C's \@synchronized statement.
     */
    ObjCAtSynchronizedStmt = ::CXCursor_ObjCAtSynchronizedStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2112: name='CXCursor_ObjCAtSynchronizedStmt', value=220)

    /** \brief Objective-C's autorelease pool statement.
     */
    ObjCAutoreleasePoolStmt = ::CXCursor_ObjCAutoreleasePoolStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2116: name='CXCursor_ObjCAutoreleasePoolStmt', value=221)

    /** \brief Objective-C's collection statement.
     */
    ObjCForCollectionStmt = ::CXCursor_ObjCForCollectionStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2120: name='CXCursor_ObjCForCollectionStmt', value=222)

    /** \brief C++'s catch statement.
     */
    CXXCatchStmt = ::CXCursor_CXXCatchStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2124: name='CXCursor_CXXCatchStmt', value=223)

    /** \brief C++'s try statement.
     */
    CXXTryStmt = ::CXCursor_CXXTryStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2128: name='CXCursor_CXXTryStmt', value=224)

    /** \brief C++'s for (* : *) statement.
     */
    CXXForRangeStmt = ::CXCursor_CXXForRangeStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2132: name='CXCursor_CXXForRangeStmt', value=225)

    /** \brief Windows Structured Exception Handling's try statement.
     */
    SEHTryStmt = ::CXCursor_SEHTryStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2136: name='CXCursor_SEHTryStmt', value=226)

    /** \brief Windows Structured Exception Handling's except statement.
     */
    SEHExceptStmt = ::CXCursor_SEHExceptStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2140: name='CXCursor_SEHExceptStmt', value=227)

    /** \brief Windows Structured Exception Handling's finally statement.
     */
    SEHFinallyStmt = ::CXCursor_SEHFinallyStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2144: name='CXCursor_SEHFinallyStmt', value=228)

    /** \brief A MS inline assembly statement extension.
     */
    MSAsmStmt = ::CXCursor_MSAsmStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2148: name='CXCursor_MSAsmStmt', value=229)

    /** \brief The null statement ";": C99 6.8.3p3.
     *
     * This cursor kind is used to describe the null statement.
     */
    NullStmt = ::CXCursor_NullStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2154: name='CXCursor_NullStmt', value=230)

    /** \brief Adaptor class for mixing declarations with statements and
     * expressions.
     */
    DeclStmt = ::CXCursor_DeclStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2159: name='CXCursor_DeclStmt', value=231)

    /** \brief OpenMP parallel directive.
     */
    OMPParallelDirective = ::CXCursor_OMPParallelDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2163: name='CXCursor_OMPParallelDirective', value=232)

    /** \brief OpenMP SIMD directive.
     */
    OMPSimdDirective = ::CXCursor_OMPSimdDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2167: name='CXCursor_OMPSimdDirective', value=233)

    /** \brief OpenMP for directive.
     */
    OMPForDirective = ::CXCursor_OMPForDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2171: name='CXCursor_OMPForDirective', value=234)

    /** \brief OpenMP sections directive.
     */
    OMPSectionsDirective = ::CXCursor_OMPSectionsDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2175: name='CXCursor_OMPSectionsDirective', value=235)

    /** \brief OpenMP section directive.
     */
    OMPSectionDirective = ::CXCursor_OMPSectionDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2179: name='CXCursor_OMPSectionDirective', value=236)

    /** \brief OpenMP single directive.
     */
    OMPSingleDirective = ::CXCursor_OMPSingleDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2183: name='CXCursor_OMPSingleDirective', value=237)

    /** \brief OpenMP parallel for directive.
     */
    OMPParallelForDirective = ::CXCursor_OMPParallelForDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2187: name='CXCursor_OMPParallelForDirective', value=238)

    /** \brief OpenMP parallel sections directive.
     */
    OMPParallelSectionsDirective = ::CXCursor_OMPParallelSectionsDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2191: name='CXCursor_OMPParallelSectionsDirective', value=239)

    /** \brief OpenMP task directive.
     */
    OMPTaskDirective = ::CXCursor_OMPTaskDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2195: name='CXCursor_OMPTaskDirective', value=240)

    /** \brief OpenMP master directive.
     */
    OMPMasterDirective = ::CXCursor_OMPMasterDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2199: name='CXCursor_OMPMasterDirective', value=241)

    /** \brief OpenMP critical directive.
     */
    OMPCriticalDirective = ::CXCursor_OMPCriticalDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2203: name='CXCursor_OMPCriticalDirective', value=242)

    /** \brief OpenMP taskyield directive.
     */
    OMPTaskyieldDirective = ::CXCursor_OMPTaskyieldDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2207: name='CXCursor_OMPTaskyieldDirective', value=243)

    /** \brief OpenMP barrier directive.
     */
    OMPBarrierDirective = ::CXCursor_OMPBarrierDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2211: name='CXCursor_OMPBarrierDirective', value=244)

    /** \brief OpenMP taskwait directive.
     */
    OMPTaskwaitDirective = ::CXCursor_OMPTaskwaitDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2215: name='CXCursor_OMPTaskwaitDirective', value=245)

    /** \brief OpenMP flush directive.
     */
    OMPFlushDirective = ::CXCursor_OMPFlushDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2219: name='CXCursor_OMPFlushDirective', value=246)

    /** \brief Windows Structured Exception Handling's leave statement.
     */
    SEHLeaveStmt = ::CXCursor_SEHLeaveStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2223: name='CXCursor_SEHLeaveStmt', value=247)

    /** \brief OpenMP ordered directive.
     */
    OMPOrderedDirective = ::CXCursor_OMPOrderedDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2227: name='CXCursor_OMPOrderedDirective', value=248)

    /** \brief OpenMP atomic directive.
     */
    OMPAtomicDirective = ::CXCursor_OMPAtomicDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2231: name='CXCursor_OMPAtomicDirective', value=249)

    /** \brief OpenMP for SIMD directive.
     */
    OMPForSimdDirective = ::CXCursor_OMPForSimdDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2235: name='CXCursor_OMPForSimdDirective', value=250)

    /** \brief OpenMP parallel for SIMD directive.
     */
    OMPParallelForSimdDirective = ::CXCursor_OMPParallelForSimdDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2239: name='CXCursor_OMPParallelForSimdDirective', value=251)

    /** \brief OpenMP target directive.
     */
    OMPTargetDirective = ::CXCursor_OMPTargetDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2243: name='CXCursor_OMPTargetDirective', value=252)

    /** \brief OpenMP teams directive.
     */
    OMPTeamsDirective = ::CXCursor_OMPTeamsDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2247: name='CXCursor_OMPTeamsDirective', value=253)

    /** \brief OpenMP taskgroup directive.
     */
    OMPTaskgroupDirective = ::CXCursor_OMPTaskgroupDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2251: name='CXCursor_OMPTaskgroupDirective', value=254)

    /** \brief OpenMP cancellation point directive.
     */
    OMPCancellationPointDirective = ::CXCursor_OMPCancellationPointDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2255: name='CXCursor_OMPCancellationPointDirective', value=255)

    /** \brief OpenMP cancel directive.
     */
    OMPCancelDirective = ::CXCursor_OMPCancelDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2259: name='CXCursor_OMPCancelDirective', value=256)

    /** \brief OpenMP target data directive.
     */
    OMPTargetDataDirective = ::CXCursor_OMPTargetDataDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2263: name='CXCursor_OMPTargetDataDirective', value=257)

    /** \brief OpenMP taskloop directive.
     */
    OMPTaskLoopDirective = ::CXCursor_OMPTaskLoopDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2267: name='CXCursor_OMPTaskLoopDirective', value=258)

    /** \brief OpenMP taskloop simd directive.
     */
    OMPTaskLoopSimdDirective = ::CXCursor_OMPTaskLoopSimdDirective, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2271: name='CXCursor_OMPTaskLoopSimdDirective', value=259)

    /** \brief OpenMP distribute directive.
     */
    LastStmt = ::CXCursor_LastStmt, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2277: name='CXCursor_LastStmt', value=260)

    /**
     * \brief Cursor that represents the translation unit itself.
     *
     * The translation unit cursor exists primarily to act as the root
     * cursor for traversing the contents of a translation unit.
     */
    TranslationUnit = ::CXCursor_TranslationUnit, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2285: name='CXCursor_TranslationUnit', value=300)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    UnexposedAttr = ::CXCursor_UnexposedAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2293: name='CXCursor_UnexposedAttr', value=400)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    IBActionAttr = ::CXCursor_IBActionAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2295: name='CXCursor_IBActionAttr', value=401)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    IBOutletAttr = ::CXCursor_IBOutletAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2296: name='CXCursor_IBOutletAttr', value=402)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    IBOutletCollectionAttr = ::CXCursor_IBOutletCollectionAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2297: name='CXCursor_IBOutletCollectionAttr', value=403)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CXXFinalAttr = ::CXCursor_CXXFinalAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2298: name='CXCursor_CXXFinalAttr', value=404)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CXXOverrideAttr = ::CXCursor_CXXOverrideAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2299: name='CXCursor_CXXOverrideAttr', value=405)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    AnnotateAttr = ::CXCursor_AnnotateAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2300: name='CXCursor_AnnotateAttr', value=406)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    AsmLabelAttr = ::CXCursor_AsmLabelAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2301: name='CXCursor_AsmLabelAttr', value=407)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    PackedAttr = ::CXCursor_PackedAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2302: name='CXCursor_PackedAttr', value=408)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    PureAttr = ::CXCursor_PureAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2303: name='CXCursor_PureAttr', value=409)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    ConstAttr = ::CXCursor_ConstAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2304: name='CXCursor_ConstAttr', value=410)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    NoDuplicateAttr = ::CXCursor_NoDuplicateAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2305: name='CXCursor_NoDuplicateAttr', value=411)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CUDAConstantAttr = ::CXCursor_CUDAConstantAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2306: name='CXCursor_CUDAConstantAttr', value=412)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CUDADeviceAttr = ::CXCursor_CUDADeviceAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2307: name='CXCursor_CUDADeviceAttr', value=413)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CUDAGlobalAttr = ::CXCursor_CUDAGlobalAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2308: name='CXCursor_CUDAGlobalAttr', value=414)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CUDAHostAttr = ::CXCursor_CUDAHostAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2309: name='CXCursor_CUDAHostAttr', value=415)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    CUDASharedAttr = ::CXCursor_CUDASharedAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2310: name='CXCursor_CUDASharedAttr', value=416)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    VisibilityAttr = ::CXCursor_VisibilityAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2311: name='CXCursor_VisibilityAttr', value=417)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    DLLExport = ::CXCursor_DLLExport, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2312: name='CXCursor_DLLExport', value=418)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    LastAttr = ::CXCursor_LastAttr, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2314: name='CXCursor_LastAttr', value=419)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    FirstPreprocessing = ::CXCursor_FirstPreprocessing, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2322: name='CXCursor_FirstPreprocessing', value=500)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    MacroDefinition = ::CXCursor_MacroDefinition, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2318: name='CXCursor_MacroDefinition', value=501)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    MacroInstantiation = ::CXCursor_MacroInstantiation, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2320: name='CXCursor_MacroInstantiation', value=502)

    /**
     * \brief An attribute whose specific kind is not exposed via this
     * interface.
     */
    LastPreprocessing = ::CXCursor_LastPreprocessing, // (/home/manu343726/.conan/data/clang/3.8.0/smspillaz/stable/package/64e4f631b5b6e48c61788344ac830b9abb1e9726/include/clang-c/Index.h:2323: name='CXCursor_LastPreprocessing', value=503)
};