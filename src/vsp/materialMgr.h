//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// materialMgr.h: interface for the materialMgr class.
// J.R. Gloudemans
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALMGR_H__635B9760_A025_11D7_AC31_0003473A025A__INCLUDED_)
#define AFX_MATERIALMGR_H__635B9760_A025_11D7_AC31_0003473A025A__INCLUDED_

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "xmlvsp.h"
#include <stdio.h>
#include "stringc.h"
#include <vector>				//jrg windows??	
//using namespace std;			//jrg windows??

class Material
{
public:
	Material();
	virtual ~Material()		{}

	Stringc name;
	float amb[4];
	float diff[4];
	float spec[4];
	float emiss[4];
	float shine;

	float alpha;
	int alphaFlag;

	void read( xmlNodePtr mat_node );
	void bind();

};

class MaterialMgr
{
public:

	MaterialMgr();
	virtual ~MaterialMgr();

	virtual void readFile( xmlNodePtr mat_node );

	virtual int  getNumMaterial()				{ return matVec.size(); }
	virtual Material* getMaterial(int i);

	virtual Material getMaterialCopy(int i);

	virtual Material getWhiteMaterial( float brightness, float shine );

private:
	vector< Material* > matVec;
};


class Single
{
public:
	Single();
	MaterialMgr* matMgr;
};


static Single single;

#define matMgrPtr (single.matMgr)












   

#endif



