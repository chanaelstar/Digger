# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-src")
  file(MAKE_DIRECTORY "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-build"
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix"
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/tmp"
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/src/exe_path-populate-stamp"
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/src"
  "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/src/exe_path-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/src/exe_path-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/ALICE/Desktop/IMAC/Travail/Programmation et Algorithmie/renduS2/Digger/build/_deps/exe_path-subbuild/exe_path-populate-prefix/src/exe_path-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
