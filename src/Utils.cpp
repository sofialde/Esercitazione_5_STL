#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;
namespace PolygonalLibrary {

bool ImportMesh(PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;
}

// ***************************************************************************

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
    {
        cerr<<"file Cell0Ds non trovato"<<endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines) 
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        string variabile;

        getline(converter, variabile, ';');
        id = stoi(variabile);
        getline(converter, variabile, ';');
        marker = stoi(variabile);
        getline(converter, variabile, ';');
        mesh.Cell0DsCoordinates(0, id) = stod(variabile);
        getline(converter, variabile, ';');
        mesh.Cell0DsCoordinates(1, id) = stod(variabile);

        mesh.Cell0DsId.push_back(id);

        if(marker != 0)
        {
            auto it = mesh.MarkerCell0Ds.find(marker);
            if(it != mesh.MarkerCell0Ds.end())
                (*it).second.push_back(id);
            else
                mesh.MarkerCell0Ds.insert({marker, {id}});
        }
    }

    return true;
}

// ***************************************************************************

bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
    {
        cerr<<"file Cell1Ds non trovato"<<endl;
        return false;
    }

    list<string> listLines;
    string line;

    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtremes = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        string variabile;

        getline(converter, variabile, ';');
        id = stoi(variabile);
        getline(converter, variabile, ';');
        marker = stoi(variabile);
        getline(converter, variabile, ';');
        mesh.Cell1DsExtremes(0, id) = stoi(variabile);
        getline(converter, variabile, ';');
        mesh.Cell1DsExtremes(1, id) = stoi(variabile);

        mesh.Cell1DsId.push_back(id);

        if(marker != 0)
        {
            auto it = mesh.MarkerCell1Ds.find(marker);
            if(it != mesh.MarkerCell1Ds.end())
                (*it).second.push_back(id);
            else
                mesh.MarkerCell1Ds.insert({marker, {id}});
        }
    }

    return true;
}

// ***************************************************************************

bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell2Ds.csv"); 

    if(file.fail())
    {
        cerr<<"file Cell2Ds non trovato"<<endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        
        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        string variabile;

        getline(converter, variabile, ';');
        id = stoi(variabile);
        getline(converter, variabile, ';');
        marker = stoi(variabile);
        getline(converter, variabile, ';');
        NumVertices = stoi(variabile);
         
        for(unsigned int i=0; i < NumVertices; i++)
        {
            getline(converter, variabile, ';');
            vertices.push_back(stoi(variabile));
        }

        getline(converter, variabile, ';');
        NumEdges = stoi(variabile);
        for(unsigned int i=0; i < NumEdges; i++)
        {
            getline(converter, variabile, ';');
            edges.push_back(stoi(variabile));
        }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);

        if(marker != 0)
        {
            auto it = mesh.MarkerCell2Ds.find(marker);
            if(it != mesh.MarkerCell2Ds.end())
                (*it).second.push_back(id);
            else
                mesh.MarkerCell2Ds.insert({marker, {id}});
        }
    }

    return true;
}

// ***************************************************************************

bool CalcolaArea(PolygonalMesh& mesh)
{
    for (const auto& riga : mesh.Cell2DsEdges)
    {
        double Area = 0.0;
        for(unsigned int i = 0; i < riga.size(); i++)
        {
            int idLato = riga[i];
            int idPunto1 = mesh.Cell1DsExtremes(0, idLato);
            int idPunto2 = mesh.Cell1DsExtremes(1, idLato);

            double x1 = mesh.Cell0DsCoordinates(0, idPunto1);
            double y1 = mesh.Cell0DsCoordinates(1, idPunto1);

            double x2 = mesh.Cell0DsCoordinates(0, idPunto2);
            double y2 = mesh.Cell0DsCoordinates(1, idPunto2);

            Area += x1 * y2 - x2 * y1;
        }

        Area = 0.5 * abs(Area); 

        if(Area < numeric_limits<double>::epsilon() * sqrt(3) / 4.0)
            return false;
    }

    return true;
}

// ***************************************************************************

bool CalcolaLati(PolygonalMesh& mesh)
{
    for(int i = 0; i < mesh.Cell1DsExtremes.cols(); i++)
    {
        int idPunto1 = mesh.Cell1DsExtremes(0, i);
        int idPunto2 = mesh.Cell1DsExtremes(1, i);

        double lato1 = mesh.Cell0DsCoordinates(0, idPunto1) - mesh.Cell0DsCoordinates(0, idPunto2);
        double lato2 = mesh.Cell0DsCoordinates(1, idPunto1) - mesh.Cell0DsCoordinates(1, idPunto2);
        double distanza_quadra = sqrt(lato1 * lato1 + lato2 * lato2);

        if(abs(distanza_quadra) < numeric_limits<double>::epsilon())
            return false;
    }

    return true;
}

// ***************************************************************************

void VerificaMarker(PolygonalMesh& mesh)
{
    cout << "Verifico che i marker dei punti siano corretti" << endl << endl;
    for(const auto& pair : mesh.MarkerCell0Ds)
    {
        cout << "Marker: " << pair.first << endl;
        cout << "Id dei Punti associati: ";
        for(const unsigned int& IdPunti : pair.second)
            cout << IdPunti << " ";
        cout << endl;
    }

    cout << endl << "Verifico che i marker dei lati siano corretti" << endl << endl;
    for(const auto& pair : mesh.MarkerCell1Ds)
    {
        cout << "Marker: " << pair.first << endl;
        cout << "Id dei Lati associati: ";
        for(const unsigned int& IdLati : pair.second)
            cout << IdLati << " ";
        cout << endl;
    }
}

} // namespace PolygonalLibrary



