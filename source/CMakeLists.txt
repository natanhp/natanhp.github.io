SET(WT_PROJECT_SOURCE
        main.cpp
        blog.cpp blog.h about.cpp about.h util.cpp util.h)


SET(WT_PROJECT_TARGET natanhpid.wt)

ADD_EXECUTABLE(${WT_PROJECT_TARGET} ${WT_PROJECT_SOURCE})

# Add the include directories for Wt
include_directories(${CONAN_INCLUDE_DIRS})

include_directories(${CONAN_INCLUDE_DIRS_WT})

target_link_libraries(${WT_PROJECT_TARGET} ${CONAN_LIBS})

ADD_CUSTOM_COMMAND(TARGET ${WT_PROJECT_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${WT_PROJECT_TARGET}>/resources)
