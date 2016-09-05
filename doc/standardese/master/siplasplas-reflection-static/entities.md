---
layout: standardese-doc
---

  - `cpp::static_reflection`
    
      - [Class\<T\>](class.md#cpp::static_reflection::Class\<T\> "cpp::static_reflection::Class\<T\>") - This template returns (inherits) a cpp::static\_reflection::meta::Class instance with the information of the given class type. If there's no static reflection information of this class in the current translation unit, returns an empty cpp::static\_reflection::meta::Class instance (Default "unknown" source info, empty methods, ctors, enums, and classes lists, etc). The behavior is undefined if the given type is not a class type.
    
      - [Kind](sourceinfo.md#cpp::static_reflection::Kind "cpp::static_reflection::Kind") - See cpp::static\_reflection::meta::SourceInfo.

  - `cpp::static_reflection::meta`
    
      - [SourceInfo\<Kind, FullName, Spelling, DisplayName, File, Line\>](sourceinfo.md#cpp::static_reflection::meta::SourceInfo\<Kind,%20FullName,%20Spelling,%20DisplayName,%20File,%20Line\> "cpp::static_reflection::meta::SourceInfo\<Kind, FullName, Spelling, DisplayName, File, Line\>") - SourceInfo class stores information about the declaration of an entity (class, function, etc) in the context of the sourcecode where the entity is declared. This information includes the file where the entity is declared, the line, the name of the entity, etc.
