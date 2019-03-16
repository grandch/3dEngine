#include "Importer.h"

#include "Mesh/Mesh.h"

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
        word = strtok(line, " "); //cut the string between words
        if(word == NULL) continue;

        if(strcmp(word, "v") == 0)
        {
            //vertex coord
            float x = atof(strtok(NULL, " "));
            float y = atof(strtok(NULL, " "));
            float z = atof(strtok(NULL, " "));

            coordList.push_back(vec3(x, y, z));
            stringstream ss;
            ss << "v(" << x << ", " << y << ", " << z << ")";
            string name = ss.str();
            MeshVertex* vertex = new MeshVertex(m_mesh, name);
            vertex->setCoord(vec3(x, y, z));
            vertex->setColor(vec3(1.0, 1.0, 1.0));
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
            //face 
            //first vertex index
            word = strtok(NULL, " ");
            MeshVertex* v1 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList); //gestion des index de normal et de texture !!!!!!!!!!!

            //second vertex index
            word = strtok(NULL, " ");
            MeshVertex* v2 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList);

            //other vertex index
            while((word = strtok(NULL, " ")))
            {
                MeshVertex* v3 = findOrCreateVertex(word, vertexList, coordList, texCoordList, normalList);
                m_mesh->addTriangle(v1, v2, v3);
                v2 = v3;
            }
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
    int nv, nn;
    sscanf(nvntnn, "%d//%d", &nv, &nn);
    nv--; nn--;

    //vertex name
    stringstream ss;
    ss << "v(" << coordList[nv].x << ", " << coordList[nv].y << ", " << coordList[nv].z << ")";
    string name = ss.str();

    //find vertex
    for(MeshVertex* s: vertexList[nv])
    {
        if(name == s->getName())
        {
            cout << "vertex found" << endl;
            return s;
        }
    }

    //create vertex
    MeshVertex* vertex = m_mesh->addVertex(name);
    //vertexList[nv].push_front(vertex);
    //vertex->setCoord(coordList[nv]);
    //vertex->setTexCoord(texCoordList[nt]);
    //vertex->setNormal(normalList[nn]);

    return vertex;
}