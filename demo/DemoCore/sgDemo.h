#ifndef __SGDEMO_H__
#define __SGDEMO_H__

#include <math/SagittaPlatform.h>
#include "sgKeyDefine.h"
#include "sgInputEvent.h"
#include <string>
#include <math/sgVector3.h>

class sgDemo;
_SG_DemoExport sgDemo *GetApp(void);

_SG_DemoExport void sgMessageBox(const std::string &title, const std::string &content);

namespace Sagitta 
{
	class sgScene;
	class sgSceneObject;
}

class _SG_DemoExport sgDemo
{
private:
    float mFrameRate;
    
	// only for window creating
    int mRenderWindowX; // default -1 -- center of the screen
    int mRenderWindowY; // default -1 -- center of the screen
    int mRenderWindowW; // default 400
    int mRenderWindowH; // default 300
	std::string mRenderWindowTitle;

	bool mDestroying;

    std::string mAssetRootDir;
    
protected:
	// scene members
	Sagitta::sgScene *mScene;
	Sagitta::sgSceneObject *mTargetRoot;
	Sagitta::sgSceneObject *mCamera;
	Sagitta::sgSceneObject *mLight;

	Sagitta::Vector3 mRotationSphereCenter;
	Sagitta::Real mRotationSphereRadius;
	Sagitta::Vector3 mStartRotRayX;
	Sagitta::Vector3 mStartRotRayY;
	Sagitta::Quaternion mStartRotOrientation;

	Sagitta::Vector3 mStartRotRayTarget;
	Sagitta::Quaternion mStartRotOrientationTarget;

public:
    sgDemo(const std::string &winTitle,
           const std::string &assetRootDir);
    virtual ~sgDemo(void);
    
    int run(int argc, char *argv[]);
    
    int getRenderWindowWidth(void) const;
    int getRenderWindowHeight(void) const;
    float getFrameRate(void) const;
	void setFrameRate(float fr);

	void setWindowSize(int width, int height);
	void setWindowPos(int x, int y);
	void setWindowTitle(const std::string title);
    
    bool isDestroying(void) const{ return mDestroying; }

protected:
	virtual void createEvent(void);
	virtual void destroyEvent(void);

public:
	void OnCreate(void);
	void OnDestroy(void);

    virtual void keyPressEvent(sgKeyEvent &event);
    virtual void keyReleaseEvent(sgKeyEvent &event){}
    virtual void mousePressEvent(sgMouseEvent &event);
    virtual void mouseReleaseEvent(sgMouseEvent &event){}
    virtual void mouseMoveEvent(sgMouseEvent &event);
    virtual void mouseDoubleClickEvent(sgMouseEvent &event){}
    virtual void wheelEvent(sgWheelEvent &event){}
    
	void initialize(void);
protected:
	// called by initialize, prepare the scene
	virtual void prepare(void){};

public:
	virtual void update(float dt);
	virtual void render(void);
	virtual void resize(int width, int height);


    
};
    

#endif // __SGDEMO_H__