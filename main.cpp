#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp" //per disegno

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;

int main()
{
    PolygonalMesh mesh;     //creo un oggetto mesh
    if(!ImportMesh(mesh))
    {
        cerr<<"file not found"<<endl;
        return 1;
    }

//verifico se esistono lati nulli
    if(!CalcolaLati(mesh))
        {
            cerr << "c'è un lato nullo" << endl;
            return 1;
        }
    else  
    {
        cout<<"Non ci sono lati nulli"<<endl;
    };
//verifico se esistono poligoni con area nulla
     if(!CalcolaArea(mesh))
    {
        cerr << "c'è una figura di area nulla" << endl;
        return 1;
    }
    else  
    {
        cout<<"Non ci sono poligoni di area nulla"<<endl;
    };

    VerificaMarker(mesh);

    UCDUtilities utilities;
    utilities.ExportPoints("./MeshCompleta.inp", 
                            mesh.Cell0DsCoordinates);
    utilities.ExportSegments("./MeshCompleta.inp", 
                            mesh.Cell0DsCoordinates,
                            mesh.Cell1DsExtremes);


    return 0;
}

