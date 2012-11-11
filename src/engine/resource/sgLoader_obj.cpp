//////////////////////////////////////////////////////
// file: sgResource.cpp 
// created by zhangxiang on 09-01-09
// define of the class sgResource
// sgResource is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgLoader.h"
#include "sgResource.h"
#include "sgResourceCenter.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/resource/sgMesh.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgUtil.h"
#include "engine/common/sgException.h"

#include <fstream>

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	sgSceneObject *sgLoader::load_obj(const std::string &filename)
    {
        sgSceneObject *sceneRoot = 0;
        
        size_t lastslash = filename.find_last_of("\\/");
		size_t lastdot = filename.find_last_of(".");
        
        bool isobj = true;
        if(lastdot != std::string::npos)
        {
            std::string ext = filename.substr(lastdot);
            if(ext != ".obj")
            {
                isobj = false;
            }
			else
            {
                isobj = true;
                
                std::ifstream file((sgResourceCenter::instance()->getRootDir() + filename).c_str());
                if(file.fail())
                {
                    THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                                      "cannot open the obj file: " + filename + ".",
                                      "sgLoaderObj");

                }
                
                // laod from obj
                sceneRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
                
                std::string sceneName;
                if(lastslash == std::string::npos)
                {
                    sceneName = filename.substr(0, lastdot);
                }
                else
                {
                    sceneName = filename.substr(lastslash + 1, lastdot);
                }
                sceneRoot->setName(sceneName + "_Node");
                
                size_t gCount = 0;			// group count
                size_t gVCount = 0;			// current group vertex count
                size_t gTCount = 0;			// current group texcoord count
                size_t gNCount = 0;			// current group normal count
                size_t gFCount = 0;			// current group face count
                std::string gName = "";		// current group name
                
                // Now assume triangle base scene,
                // and assume using Vector3 to express a vertex, and a normal
                // and using Vector2 to express a texcoord.
                // if this assumes are not support, throw exception.
                // for future to support more types.
                // for future ...
                uInt polyType = 3;
                
                std::vector<size_t> vcounts;
                std::vector<size_t> tcounts;
                std::vector<size_t> ncounts;
                std::vector<size_t> fcounts;
              //  std::vector<sgEntity*> groups;
                std::vector<sgMesh*> groups;
                std::vector<sgSceneObject*> objGroups;
                vcounts.reserve(7);
                tcounts.reserve(7);
                ncounts.reserve(7);
                groups.reserve(7);
                
                // store the state
                // 1 - in v, 2 - in face
                int state = 2;
                
                // for get line
                std::vector<std::string> words;
                
                std::string strLine;
                
                // first pass begin //////////////////////////////////////////////////////////////////////////
                // travel the file to
                // gets all the statistics of the model
                // (such as #vertices, #normals, etc)
                char c;
                while(file >> c){
                    switch(c){
                        case '#':		// comment
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'v':		// v, vn, vt
                        {
                            if(state != 1){
                                // start a new group
                                // first take charge latest group
                                if(gFCount != 0){
                                    // have a latest group
                                    if(gName.empty())
                                        gName = sceneName + sgStringUtil::to_string(static_cast<uInt>(gCount));
                                    
                                    bool bsmooth = true;
                                    if(gNCount != 0 && gVCount != gNCount){
                                        bsmooth = false;
                                    }
                                    // assume counterclockwise
                                    sgMesh *mesh = (sgMesh*)sgResourceCenter::instance()->createResource(sgMesh::GetClassName(), (filename + gName + "_mesh" + sgStringUtil::to_string(static_cast<uInt>(gCount))).c_str());
                                    mesh->reset(polyType, gVCount, gFCount);
                             //       sgEntity *entity = new sgEntity(gName, polyType, gVCount, gFCount, bsmooth);
                              //      entity->renderOption().setRenderElementType(sgRenderOption::RET_TRIANGLES);
                                    sgSceneObject *node = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
                                    node->setName(gName + "_node");
                                    node->setParent(sceneRoot);
                                    sgMeshComponent *meshComp = (sgMeshComponent*)node->createComponent(sgMeshComponent::GetClassName());
                                    meshComp->setMeshFile(mesh->getFilename());
                                    //groups.push_back(entity);
                                    groups.push_back(mesh);
                                    objGroups.push_back(node);
                                    // init vertex and index data
                                    sgVertexData *vdata = mesh->getVertexData();
                                    sgIndexData *idata = mesh->getIndexData();
                                    if(gVCount > 0){
                                        vdata->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, gVCount);
                                        idata->createElement(sgVertexBufferElement::ET_VERTEX);
                                    }
                                    if(gNCount > 0){
                                        vdata->createElement(sgVertexBufferElement::NormalAttributeName, RDT_F, 3, gNCount);
                                        idata->createElement(sgVertexBufferElement::ET_NORMAL);
                                    }
                                    if(gTCount > 0){
                                        vdata->createElement(sgVertexBufferElement::UV0AttributeName, RDT_F, 2, gTCount);
                                        idata->createElement(sgVertexBufferElement::ET_TEXTURE_COORD);
                                    }
                                    
                                    vcounts.push_back(gVCount);
                                    tcounts.push_back(gTCount);
                                    ncounts.push_back(gNCount);
                                    fcounts.push_back(gFCount);
                                    
                                    gCount++;
                                    
                                    gVCount = 0;
                                    gNCount = 0;
                                    gTCount = 0;
                                    gFCount = 0;
                                    
                                    gName.clear();
                                }
                                // now we are in 'v' area
                                state = 1;
                            }
                            
                            c = file.get();
                            switch(c){
                                case ' ':	// vertex
                                {
                                    ++gVCount;
                                    // eat up rest of line
                                    std::getline(file, strLine);
                                    break;
                                }
                                    
                                case 'n':	// normal
                                {
                                    ++gNCount;
                                    // eat up rest of line
                                    std::getline(file, strLine);
                                    break;
                                }
                                    
                                case 't':	// texcoord
                                {
                                    ++gTCount;
                                    // eat up rest of line
                                    std::getline(file, strLine);
                                    break;
                                }
                                    
                                default:	// unknown
                                {
                                    file.close();
                                    THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                                                      "Unknown token after \"v\".",
                                                      "sgSceneDelegate::loadScene_obj");
                                }
                            }
                            
                            break;
                        } //#### end v switch
                            
                        case 'm':		// mtl file
                        {
                            // read mtl file
                            // for future ...
                            break;
                        }
                            
                        case 'u':		// nothing
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'g':		// group
                        {
                            if(state == 1){
                                // next into 'f' area
                                //file.getLine(&words);
                                std::getline(file, strLine);
                                sgStringUtil::split(words, strLine);
                                if(!words.empty()){
                                    gName = words[0];
                                }
                            }else{
                                // eat up rest of line
                                std::getline(file, strLine);
                            }
                            
                            break;
                        }
                            
                        case 'f':		// face
                        {
                            if(state != 2){
                                // start a group
                                state = 2;
                            }
                            ++gFCount;
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        default:
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                    }
                }
                
                // take charge the last group
                if(gFCount != 0){
                    // have a last group
                    if(gName.empty())
                        gName = sceneName + sgStringUtil::to_string(static_cast<uInt>(gCount));
                    
                    bool bsmooth = true;
                    if(gNCount != 0 && gVCount != gNCount){
                        bsmooth = false;
                    }
                    // assume counterclockwise
                    sgMesh *mesh = (sgMesh*)sgResourceCenter::instance()->createResource(sgMesh::GetClassName(), (filename + gName + "_mesh").c_str());
                    mesh->reset(polyType, gVCount, gFCount);
                    //       sgEntity *entity = new sgEntity(gName, polyType, gVCount, gFCount, bsmooth);
                    //      entity->renderOption().setRenderElementType(sgRenderOption::RET_TRIANGLES);
                    sgSceneObject *node = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
                    node->setName(gName + "_node");
                    node->setParent(sceneRoot);
                    sgMeshComponent *meshComp = (sgMeshComponent*)node->createComponent(sgMeshComponent::GetClassName());
                    meshComp->setMeshFile(mesh->getFilename());
                    //groups.push_back(entity);
                    groups.push_back(mesh);
                    objGroups.push_back(node);
                    // init vertex and index data
                    sgVertexData *vdata = mesh->getVertexData();
                    sgIndexData *idata = mesh->getIndexData();
                    if(gVCount > 0){
                        vdata->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, gVCount);
                        idata->createElement(sgVertexBufferElement::ET_VERTEX);
                    }
                    if(gNCount > 0){
                        vdata->createElement(sgVertexBufferElement::NormalAttributeName, RDT_F, 3, gNCount);
                        idata->createElement(sgVertexBufferElement::ET_NORMAL);
                    }
                    if(gTCount > 0){
                        vdata->createElement(sgVertexBufferElement::UV0AttributeName, RDT_F, 2, gTCount);
                        idata->createElement(sgVertexBufferElement::ET_TEXTURE_COORD);
                    }
                    
                    vcounts.push_back(gVCount);
                    tcounts.push_back(gTCount);
                    ncounts.push_back(gNCount);
                    fcounts.push_back(gFCount);
                    
                    gCount++;
                    
                    
                    gVCount = 0;
                    gNCount = 0;
                    gTCount = 0;
                    gFCount = 0;
                    
                    gName.clear();
                }
                // first pass end //////////////////////////////////////////////////////////////////////////
                
                // second pass begin //////////////////////////////////////////////////////////////////////////
                // read and store data
                size_t tVCount = 0;			// total vertex count till current
                size_t tTCount = 0;			// total texcoord count till current
                size_t tNCount = 0;			// total normal count till current
                gCount = 0;
                gVCount = 0;
                gNCount = 0;
                gTCount = 0;
                gFCount = 0;
                state = 0;
                
                // store datas
                Vector3 *vertices = 0;
                Vector3 *normals = 0;
                Vector2 *texcoords = 0;
                IndexedTriangle *vindices = 0;
                IndexedTriangle *nindices = 0;
                IndexedTriangle *tindices = 0;
                
                Real x, y, z;
                int v, n, t;
                
                if(file.eof()){
                    file.clear();
                }
                file.seekg(0);
                while(file >> c){
                    switch(c){
                        case '#':		// comment
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'v':		// v, vn, vt
                        {
                            if(state != 1){
                                // start a new group
                                // take charge the lastest group
                                if(gFCount != 0){
                                    // have a last group
                                    sgSceneObject *obj = objGroups[gCount];
                                    sgMesh *mesh = groups[gCount];
                                    mesh->setupNormals();
                                    mesh->prepareGeometry();
                                    obj->translate(mesh->center());
                                    mesh->locateToCenter();
                                    
                                    vertices = 0;
                                    normals = 0;
                                    texcoords = 0;
                                    vindices = 0;
                                    nindices = 0;
                                    tindices = 0;
                                    
                                    gCount++;
                                    
                                    tVCount += gVCount;
                                    tNCount += gNCount;
                                    tTCount += gTCount;
                                    
                                    gVCount = 0;
                                    gNCount = 0;
                                    gTCount = 0;
                                    gFCount = 0;
                                }
                                
                                // prepare to setup next entity's data
                                sgMesh *nextMesh = groups[gCount];
                                sgVertexData *vdata = nextMesh->getVertexData();
                                sgIndexData *idata = nextMesh->getIndexData();
                                
                                // vertex
                                vertices = static_cast<Vector3*>(vdata->getElement(sgVertexBufferElement::VertexAttributeName)->data());
                                vindices = static_cast<IndexedTriangle*>(idata->getElement(sgVertexBufferElement::ET_VERTEX)->data());
                                if(ncounts[gCount]){
                                    normals = static_cast<Vector3*>(vdata->getElement(sgVertexBufferElement::NormalAttributeName)->data());
                                    nindices = static_cast<IndexedTriangle*>(idata->getElement(sgVertexBufferElement::ET_NORMAL)->data());
                                }
                                if(tcounts[gCount]){
                                    texcoords = static_cast<Vector2*>(vdata->getElement(sgVertexBufferElement::UV0AttributeName)->data());
                                    tindices = static_cast<IndexedTriangle*>(idata->getElement(sgVertexBufferElement::ET_TEXTURE_COORD)->data());
                                }
                                
                                // now we are in 'v' area
                                state = 1;
                            }
                            
                            c = file.get();
                            switch(c){
                                case ' ':	// vertex
                                {
                                    file >> x >> y >> z;
                                    vertices[gVCount].setValues(x, y, z);
                                    
                                    ++gVCount;
                                    break;
                                }
                                    
                                case 'n':	// normal
                                {
                                    file >> x >> y >> z;
                                    normals[gNCount].setValues(x, y, z);
                                    
                                    ++gNCount;
                                    break;
                                }
                                    
                                case 't':	// texcoord
                                {
                                    file >> x >> y;
                                    texcoords[gTCount].setValues(x, y);
                                    
                                    ++gTCount;
                                    break;
                                }
                                    
                                default:	// unknown
                                {
                                    file.close();
                                    THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                                                      "Unknown token after \"v\".",
                                                      "sgSceneDelegate::loadScene_obj");
                                }
                            }
                            
                            break;
                        } //#### end v switch
                            
                        case 'm':		// mtl file
                        {
                            // read mtl file
                            // for future ...
                            break;
                        }
                            
                        case 'u':		// nothing
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'g':		// group
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'f':		// face
                        {
                            if(state != 2){
                                // start a group
                                state = 2;
                            }
                            
                            v = t = n = 0;
                            for(uInt i=0; i<polyType; i++){
                                if(tcounts[gCount]){
                                    if(ncounts[gCount]){	// v/t/n
                                        file >> v >> c >> t >> n;
                                    }else{			// v/t
                                        file >> v >> c >> t;
                                    }
                                }else{
                                    if(ncounts[gCount]){	// v//n
                                        file >> v >> c >> c >> n;
                                    }else{			// v
                                        file >> v;
                                    }
                                }
                                
                                if(v > 0){
                                    vindices[gFCount][i] = v - tVCount - 1;
                                    if(tindices)
                                        tindices[gFCount][i] = t - tTCount - 1;
                                    if(nindices)
                                        nindices[gFCount][i] = n - tNCount - 1;
                                }else if(v < 0){
                                    vindices[gFCount][i] = v + gVCount;
                                    if(tindices)
                                        tindices[gFCount][i] = t + gTCount;
                                    if(nindices)
                                        nindices[gFCount][i] = n + gNCount;
                                }
                            }
                            
                            ++gFCount;
                            break;
                        }
                            
                        default:
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                    }
                }
                
                // take charge the last group
                if(gFCount != 0){
                    // have a last group
                    sgSceneObject *obj = objGroups[gCount];
                    sgMesh *mesh = groups[gCount];
                    mesh->setupNormals();
                    mesh->prepareGeometry();
                    obj->translate(mesh->center());
                    mesh->locateToCenter();
                    
                    vertices = 0;
                    normals = 0;
                    texcoords = 0;
                    vindices = 0;
                    nindices = 0;
                    tindices = 0;
                    
                    gCount++;
                    
                    tVCount += gVCount;
                    tNCount += gNCount;
                    tTCount += gTCount;
                    
                    gVCount = 0;
                    gNCount = 0;
                    gTCount = 0;
                    gFCount = 0;
                }
                // loading end
            }
		}
        else
        {
            isobj = false;
			
		}
        
        if(!isobj)
        {
            THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                              "The file is not .obj file.",
                              "sgLoaderObj");
        }
        
        return sceneRoot;
        
    }
    
    struct _MeshInfo
    {
        std::string name;
        sg_vector(Vector3) vertex;
        sg_vector(Vector3) normal;
        sg_vector(Vector2) uv0;
        sg_vector(size_t) vertexIndex;
        sg_vector(size_t) normalIndex;
        sg_vector(size_t) uv0Index;
        UInt8 polyType;
        size_t faceNum;
    };
    typedef sg_vector(_MeshInfo) _MeshInfoList;
    
    sgSceneObject *sgLoader::load_obj2(const std::string &filename)
    {
        sgSceneObject *sceneRoot = NULL;
        
        size_t lastslash = filename.find_last_of("\\/");
		size_t lastdot = filename.find_last_of(".");
        
        bool isobj = true;
        if(lastdot != std::string::npos)
        {
            std::string ext = filename.substr(lastdot);
            if(ext != ".obj")
            {
                isobj = false;
            }
			else
            {
                isobj = true;
                
                std::ifstream file((sgResourceCenter::instance()->getRootDir() + filename).c_str());
                if(file.fail())
                {
                    THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                                      "cannot open the obj file: " + filename + ".",
                                      "sgLoaderObj");
                    
                }
                
                // load from obj
                std::string sceneName;
                if(lastslash == std::string::npos)
                {
                    sceneName = filename.substr(0, lastdot);
                }
                else
                {
                    sceneName = filename.substr(lastslash + 1, lastdot);
                }
                
                size_t gCount = 0;			// group count
                size_t gVCount = 0;			// current group vertex count
                size_t gTCount = 0;			// current group texcoord count
                size_t gNCount = 0;			// current group normal count
                //size_t gFCount = 0;			// current group face count
                
                size_t tVCount = 0;			// total vertex count till current
                size_t tTCount = 0;			// total texcoord count till current
                size_t tNCount = 0;			// total normal count till current
            
                
                // store the state
                // 1 - in v, 2 - in face
                int state = 2;
                
                _MeshInfo *currentMesh = NULL;
                _MeshInfoList meshList;
                
                Real x, y, z;
                int v, n, t;
                
                // for get line
                std::vector<std::string> words;
                
                std::string strLine;
                
                char c;
                while(file >> c){
                    switch(c){
                        case '#':		// comment
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'v':		// v, vn, vt
                        {
                            if(state != 1)
                            {
                                // start a new group
                                // first take charge latest group
                                meshList.push_back(_MeshInfo());
                                currentMesh = &(meshList[meshList.size()-1]);
                                currentMesh->name = sceneName + "_mesh_" + sgStringUtil::to_string(meshList.size());
                                currentMesh->faceNum = 0;
                                
                                ++gCount;
                                
                                tVCount += gVCount;
                                tNCount += gNCount;
                                tTCount += gTCount;
                                
                                gVCount = 0;
                                gNCount = 0;
                                gTCount = 0;
                                
                                // now we are in 'v' area
                                state = 1;
                            }
                            
                            c = file.get();
                            switch(c){
                                case ' ':	// vertex
                                {
                                    file >> x >> y >> z;
                                    currentMesh->vertex.push_back(Vector3(x, y, z));
                                    
                                    ++gVCount;
                                    
                                    break;
                                }
                                    
                                case 'n':	// normal
                                {
                                    file >> x >> y >> z;
                                    currentMesh->normal.push_back(Vector3(x, y, z));
                                    
                                    ++gNCount;
                                    
                                    break;
                                }
                                    
                                case 't':	// texcoord
                                {
                                    file >> x >> y;
                                    currentMesh->uv0.push_back(Vector2(x, y));
                                    
                                    ++gTCount;
                                    
                                    break;
                                }
                                    
                                default:	// unknown
                                {
                                    file.close();
                                    THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                                                      "Unknown token after \"v\".",
                                                      "sgSceneDelegate::loadScene_obj");
                                }
                            }
                            
                            break;
                        } //#### end v switch
                            
                        case 'm':		// mtl file
                        {
                            // read mtl file
                            // for future ...
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'u':		// nothing
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                        case 'g':		// group
                        {
                            if(state == 1)
                            {
                                // next into 'f' area
                                //file.getLine(&words);
                                std::getline(file, strLine);
                                sgStringUtil::split(words, strLine);
                                if(!words.empty()){
                                    currentMesh->name = words[0];
                                }
                            }
                            else
                            {
                                // eat up rest of line
                                std::getline(file, strLine);
                            }
                            
                            break;
                        }
                            
                        case 'p':   // point
                        case 'l':   // line
                        case 'f':		// face
                        {
                            if(state != 2)
                            {
                                // start a group
                                state = 2;
                                
                                currentMesh->polyType =
                                    (c == 'f' ? 3 : ( c== 'l' ? 2 : 1));
                            }
                            
                            v = t = n = 0xffffffff;
                            for(uInt i=0; i<currentMesh->polyType; ++i){
                                if(gTCount == 0 && gNCount == 0)
                                {
                                    // v
                                    file >> v;
                                }
                                else if(gTCount > 0 && gNCount == 0)
                                {
                                    // v/t
                                    file >> v >> c >> t;
                                }
                                else if(gTCount == 0 && gNCount > 0)
                                {
                                    // v//n
                                    file >> v >> c >> c >> n;
                                }
                                else
                                {
                                    // v/t/n
                                    file >> v >> c >> t >> c >> n;
                                }
                                if(v > 0)
                                {
                                    v = v - tVCount - 1;
                                    if(gTCount > 0)
                                        t = t - tTCount - 1;
                                    if(gNCount > 0)
                                        n = n - tNCount - 1;
                                }
                                else
                                {
                                    v = v + gVCount;
                                    if(gTCount > 0)
                                        t = t + gTCount;
                                    if(gNCount > 0)
                                        n = n + gNCount;
                                }
                                if(gVCount > 0)
                                    currentMesh->vertexIndex.push_back(v);
                                if(gTCount > 0)
                                    currentMesh->uv0Index.push_back(t);
                                if(gNCount > 0)
                                    currentMesh->normalIndex.push_back(n);
                                
                                
                            }
                            
                            //++gFCount;
                            ++(currentMesh->faceNum);
                            
                            break;
                        }
                            
                        default:
                        {
                            // eat up rest of line
                            std::getline(file, strLine);
                            break;
                        }
                            
                    }
                }
                // loading end
                
                // construct meshes
                sceneRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
                sceneRoot->setName(sceneName + "_Node");
                
                for(size_t i=0; i<meshList.size(); ++i)
                {
                    _MeshInfo &meshInfo = meshList[i];
                    
                    if(meshInfo.vertex.empty()
                       || meshInfo.vertexIndex.empty()
                       || meshInfo.faceNum == 0)
                    {
                        continue ;
                    }
                    
                    sgMesh *mesh = (sgMesh*)sgResourceCenter::instance()->createResource(sgMesh::GetClassName(), (filename + meshInfo.name + "_mesh").c_str());
                    mesh->reset(meshInfo.polyType, meshInfo.vertex.size(), meshInfo.faceNum);
                    
                    sgVertexData *vdata = mesh->getVertexData();
                    sgIndexData *idata = mesh->getIndexData();
                    
                    Vector3 *vertices = NULL;
                    Vector3 *normals = NULL;
                    Vector2 *uv0s = NULL;
                    size_t *vindices = NULL;
                    size_t *nindices = NULL;
                    size_t *tindices = NULL;
                    
                    // vertex
                    {
                        vertices = static_cast<Vector3*>( vdata->createElement(sgVertexBufferElement::VertexAttributeName, RDT_F, 3, meshInfo.vertex.size())->data() );
                        vindices = static_cast<size_t*>( idata->createElement(sgVertexBufferElement::ET_VERTEX)->data() );
                        
                        for(size_t i=0; i<meshInfo.vertex.size(); ++i)
                        {
                            vertices[i] = meshInfo.vertex[i];
                        }
                        for(size_t i=0; i<meshInfo.vertexIndex.size(); ++i)
                        {
                            vindices[i] = meshInfo.vertexIndex[i];
                        }
                    }
                    
                    // normal
                    if(!meshInfo.normal.empty() && !meshInfo.normalIndex.empty())
                    {
                        normals = static_cast<Vector3*>( vdata->createElement(sgVertexBufferElement::NormalAttributeName, RDT_F, 3, meshInfo.normal.size())->data() );
                        nindices = static_cast<size_t*>( idata->createElement(sgVertexBufferElement::ET_NORMAL)->data() );
                        
                        for(size_t i=0; i<meshInfo.normal.size(); ++i)
                        {
                            normals[i] = meshInfo.normal[i];
                        }
                        for(size_t i=0; i<meshInfo.normalIndex.size(); ++i)
                        {
                            nindices[i] = meshInfo.normalIndex[i];
                        }
                    }
                    
                    // uv0
                    if(!meshInfo.uv0.empty() && !meshInfo.uv0Index.empty())
                    {
                        uv0s = static_cast<Vector2*>( vdata->createElement(sgVertexBufferElement::UV0AttributeName, RDT_F, 2, meshInfo.uv0.size())->data() );
                        tindices = static_cast<size_t*>( idata->createElement(sgVertexBufferElement::ET_TEXTURE_COORD)->data() );
                        
                        for(size_t i=0; i<meshInfo.uv0.size(); ++i)
                        {
                            uv0s[i] = meshInfo.uv0[i];
                        }
                        for(size_t i=0; i<meshInfo.uv0Index.size(); ++i)
                        {
                            tindices[i] = meshInfo.uv0Index[i];
                        }
                    }
                    
                    mesh->prepareGeometry();
                    
                    Vector3 originalMeshCenter = mesh->center();
                    
                    mesh->locateToCenter();
                    
                    // create scene objects
                    sgSceneObject *object = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassName());
                    object->setName(meshInfo.name + "_node");
                    object->setParent(sceneRoot);
                    sgMeshComponent *meshComp = (sgMeshComponent*)object->createComponent(sgMeshComponent::GetClassName());
                    meshComp->setMeshFile(mesh->getFilename());
                    object->translate(originalMeshCenter);
                    
                }
                
                
                
                
            }
		}
        else
        {
            isobj = false;
			
		}
        
        if(!isobj)
        {
            THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                              "The file is not .obj file.",
                              "sgLoaderObj");
        }
        
        return sceneRoot;
        
    }
    
} // namespace Sagitta

