# Boost support
macro(compileWithBoost)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost COMPONENTS date_time filesystem graph iostreams regex serialization signals system thread unit_test_framework wave) 
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endmacro(compileWithBoost)

macro(linkWithBoost projectName)
    if(NOT MSVC)
        target_link_libraries(${projectName} ${Boost_LIBRARIES})
    endif(NOT MSVC)
endmacro(linkWithBoost)

# FBX SDK and FreeImage
macro(linkWithFBXSDK projectName)
    if(MSVC)
        target_link_libraries(${projectName} 
            debug       FreeImaged
            optimized   FreeImage)
        target_link_libraries(${projectName} wininet)
        if(MSVC80)
            target_link_libraries(${projectName} 
                debug       fbxsdk_md2005d
                optimized   fbxsdk_md2005)
        else(MSVC80)
            target_link_libraries(${projectName} 
                debug       fbxsdk_md2008d
                optimized   fbxsdk_md2008)
        endif(MSVC80)
    endif(MSVC)
    if(UNIX)
        target_link_libraries(${projectName} freeimage)
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            target_link_libraries(${projectName} 
                debug       fbxsdk_gcc4
                optimized   fbxsdk_gcc4)
        else(CMAKE_SIZEOF_VOID_P EQUAL 4)
            target_link_libraries(${projectName} 
                debug       fbxsdk_gcc4_x64d
                optimized   fbxsdk_gcc4_x64)
        endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    endif(UNIX)
endmacro(linkWithFBXSDK)

# Bullet Physics
macro(linkWithBullet projectName)
    target_link_libraries(${projectName} 
            BulletSoftBody BulletDynamics BulletCollision LinearMath)
endmacro(linkWithBullet)

