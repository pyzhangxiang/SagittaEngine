INCLUDE_DIRECTORIES( ${BULLET_PHYSICS_SOURCE_DIR}/src  )



SET(BulletDynamics_SRCS
	Character/btKinematicCharacterController.cpp
	ConstraintSolver/btConeTwistConstraint.cpp
	ConstraintSolver/btContactConstraint.cpp
	ConstraintSolver/btGearConstraint.cpp
	ConstraintSolver/btGeneric6DofConstraint.cpp
	ConstraintSolver/btGeneric6DofSpringConstraint.cpp
	ConstraintSolver/btHinge2Constraint.cpp
	ConstraintSolver/btHingeConstraint.cpp
	ConstraintSolver/btPoint2PointConstraint.cpp
	ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
	ConstraintSolver/btSliderConstraint.cpp
	ConstraintSolver/btSolve2LinearConstraint.cpp
	ConstraintSolver/btTypedConstraint.cpp
	ConstraintSolver/btUniversalConstraint.cpp
	Dynamics/btDiscreteDynamicsWorld.cpp
	Dynamics/btRigidBody.cpp
	Dynamics/btSimpleDynamicsWorld.cpp
	Dynamics/Bullet-C-API.cpp
	Vehicle/btRaycastVehicle.cpp
	Vehicle/btWheelInfo.cpp
)

SET(Root_HDRS
	../btBulletDynamicsCommon.h
	../btBulletCollisionCommon.h
)
SET(ConstraintSolver_HDRS
	ConstraintSolver/btConeTwistConstraint.h
	ConstraintSolver/btConstraintSolver.h
	ConstraintSolver/btContactConstraint.h
	ConstraintSolver/btContactSolverInfo.h
	ConstraintSolver/btGearConstraint.h
	ConstraintSolver/btGeneric6DofConstraint.h
	ConstraintSolver/btGeneric6DofSpringConstraint.h
	ConstraintSolver/btHinge2Constraint.h
	ConstraintSolver/btHingeConstraint.h
	ConstraintSolver/btJacobianEntry.h
	ConstraintSolver/btPoint2PointConstraint.h
	ConstraintSolver/btSequentialImpulseConstraintSolver.h
	ConstraintSolver/btSliderConstraint.h
	ConstraintSolver/btSolve2LinearConstraint.h
	ConstraintSolver/btSolverBody.h
	ConstraintSolver/btSolverConstraint.h
	ConstraintSolver/btTypedConstraint.h
	ConstraintSolver/btUniversalConstraint.h
)
SET(Dynamics_HDRS
	Dynamics/btActionInterface.h
	Dynamics/btDiscreteDynamicsWorld.h
	Dynamics/btDynamicsWorld.h
	Dynamics/btSimpleDynamicsWorld.h
	Dynamics/btRigidBody.h
)
SET(Vehicle_HDRS
	Vehicle/btRaycastVehicle.h
	Vehicle/btVehicleRaycaster.h
	Vehicle/btWheelInfo.h
)

SET(Character_HDRS
	Character/btCharacterControllerInterface.h
	Character/btKinematicCharacterController.h
)



SET(BulletDynamics_HDRS
	${Root_HDRS}
	${ConstraintSolver_HDRS}
	${Dynamics_HDRS}
	${Vehicle_HDRS}
	${Character_HDRS}
)

IF (BUILD_SHARED_LIBS)
	ADD_LIBRARY(BulletDynamics SHARED ${BulletDynamics_SRCS} ${BulletDynamics_HDRS})
ELSE (BUILD_SHARED_LIBS)
	ADD_LIBRARY(BulletDynamics STATIC ${BulletDynamics_SRCS} ${BulletDynamics_HDRS})
ENDIF (BUILD_SHARED_LIBS)

SET_TARGET_PROPERTIES(BulletDynamics PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletDynamics PROPERTIES SOVERSION ${BULLET_VERSION})
IF (BUILD_SHARED_LIBS)
	TARGET_LINK_LIBRARIES(BulletDynamics BulletCollision LinearMath)
ENDIF (BUILD_SHARED_LIBS)
