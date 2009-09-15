//////////////////////////////////////////////////////
// file: SNode.h @ 2008-1-31 by Zhang Xiang
// declares of the class SNode
// SNode is a class ...
//////////////////////////////////////////////////////
#ifndef __SNODE_H__
#define __SNODE_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../math/SMathHeader.h"
#include "../../common/utils/SIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SNode : public SObject{
	// enum defines
	public:
		/** Enumeration denoting the spaces which a transform can be relative to.
		*/
		enum TransformSpace{
			/// Transform is relative to the local space
			TS_LOCAL = 0,
			/// Transform is relative to the space of the parent node
			TS_PARENT = 1,
			/// Transform is relative to world space
			TS_WORLD = 2
		};

	// type defines
	protected:
		typedef std::map<uLong, SNode*> ChildNodeMap;
	public:
		typedef SMapIterator<ChildNodeMap> ChildIterator;
		typedef SConstMapIterator<ChildNodeMap> ConstChildIterator;

	// static members
	private:
		/// Incremented count for next name extension
		static uLong ms_iNodeCount;

	// member variables	//////////////////////////////////////////////////////////////////////////
	private:
		/// node id
		const uLong mc_iNodeID;

		/// node name
		StdString m_sName;

		/// whether this node is active
		bool m_bActive;

		/// Pointer to parent node
		SNode *m_pParent;
		
		/// Flag to indicate own transform from parent is out of date
		mutable bool m_bNeedUpdateFromParent;

		/// Stores the orientation of the node relative to it's parent.
		Quaternion m_RelativeOrientation;

		/// Stores the position/translation of the node relative to its parent.
		Vector3 m_RelativePosition;

		/// Stores the scaling factor applied to this node
		Vector3 m_RelativeScale;

		/// Stores whether this node inherits orientation from it's parent
		bool m_bInheritOrientation;

		/// Stores whether this node inherits scale from it's parent
		bool m_bInheritScale;

		/** Cached combined orientation.
			@par
				This member is the orientation derived by combining the
				local transformations and those of it's parents.
				This is updated when _updateFromParent is called by the
				SceneManager or the nodes parent.
		*/
		mutable Quaternion m_DerivedOrientation;

		/** Cached combined position.
			@par
				This member is the position derived by combining the
				local transformations and those of it's parents.
				This is updated when _updateFromParent is called by the
				SceneManager or the nodes parent.
		*/
		mutable Vector3 m_DerivedPosition;

		/** Cached combined scale.
			@par
				This member is the position derived by combining the
				local transformations and those of it's parents.
				This is updated when _updateFromParent is called by the
				SceneManager or the nodes parent.
		*/
		mutable Vector3 m_DerivedScale;

		/// Cached derived transform as a 4x4 matrix
		mutable Matrix4 m_CachedTransform;
		mutable bool m_bCachedTransformOutOfDate;

	protected:
		/// Collection of pointers to direct children; hashmap for efficiency
		ChildNodeMap m_Children;

	// constructors & destructor //////////////////////////////////////////////////////////////////////////
	public:
		/** Constructor.
		*	@remarks
		*		Generates a name.
		*/
		SNode(void);
		/** Constructor.
		*	@remarks
		*		Assigned a name.
		*/
		SNode(const StdString &aName);
		virtual ~SNode(void);

	// member functions //////////////////////////////////////////////////////////////////////////
	private:
		/** Triggers the node to update it's combined transforms.
			@par
				This method is called internally by Sagitta to ask the node
				to update it's complete transformation based on it's parents
				derived transform.
		*/
		void _updateFromParent(void) const;

		/** Gets the orientation of the node as derived from all parents. */
		const Quaternion &_getDerivedOrientation(void) const;

		/** Gets the position of the node as derived from all parents. */
		const Vector3 &_getDerivedPosition(void) const;

		/** Gets the scaling factor of the node as derived from all parents. */
		const Vector3 &_getDerivedScale(void) const;

	protected:
		/** Changes the Node's parent.
			@remarks
				Just set the parent node pointer.
				Only called internally.
		*/
		void setParent(SNode *aParent);

		/** Sets my active property.
			@remarks Notify children to set active.
		*/
		void setActive(bool aActive);

	public:
		/** Clone this node - should be re-implemented by subclass*/
		virtual SNode *clone(void) const = 0;

		/** Gets id. */
		uLong getNodeID(void) const;

		/** Gets my name. */
		const StdString &name(void) const;
		/** Sets my name. */
		void setName(const StdString &aName);

		/** Check if I'm active. */
		bool isActive(void) const;

		/** Step myself to another parent. */
		virtual void stepToParent(SNode *aNewParent);

        /** Adds a (precreated) child scene node to this node. If it is attached to another node,
            it must be detached first.
			@param
				aChild The Node which is to become a child node of this one
        */
        virtual void addChild(SNode* aChild);

		/** Drops the specified child from this node. 
			@remarks
				Does not delete the node, just detaches it from
				this parent, potentially to be reattached elsewhere. 
				There is also an alternate version which drops a named
				child from this node.
        */
        SNode *removeChild(size_t aIndex);

        /** Drops the specified child from this node. 
			@remarks
				Does not delete the node, just detaches it from
				this parent, potentially to be reattached elsewhere. 
				There is also an alternate version which drops a named
				child from this node.
        */
        SNode *removeChild(SNode *apChild);

        /** Removes all child Nodes attached to this node. Does not delete the nodes, just detaches them from
            this parent, potentially to be reattached elsewhere.
        */
        void removeAllChildren(void);

		/** Gets my parent (NULL if this is the root).
		*/
		SNode *parent(void) const;
		
		/** Gets a quaternion representing the node's relative orientation. */
		const Quaternion &relativeOrentation(void) const;

		/** Sets the relative orientation of this node via a quaternion.
			@remarks
				Orientatings, unlike other transforms, are not always inherited by child nodes.
				Whether or not orientatings affect the orientation of the child nodes depends on
				the setInheritOrientation option of the child. In some cases you want a orientating
				of a parent node to apply to a child node (e.g. where the child node is a part of
				the same object, so you want it to be the same relative orientation based on the
				parent's orientation), but not in other cases (e.g. where the child node is just
				for positioning another object, you want it to maintain it's own orientation).
				The default is to inherit as with other transforms.
			@par
				Note that rotations are oriented around the node's origin.
		*/
		void setRelativeOrientation(const Quaternion &aq);

		/** Resets the nodes relative orientation (local axes as world axes, no rotation).
			@remarks
				Orientatings, unlike other transforms, are not always inherited by child nodes.
				Whether or not orientatings affect the orientation of the child nodes depends on
				the setInheritOrientation option of the child. In some cases you want a orientating
				of a parent node to apply to a child node (e.g. where the child node is a part of
				the same object, so you want it to be the same relative orientation based on the
				parent's orientation), but not in other cases (e.g. where the child node is just
				for positioning another object, you want it to maintain it's own orientation).
				The default is to inherit as with other transforms.
			@par
				Note that rotations are oriented around the node's origin.
		*/
		void resetRelativeOrientation(void);

		/** Gets a quaternion representing the node's absolute orientation of the world space. */
		const Quaternion &absoluteOrientation(void) const;

		/** Gets the orientation of the node, return value depends on if inherited. */
		const Quaternion &orientation(void) const;

		/** Gets the position of the node relative to it's parent. */
		const Vector3 &relativePosition(void) const;

		/** Sets the position of the node relative to it's parent. */
		void setRelativePosition(const Vector3 &aPos);

		/** Gets the absolute position of the node in the world space. */
		const Vector3 &absolutePosition(void) const;

		/** Return this node's position in the world space. The same as absolutePosition. */
		Vector3 position(void) const;

		/** Sets the absolute position of the node in the world space. */
		void setAbsolutePosition(const Vector3 &aPos);

		/** Gets the relative scaling factor applied this node. */
		const Vector3 &relativeScale(void) const;

		/** Sets the relative scaling factor applied to this node.
			@remarks
				Scaling factors, unlike other transforms, are not always inherited by child nodes.
				Whether or not scalings affect the size of the child nodes depends on the setInheritScale
				option of the child. In some cases you want a scaling factor of a parent node to apply to
				a child node (e.g. where the child node is a part of the same object, so you want it to be
				the same relative size based on the parent's size), but not in other cases (e.g. where the
				child node is just for positioning another object, you want it to maintain it's own size).
				The default is to inherit as with other transforms.
			@par
				Note that like rotations, scalings are oriented around the node's origin.
		*/
		void setRelativeScale(const Vector3 &aScale);

		/** Gets the absolute scaling factor applied this node. */
		const Vector3 &absoluteScale(void) const;

		/** Gets the scale factor of the node, return value depends on if inherited. */
		const Vector3 &scale(void) const;

		/** Returns true if this node is affected by orientation applied to the parent node. 
			@remarks
				Orientations, unlike other transforms, are not always inherited by child nodes.
				Whether or not orientations affect the orientation of the child nodes depends on
				the setInheritOrientation option of the child. In some cases you want a orientating
				of a parent node to apply to a child node (e.g. where the child node is a part of
				the same object, so you want it to be the same relative orientation based on the
				parent's orientation), but not in other cases (e.g. where the child node is just
				for positioning another object, you want it to maintain it's own orientation).
				The default is to inherit as with other transforms.
			@remarks
				See setInheritOrientation for more info.
		*/
		bool inheritOrientation(void) const;


		/** Tells the node whether it should inherit orientation from it's parent node.
			@remarks
				Orientations, unlike other transforms, are not always inherited by child nodes.
				Whether or not orientations affect the orientation of the child nodes depends on
				the setInheritOrientation option of the child. In some cases you want a orientating
				of a parent node to apply to a child node (e.g. where the child node is a part of
				the same object, so you want it to be the same relative orientation based on the
				parent's orientation), but not in other cases (e.g. where the child node is just
				for positioning another object, you want it to maintain it's own orientation).
				The default is to inherit as with other transforms.
			@param inherit If true, this node's orientation will be affected by its parent's orientation.
				If false, it will not be affected.
		*/
		void setInheritOrientation(bool abInherit);

		/** Returns true if this node is affected by scaling factors applied to the parent node. 
			@remarks
				See setInheritScale for more info.
		*/
		bool inheritScale(void) const;

		/** Tells the node whether it should inherit scaling factors from it's parent node.
			@remarks
				Scaling factors, unlike other transforms, are not always inherited by child nodes.
				Whether or not scalings affect the size of the child nodes depends on the setInheritScale
				option of the child. In some cases you want a scaling factor of a parent node to apply to
				a child node (e.g. where the child node is a part of the same object, so you want it to be
				the same relative size based on the parent's size), but not in other cases (e.g. where the
				child node is just for positioning another object, you want it to maintain it's own size).
				The default is to inherit as with other transforms.
			@param inherit If true, this node's scale will be affected by its parent's scale. If false,
				it will not be affected.
		*/
		void setInheritScale(bool abInherit);

		/** Scales the node, combining it's current scale with the passed in scaling factor. 
			@remarks
				This method applies an extra scaling factor to the node's existing scale, (unlike setScale
				which overwrites it) combining it's current scale with the new one. E.g. calling this 
				method twice with Vector3(2,2,2) would have the same effect as setScale(Vector3(4,4,4)) if
				the existing scale was 1.
			@par
				Note that like rotations, scalings are oriented around the node's origin.
		*/
		void scale(const Vector3 &aScale);

		/** Moves the node along the cartesian axes.
			@par
				This method moves the node by the supplied vector along the
				world cartesian axes, i.e. along world x,y,z
			@param 
				d Vector with x,y,z values representing the translation.
			@param
				relativeTo The space which this transform is relative to.
		*/
		void translate(const Vector3 &aTranslate, int aRelativeTo = TS_PARENT);
		
		/** Moves the node along arbitrary axes.
			@remarks
				This method translates the node by a vector which is relative to
				a custom set of axes.
			@param 
				axes A 3x3 Matrix containg 3 column vectors each representing the
				axes X, Y and Z respectively. In this format the standard cartesian
				axes would be expressed as:
				<pre>
				1 0 0
				0 1 0
				0 0 1
				</pre>
				i.e. the identity matrix.
			@param 
				move Vector relative to the axes above.
			@param
				relativeTo The space which this transform is relative to.
		*/
		void translate(const Matrix3 &aAxes, const Vector3 &aTranslate, int aRelativeTo = TS_PARENT);

		/** Rotate the node around the Z-axis. */
		void roll(const Radian &aAngle, int aRelativeTo = TS_LOCAL);
		void rotationZ(const Radian &aAngle, int aRelativeTo = TS_LOCAL);

		/** Rotate the node around the X-axis. */
		void pitch(const Radian &aAngle, int aRelativeTo = TS_LOCAL);
		void rotationX(const Radian &aAngle, int aRelativeTo = TS_LOCAL);

		/** Rotate the node around the Y-axis. */
		void yaw(const Radian &aAngle, int aRelativeTo = TS_LOCAL);
		void rotationY(const Radian &aAngle, int aRelativeTo = TS_LOCAL);

		/** Rotate the node around an arbitrary axis. */
		void rotate(const Vector3 &aAxis, const Radian& aAngle, int aRelativeTo = TS_LOCAL);

		/** Rotate the node around an arbitrary axis using a Quaternion. */
		void rotate(const Quaternion &aq, int aRelativeTo = TS_LOCAL);

		/** Gets a matrix whose columns are the local axes based on
		the nodes orientation relative to it's parent. */
		Matrix3 getLocalAxes(void) const;

        /** Reports the number of child nodes under this one.
        */
        size_t childNum(void) const;

        /** Gets a pointer to a child node.
			@remarks
				There is an alternate getChild method which returns child by name.
		*/
        SNode *getChild(size_t aIndex) const;    

        /** Gets child by ID.
			@remarks
				return the first one found.
		*/
		SNode *getChild(const StdString &aName) const;

		/** Gets the full transformation matrix for this node.
            @remarks
                This method returns the full transformation matrix
                for this node, including the effect of any parent node
                transformations, provided they have been updated using the Node::_update method.
                This should only be called by a SceneManager which knows the
                derived transforms have been updated before calling this method.
                Applications using Sagitta should just use the relative transforms.
        */
        const Matrix4 &_getFullTransform(void) const;

		/** Returns if need update from parent. */
		bool isNeedUpdate(void) const;

		/** To be called in the event of transform changes to this node that require it's recalculation.
			@remarks
				This not only tags the node state as being 'dirty', it also requests it's parent to 
				know about it's dirtiness so it will get an update next time.
			@param forceParentUpdate Even if the node thinks it has already told it's
				parent, tell it anyway
        */
        void needUpdate(void);

		/** Gets child iterator. */
		ChildIterator getChildIterator(void);

		/** Gets const child iterator. */
		ConstChildIterator getConstChildIterator(void) const;
        
	}; //#### end class SNode

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SNODE_H__