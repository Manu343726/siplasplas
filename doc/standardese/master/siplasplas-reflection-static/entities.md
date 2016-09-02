---
layout: standardese-doc
---

  - `cpp::static_reflection`
    
      - [Class\<T\>](class.md#cpp::static_reflection::Class\<T\> "cpp::static_reflection::Class\<T\>") - This template returns (inherits) a cpp::static\_reflection::meta::Class instance with the information of the given class type. If there's no static reflection information of this class in the current translation unit, returns an empty cpp::static\_reflection::meta::Class instance (Default "unknown" source info, empty methods, ctors, enums, and classes lists, etc). The behavior is undefined if the given type is not a class type.
