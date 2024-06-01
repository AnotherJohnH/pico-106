#-------------------------------------------------------------------------------
#  Copyright (c) 2024 John D. Haughton
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

add_custom_command(
   COMMENT
      "Auto-generate tables"
   COMMAND
      ${CMAKE_COMMAND}
         -E env PYTHONPATH=${python_path}
         ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
   DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
      ${python_path}/Table.py
   OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/Table_exp_24.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_exp_24.cpp
   )

add_executable(pico106
               Table_exp_24.cpp
               pico106_${PLT_TARGET}.cpp)

target_include_directories(pico106
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(${PLT_TARGET} STREQUAL native)
   target_link_libraries(pico106 PRIVATE PLT STB)
else()
   target_link_libraries(pico106 PRIVATE TNY MTL TNY)
endif()
