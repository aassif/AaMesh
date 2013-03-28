# Include.
find_path(AAMESH_INCLUDE_DIR
    NAMES
	AaMesh
    AaMeshExport
    AaMeshGts
    AaMeshNOFF
    AaMeshOptimizer
    AaMeshPolygonizer
    AaMeshPrimitives
    AaMeshRenderer
    AaMeshSeparator
    AaMeshVBO
	bits/AaMesh.hh
    bits/AaMeshBox.hh
    bits/AaMeshExport.hh
    bits/AaMeshGts.hh
    bits/AaMeshNOFF.hh
    bits/AaMeshOptimizer.hh
    bits/AaMeshPolygonizer.hh
    bits/AaMeshPrimitives.hh
    bits/AaMeshRenderer.hh
    bits/AaMeshSeparator.hh
    bits/AaMeshVBO.hh
    PATHS
    ${AAMESH_PATH}/include
)

if(AAMESH_INCLUDE_DIR)
    set(AAMESH_FOUND TRUE)
endif(AAMESH_INCLUDE_DIR)

if(AAMESH_FOUND)
    if(NOT AAMESH_FIND_QUIETLY)
        message(STATUS "Found AAMESH : ${AAMESH_INCLUDE_DIR}")
    endif(NOT AAMESH_FIND_QUIETLY)
else(AAMESH_FOUND)
    if(AAMESH_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find AAMESH")
    endif(AAMESH_FIND_REQUIRED)
endif(AAMESH_FOUND)
