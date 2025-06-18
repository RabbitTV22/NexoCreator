cmake_minimum_required(VERSION 3.31)
project(_idea)

set(CMAKE_CXX_STANDARD 14)

add_executable(_idea
        editor.xml
        final_project.iml
        icon.png
        misc.xml
        modules.xml
        vcs.xml)
