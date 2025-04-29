#pragma once

#include <vector>
#include<iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
	bool ImportMesh(PolygonalMesh& mesh);	    //chiama le funzioni che importano Cell0Ds, Cell1Ds, Cell2Ds 
	bool ImportCell0Ds(PolygonalMesh& mesh);    //importa i punti e li salva 
	bool ImportCell1Ds(PolygonalMesh& mesh);	//importa i segmenti e li salva 
	bool ImportCell2Ds(PolygonalMesh& mesh);	//importa i poligoni e li salva 

	bool CalcolaLati(PolygonalMesh& mesh);		//verifica che non ci siano lati nulli
	bool CalcolaArea(PolygonalMesh& mesh);		//verifica che non ci siano poligoni di area nulla
	void VerificaMarker(PolygonalMesh& mesh);	//verifica che i marker siano stati salvati correttamente
}