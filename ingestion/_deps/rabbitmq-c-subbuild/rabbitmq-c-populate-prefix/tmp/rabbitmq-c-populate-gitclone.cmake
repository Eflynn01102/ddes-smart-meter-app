
if(NOT "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitinfo.txt" IS_NEWER_THAN "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/alanxz/rabbitmq-c.git" "rabbitmq-c-src"
    WORKING_DIRECTORY "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/alanxz/rabbitmq-c.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout master --
  WORKING_DIRECTORY "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitinfo.txt"
    "/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/louie/ingestion/ddes-smart-meter-app/ingestion/_deps/rabbitmq-c-subbuild/rabbitmq-c-populate-prefix/src/rabbitmq-c-populate-stamp/rabbitmq-c-populate-gitclone-lastrun.txt'")
endif()

