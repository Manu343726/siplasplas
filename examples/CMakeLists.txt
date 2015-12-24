include(cmake/generate_assembly)

exec_target(NAME variant PREFIX example RUN)
exec_target(NAME gamelike_variant PREFIX example RUN)
exec_target(NAME messaging PREFIX example RUN)
exec_target(NAME multi_visitor PREFIX example RUN)
exec_target(NAME metatype PREFIX example RUN)
exec_target(NAME linear_allocator PREFIX example RUN)
