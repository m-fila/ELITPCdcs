find_package(open62541 REQUIRED COMPONENTS FullNamespace)

if(NOT UA_TOOLS_DIR)
    set(UA_TOOLS_DIR ${open62541_TOOLS_DIR})
endif()

if(NOT UA_NODESET_DIR)
    set(UA_NODESET_DIR ${open62541_NODESET_DIR})
endif()

set(FILE_CSV_DIRPREFIX ${CMAKE_SOURCE_DIR}/nodes)
set(FILE_BSD_DIRPREFIX ${CMAKE_SOURCE_DIR}/nodes)
set(FILE_NS_DIRPREFIX ${CMAKE_SOURCE_DIR}/nodes)

    ua_generate_nodeset_and_datatypes(
        NAME "dcsnodeset"
        FILE_CSV "${FILE_CSV_DIRPREFIX}/nodeset.csv"
        FILE_BSD "${FILE_BSD_DIRPREFIX}/nodeset.bsd"
        NAMESPACE_IDX 2
        FILE_NS "${FILE_NS_DIRPREFIX}/nodeset.xml"
	 	INTERNAL
    )
add_library(dcsNodes ${UA_NODESET_DCSNODESET_SOURCES} ${UA_TYPES_DCSNODESET_SOURCES})
target_link_libraries(dcsNodes open62541::open62541)
		