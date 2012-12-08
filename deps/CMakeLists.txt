
add_subdirectory(stb_image)
add_subdirectory(pvrt)
add_subdirectory(bullet)
add_subdirectory(alglib)


SET_PROPERTY(TARGET stb_image PROPERTY FOLDER "deps")
SET_PROPERTY(TARGET pvrt_sdk PROPERTY FOLDER "deps")
SET_PROPERTY(TARGET alglib PROPERTY FOLDER "deps")


SET_PROPERTY(TARGET LinearMath PROPERTY FOLDER "deps/bullet")
SET_PROPERTY(TARGET BulletCollision PROPERTY FOLDER "deps/bullet")
SET_PROPERTY(TARGET BulletDynamics PROPERTY FOLDER "deps/bullet")
SET_PROPERTY(TARGET BulletSoftBody PROPERTY FOLDER "deps/bullet")