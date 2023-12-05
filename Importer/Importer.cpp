#include "Importer.h"

#include "../Mesh/Mesh.h"

Importer::Importer(Mesh* mesh): m_mesh(mesh)
{}

Importer::~Importer()
{}


void Importer::loadObjFile(string fileName)
{
    map<int, list<MeshVertex*>> vertexList;

    vector<vec3> coordList;
    vector<vec2> texCoordList;
    vector<vec3> normalList;

    ifstream inputStream;
    inputStream.open(fileName.c_str(), ifstream::in);

    char line[180];
    while(inputStream.getline(line, sizeof(line)))
    {
        char* word = NULL;
        word = strtok(line, " ");
        if(word == NULL) continue;

        if(strcmp(word, "v") == 0)
        {
            //vertex coord
            float x = atof(strtok(NULL, " "));
            float y = atof(strtok(NULL, " "));
            float z = atof(strtok(NULL, " "));

            coordList.push_back(vec3(x, y, z));
        }
        else if(strcmp(word, "vt") == 0)
        {
            //texture coord
            float u = atof(strtok(NULL, " "));
            float v = atof(strtok(NULL, " "));

            texCoordList.push_back(vec2(u, v));
        }
        else if(strcmp(word, "vn") == 0)
        {
            //normal
            float nx = atof(strtok(NULL, " "));
            float ny = atof(strtok(NULL, " "));
            float nz = atof(strtok(NULL, " "));

            normalList.push_back(vec3(nx, ny, nz));
        }
        if(strcmp(word, "f") == 0)
        {
            vector<MeshVertex*> vertex;

            //first vertex index
            word = strtok(NULL, " ");
            MeshVertex* v1 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList);
            vertex.push_back(v1);

            //second vertex index
            word = strtok(NULL, " ");
            MeshVertex* v2 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList);
            vertex.push_back(v2);

            //other vertex index
            while((word = strtok(NULL, " ")))
            {
                MeshVertex* v3 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList);
                vertex.push_back(v3);
                v2 = v3;
            }
            m_mesh->addPolygon(vertex);
        }
    }
    inputStream.close();
}

MeshVertex* Importer::findOrCreateVertex(char* nvntnn,
                                         map<int, list<MeshVertex*>> &vertexList,
                                         vector<vec3> &coordList,
                                         vector<vec2> &texCoordList,
                                         vector<vec3> &normalList)
{
    //index extracting
    int nv, nt, nn;
    int ssc = sscanf(nvntnn, "%d/%d/%d", &nv, &nt, &nn);
    if( ssc == 2)
    {
        sscanf(nvntnn, "%d/%d/", &nv, &nt);
        nn = 0;
    }
    else if (ssc == 1)
    {
        ssc = sscanf(nvntnn, "%d//%d", &nv, &nn);
        if(ssc == 2)
        {
            nt = 0;
        }
        else
        {
            sscanf(nvntnn, "%d//", &nv);
            nt = 0; nn = 0;
        }
    }
    
    nv--; nt--; nn--;    

    //vertex name
    stringstream ss;
    ss << "v(" << nv << ", " << nt << ", " << nn << ")";
    string name = ss.str();

    //search vertex in vertexList
    for(MeshVertex* s: vertexList[nv])
    {
        if(name == s->getName())
        {
            return s;
        }
    }
    
    //create vertex
    MeshVertex* vertex = m_mesh->addVertex(name);
    vertexList[nv].push_front(vertex);
    
    //vertex settings
    vertex->setCoord(coordList[nv]);
    if(nt >= 0)
        vertex->setUv(texCoordList[nt]);
    if(nn >= 0)
        vertex->setNormal(normalList[nn]);
    vertex->setColor(vec3(0.0, 0.0, 0.0)); //white color for an AWESOME render

    return vertex; //to construct polygon
}