# /!\ NB: you need at least CMake version 3.20 for these functions to work

#! Copies INPUT_FILE to OUTPUT_FILE whenever the INPUT_FILE has changed
#  Both INPUT_FILE and OUTPUT_FILE must be absolute paths
function(Cool__target_copy_file_absolute_paths
         TARGET
         INPUT_FILE
         OUTPUT_FILE
)
    string(MD5 DUMMY_OUTPUT_NAME "${INPUT_FILE}${OUTPUT_FILE}${TARGET}")
    set(DUMMY_OUTPUT_NAME timestamp_${DUMMY_OUTPUT_NAME})
    add_custom_command(
        COMMENT "Copying \"${INPUT_FILE}\" to \"${OUTPUT_FILE}\""
        OUTPUT ${DUMMY_OUTPUT_NAME}
        COMMAND ${CMAKE_COMMAND} -E touch "${DUMMY_OUTPUT_NAME}" # Create a dummy file that CMake will use as a timestamp reference to know if the actual file has changed, when it checks for the OUTPUT (unfortunately OUTPUT can't use a generator expression so we can't use our actual output file as the OUTPUT)
        COMMAND ${CMAKE_COMMAND} -E copy "${INPUT_FILE}" "${OUTPUT_FILE}" # Actual copy of the file to the destination
        DEPENDS ${INPUT_FILE}
        VERBATIM
    )
    target_sources(${TARGET} PRIVATE ${DUMMY_OUTPUT_NAME}) # Required for the custom command to be run when we build our target
endfunction()

function(Cool__target_copy_file
         TARGET
         FILE
)
    # Get the part of the file path relative to the top-level CMakeLists.txt
    cmake_path(RELATIVE_PATH FILE BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE FILE_RELATIVE_PATH)

    if(NOT FILE_RELATIVE_PATH)
        set(FILE_RELATIVE_PATH ${FILE})
    endif()

    # Get OUT_FILE as an optional parameter
    if (${ARGC} GREATER_EQUAL 3)
        set(OUT_FILE $<TARGET_FILE_DIR:${TARGET}>/${ARGV2})
    else()
        set(OUT_FILE $<TARGET_FILE_DIR:${TARGET}>/${FILE_RELATIVE_PATH})
    endif()

    # Add the copy command
    Cool__target_copy_file_absolute_paths(${TARGET}
        ${CMAKE_SOURCE_DIR}/${FILE_RELATIVE_PATH}
        ${OUT_FILE})
endfunction()

function(Cool__target_copy_folder
    TARGET
    FOLDER)
    # Get the part of the folder path relative to CMAKE_SOURCE_DIR (the top-level CMakeLists.txt)
    cmake_path(RELATIVE_PATH FOLDER BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE FOLDER_RELATIVE_PATH)

    if(NOT FOLDER_RELATIVE_PATH)
        set(FOLDER_RELATIVE_PATH ${FOLDER})
    endif()

    # Get the absolute folder path
    set(FOLDER_ABSOLUTE_PATH ${CMAKE_SOURCE_DIR}/${FOLDER_RELATIVE_PATH})

    # Copy each file
    file(GLOB_RECURSE FILES CONFIGURE_DEPENDS ${FOLDER_ABSOLUTE_PATH}/*)

    foreach(FILE ${FILES})
        if(${ARGC} GREATER_EQUAL 3)
            cmake_path(RELATIVE_PATH FILE BASE_DIRECTORY ${FOLDER_ABSOLUTE_PATH} OUTPUT_VARIABLE FILE_RELATIVE_PATH)
            Cool__target_copy_file(${TARGET} ${FILE} ${ARGV2}/${FILE_RELATIVE_PATH})
        else()
            Cool__target_copy_file(${TARGET} ${FILE})
        endif()
    endforeach()
endfunction()

function(Cool__create_file_if_it_doesnt_exist FILE)
    file(APPEND ${FILE} "")
endfunction()