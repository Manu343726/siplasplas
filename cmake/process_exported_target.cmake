# This script is invoked from a POST_LINK command as follows:
#
#     cmake -DTARGET=<target> -DLOCATION="<path to target runtime location>"
#
# the <target>.json.in file is generated at configure time (See export_target.cmake)
# with a placeholder entry for the target runtime location (Which is only known at 
# build time) The following configure_file() invocation processes that file and setting
# the value of @@<target>_LOCATION@@ variable to LOCATION.

# Write the variable being used by the .in file
set(${TARGET}_LOCATION "${LOCATION}")

configure_file(
	${INPUT_DIR}/${TARGET}.json.in 
	${OUTPUT_DIR}/${TARGET}.json	
)